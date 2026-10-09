#include "Game.hpp"
#include "Collision.hpp"
#include "Screen.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>

Game::Game()
    : _background(nullptr), _bg_scale(1.0), _bg_offset_x(0.0), _bg_offset_y(0.0),
      _bgm(nullptr), _melee_cooldown(0), _ranged_cooldown(0), _player_hit_cooldown(0)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    open_window("Survival War: Pixel", Screen::WIDTH, Screen::HEIGHT);

    load_audio();
    load_background();

    _player.set_start_position(200, 100);
    Enemy::spawn_wave(_enemies, _player.get_x(), _player.get_y());
}

void Game::load_audio()
{
    _bgm = load_music("bgm", "Resources/sounds/cello.mp3");
    if (_bgm != nullptr)
    {
        set_music_volume(0.6f);
        play_music(_bgm, -1);
        write_line("Background music loaded and playing successfully!");
    }
    else
    {
        write_line("Warning: Could not load background music file.");
    }
}

void Game::load_background()
{
    _background = load_bitmap("forest_bg", "Resources/sprites/Forest/TX Tileset Grass.png");

    if (_background == nullptr)
    {
        write_line("ERROR: Failed 'Resources/sprites/Forest/TX Tileset Grass.png'");
        return;
    }

    double img_width = bitmap_width(_background);
    double img_height = bitmap_height(_background);

    // Scale to cover the whole window, then center the overflow
    double scale_x = Screen::WIDTH / img_width;
    double scale_y = Screen::HEIGHT / img_height;
    _bg_scale = (scale_x > scale_y) ? scale_x : scale_y;

    double scaled_w = img_width * _bg_scale;
    double scaled_h = img_height * _bg_scale;

    _bg_offset_x = (Screen::WIDTH - scaled_w) / 2.0;
    _bg_offset_y = (Screen::HEIGHT - scaled_h) / 2.0;
}

void Game::handle_melee_attack()
{
    if (_melee_cooldown > 0)
        _melee_cooldown--;

    if (!_player.is_attacking() || _melee_cooldown != 0)
        return;

    rectangle atk_range = _player.get_melee_range();

    for (auto &enemy : _enemies)
    {
        if (!enemy->is_active())
            continue;

        if (rectangles_intersect(atk_range, enemy->get_bounds()))
        {
            enemy->take_damage(_player.get_attack());
            _melee_cooldown = 30; // prevent multiple hits per swing
            write_line("Player hit enemy with sword!");
            break; // only hit one enemy per swing
        }
    }
}

void Game::handle_ranged_attack()
{
    if (_ranged_cooldown > 0)
        _ranged_cooldown--;

    if (!mouse_clicked(RIGHT_BUTTON) || _ranged_cooldown != 0 || _player.is_dead())
        return;

    Bullet bullet;
    bullet.fire(_player.get_x(), _player.get_y(), mouse_x(), mouse_y());
    _bullets.push_back(bullet);
    _ranged_cooldown = 15; // ~4 shots per second at 60 FPS
}

void Game::update_bullets()
{
    for (auto &bullet : _bullets)
        bullet.update();

    for (auto &bullet : _bullets)
    {
        if (!bullet.is_active())
            continue;

        for (auto &enemy : _enemies)
        {
            if (enemy->is_active() && Collision::bullet_hits_enemy(bullet, *enemy))
            {
                enemy->take_damage(BULLET_DAMAGE);
                bullet.deactivate();
                write_line("Bullet hit enemy!");
                break;
            }
        }
    }

    // Clean up inactive bullets so the vector doesn't grow forever
    _bullets.erase(
        std::remove_if(_bullets.begin(), _bullets.end(),
                        [](const Bullet &b) { return !b.is_active(); }),
        _bullets.end());
}

void Game::update_enemies()
{
    for (auto &enemy : _enemies)
    {
        enemy->update(_player.get_x(), _player.get_y());
    }
}

void Game::resolve_combat()
{
    // Player touching an enemy (melee contact damage)
    if (_player_hit_cooldown > 0)
        _player_hit_cooldown--;

    if (_player_hit_cooldown == 0)
    {
        for (auto &enemy : _enemies)
        {
            if (enemy->is_active() && Collision::player_hits_enemy(_player, *enemy))
            {
                _player.lose_heart();
                _player_hit_cooldown = 60; // 1 second cooldown at 60 FPS
                break;
            }
        }
    }

    // Enemy attack animations that landed a hit this frame
    for (auto &enemy : _enemies)
    {
        if (enemy->is_active() && enemy->consume_damage_flag())
        {
            _player.lose_heart();
            write_line("Enemy attack hit! Player health: " + std::to_string(_player.get_hearts()));
        }
    }
}

void Game::render()
{
    clear_screen(COLOR_BLACK);

    if (_background != nullptr)
    {
        drawing_options opts = option_scale_bmp(_bg_scale, _bg_scale);
        draw_bitmap(_background, _bg_offset_x, _bg_offset_y, opts);
    }
    else
    {
        clear_screen(COLOR_DARK_GREEN);
    }

    _player.draw();

    if (_player.is_attacking())
    {
        draw_rectangle(COLOR_RED, _player.get_melee_range());
    }

    for (auto &enemy : _enemies)
    {
        if (enemy->is_active())
            enemy->draw();
    }

    for (auto &bullet : _bullets)
        bullet.draw();

    // UI drawn last so it stays on top
    _player.draw_hud();
    draw_text("SPACE: sword  |  Right-click: shoot", COLOR_WHITE, "Arial", 14, 10, 570);
}

void Game::show_game_over()
{
    stop_music();

    // Fade to black over ~1.4s while keeping the last death frame visible
    for (int alpha = 0; alpha <= 255; alpha += 3)
    {
        process_events();
        clear_screen(COLOR_BLACK);
        _player.draw();

        color fade = rgba_color(0, 0, 0, alpha);
        fill_rectangle(fade, 0, 0, Screen::WIDTH, Screen::HEIGHT);

        refresh_screen(60);
    }

    delay(1000);

    clear_screen(COLOR_BLACK);
    draw_text("GAME OVER", COLOR_RED, "Arial", 80, 250, 250);
    draw_text("Press ESC to exit", COLOR_WHITE, "Arial", 24, 280, 320);
    refresh_screen(60);

    while (!quit_requested() && !key_typed(ESCAPE_KEY))
    {
        process_events();
        delay(100);
    }
}

void Game::run()
{
    while (!quit_requested())
    {
        process_events();

        _player.handle_input();
        _player.update_health();

        update_enemies();
        handle_melee_attack();
        handle_ranged_attack();
        update_bullets();
        resolve_combat();

        render();

        if (_player.is_dead() && _player.is_dying_animation_done())
        {
            show_game_over();
            break;
        }

        refresh_screen(60);
    }

    stop_music();
    close_window("Survival War: Pixel");
}

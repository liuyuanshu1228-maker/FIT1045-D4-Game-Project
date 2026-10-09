#include "Enemy.hpp"
#include "Screen.hpp"
#include "splashkit.h"
#include <cmath>
#include <cstdlib>

namespace
{
    constexpr double MIN_DISTANCE_FROM_PLAYER = 100.0;
    constexpr double MIN_DISTANCE_BETWEEN_ENEMIES = 80.0;
    constexpr int MAX_SPAWN_ATTEMPTS = 50;
}

Enemy::Enemy()
    : Character(0, 0, 50, 50, 1.0, 1),
      _health(100), _detect_radius(200.0),
      _attacking(false),
      _wander_target_x(0), _wander_target_y(0), _wander_timer(0),
      _wander_cooldown(60 + rand() % 180),
      _wander_speed_modifier(0.3 + (rand() % 50) / 100.0),
      _rest_duration(0), _rest_timer(0), _resting(false),
      _laziness((rand() % 80) / 100.0),
      _melee_range(50.0),
      _damage_pending(false), _has_hit_this_swing(false),
      _melee_cooldown_after(60), _melee_cooldown_timer(0)
{
    _walk_anim.load("enemy_sheet", "Resources/sprites/Enemy/Orc-Walk.png", 100, 100, 8, 10);
    _attack_anim.load("enemy_attack", "Resources/sprites/Enemy/Orc_Attack02.png", 100, 100, 8, 10);
}

void Enemy::set_start_position(double x, double y)
{
    _x = x;
    _y = y;
    _wander_target_x = x;
    _wander_target_y = y;
}

void Enemy::update(double target_x, double target_y)
{
    bool was_moving = false;

    if (_melee_cooldown_timer > 0)
        _melee_cooldown_timer--;

    double dx = target_x - _x;
    double dy = target_y - _y;
    double distance = sqrt(dx * dx + dy * dy);

    if (distance < _detect_radius && distance > 0)
    {
        if (distance <= _melee_range && _melee_cooldown_timer == 0 && !_attacking)
        {
            _attacking = true;
            _attack_anim.reset();
            _has_hit_this_swing = false;
        }
        else if (!_attacking)
        {
            _x += _speed * dx / distance;
            _y += _speed * dy / distance;
            was_moving = true;
        }

        if (_attacking)
        {
            // Hit frame: deal damage once per swing, the first time it reaches frame 3,
            // and only if the player hasn't stepped out of range since the swing started.
            if (_attack_anim.current_frame() == 3 && !_has_hit_this_swing)
            {
                _has_hit_this_swing = true;
                if (distance <= _melee_range)
                {
                    _damage_pending = true;
                }
            }

            if (_attack_anim.step_once_reset())
            {
                _attacking = false;
                _has_hit_this_swing = false;
                _melee_cooldown_timer = _melee_cooldown_after;
            }
        }
    }
    else
    {
        was_moving = update_wander_ai();
    }

    clamp_to_screen();

    if (!_attacking)
    {
        if (was_moving)
            _walk_anim.step_loop();
        else
            _walk_anim.reset();
    }
}

bool Enemy::update_wander_ai()
{
    if (_resting)
    {
        _rest_timer--;
        if (_rest_timer <= 0)
            _resting = false;
        return false;
    }

    _wander_timer++;
    if (_wander_timer >= _wander_cooldown)
    {
        _wander_timer = 0;

        if ((rand() % 100) / 100.0 < _laziness)
        {
            _resting = true;
            _rest_duration = 30 + (rand() % 150);
            _rest_timer = _rest_duration;
            return false;
        }

        _wander_target_x = _x + (rand() % 200 - 100);
        _wander_target_y = _y + (rand() % 200 - 100);

        if (_wander_target_x < 0)
            _wander_target_x = 0;
        if (_wander_target_x > Screen::WIDTH - _width)
            _wander_target_x = Screen::WIDTH - _width;
        if (_wander_target_y < 0)
            _wander_target_y = 0;
        if (_wander_target_y > Screen::HEIGHT - _height)
            _wander_target_y = Screen::HEIGHT - _height;
    }

    double wx = _wander_target_x - _x;
    double wy = _wander_target_y - _y;
    double wander_distance = sqrt(wx * wx + wy * wy);

    if (wander_distance > 5.0)
    {
        double move_speed = _speed * _wander_speed_modifier;
        _x += move_speed * wx / wander_distance;
        _y += move_speed * wy / wander_distance;
        return true;
    }
    return false;
}

void Enemy::draw() const
{
    if (!_active)
        return;

    if (_attacking && _attack_anim.is_loaded())
    {
        _attack_anim.draw(_x, _y);

        double bar_width = _width * 0.3;
        double bar_height = 4;
        double ratio = static_cast<double>(_health.current()) / _health.max();
        double bar_x = _x + (_width - bar_width) / 2;
        double bar_y = _y + 5;
        fill_rectangle(COLOR_RED, bar_x, bar_y, bar_width, bar_height);
        fill_rectangle(COLOR_GREEN, bar_x, bar_y, bar_width * ratio, bar_height);
        return;
    }

    if (_walk_anim.is_loaded())
    {
        _walk_anim.draw(_x, _y);
    }
    else
    {
        fill_rectangle(COLOR_BLACK, _x, _y, _width, _height);
    }

    double bar_width = _width * 0.3;
    double bar_height = 2;
    double ratio = static_cast<double>(_health.current()) / _health.max();
    double bar_x = _x + (_width - bar_width) / 2;
    double bar_y = _y - 8;
    fill_rectangle(COLOR_RED, bar_x, bar_y, bar_width, bar_height);
    fill_rectangle(COLOR_GREEN, bar_x, bar_y, bar_width * ratio, bar_height);
}

void Enemy::take_damage(int damage)
{
    _health.damage(damage);
    if (_health.is_depleted())
    {
        deactivate();
    }
}

bool Enemy::consume_damage_flag()
{
    if (_damage_pending)
    {
        _damage_pending = false;
        return true;
    }
    return false;
}

void Enemy::spawn_wave(std::vector<std::unique_ptr<Enemy>> &enemies, double player_x, double player_y)
{
    enemies.clear();

    int num_enemies = 4 + rand() % 3; // spawn 4 to 6 enemies each time

    for (int i = 0; i < num_enemies; i++)
    {
        bool valid_position = false;
        double start_x = 0, start_y = 0;
        int attempts = 0;

        while (!valid_position && attempts < MAX_SPAWN_ATTEMPTS)
        {
            start_x = 50 + rand() % 700; // 50 to 750 (screen width 800)
            start_y = 50 + rand() % 500; // 50 to 550 (screen height 600)

            double dx = start_x - player_x;
            double dy = start_y - player_y;
            double dist_from_player = sqrt(dx * dx + dy * dy);

            if (dist_from_player < MIN_DISTANCE_FROM_PLAYER)
            {
                attempts++;
                continue;
            }

            bool too_close_to_other = false;
            for (const auto &e : enemies)
            {
                double ex = e->get_x();
                double ey = e->get_y();
                double enemy_dx = start_x - ex;
                double enemy_dy = start_y - ey;
                double dist_from_enemy = sqrt(enemy_dx * enemy_dx + enemy_dy * enemy_dy);

                if (dist_from_enemy < MIN_DISTANCE_BETWEEN_ENEMIES)
                {
                    too_close_to_other = true;
                    break;
                }
            }

            valid_position = !too_close_to_other;
            attempts++;
        }

        auto new_enemy = std::make_unique<Enemy>();
        new_enemy->set_start_position(start_x, start_y);
        enemies.push_back(std::move(new_enemy));
    }
}

#ifndef GAME_HPP
#define GAME_HPP

#include "splashkit.h"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include <vector>
#include <memory>

// Owns all game state and runs the main loop. Replaces the old free-function
// main() that mixed window setup, input, physics, collisions and drawing together.
class Game
{
public:
    Game();

    void run();

private:
    void load_background();
    void load_audio();

    void handle_melee_attack();
    void handle_ranged_attack();
    void update_bullets();
    void update_enemies();
    void resolve_combat();
    void render();
    void show_game_over();

    static constexpr int BULLET_DAMAGE = 15;

    Player _player;
    std::vector<std::unique_ptr<Enemy>> _enemies;
    std::vector<Bullet> _bullets;

    bitmap _background;
    double _bg_scale, _bg_offset_x, _bg_offset_y;
    music _bgm;
};

#endif

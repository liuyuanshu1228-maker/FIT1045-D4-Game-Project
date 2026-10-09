#include "Player.hpp"
#include "splashkit.h"
#include <string>

namespace
{
    constexpr int SCREEN_WIDTH = 800;
    constexpr int SCREEN_HEIGHT = 600;
}

Player::Player()
    : Character(0, 0, 40, 40, 2.5, 25), _hearts(3), _attacking(false), _dying(false)
{
    _walk_anim.load("player_walk", "Resources/sprites/Soldier/Soldier_Walk.png", 100, 100, 8, 8);
    _attack_anim.load("player_attack", "Resources/sprites/Soldier/Soldier_Attack.png", 100, 100, 6, 6);
    _death_anim.load("player_death", "Resources/sprites/Soldier/Soldier_Death.png", 100, 100, 4, 15);
}

void Player::set_start_position(double x, double y)
{
    _x = x;
    _y = y;
}

void Player::handle_input()
{
    if (_dying)
        return;

    bool was_moving = false;

    if (key_down(UP_KEY) || key_down(W_KEY))
    {
        _y -= _speed;
        was_moving = true;
    }
    if (key_down(DOWN_KEY) || key_down(S_KEY))
    {
        _y += _speed;
        was_moving = true;
    }
    if (key_down(LEFT_KEY) || key_down(A_KEY))
    {
        _x -= _speed;
        was_moving = true;
    }
    if (key_down(RIGHT_KEY) || key_down(D_KEY))
    {
        _x += _speed;
        was_moving = true;
    }

    if (_x < 0)
        _x = 0;
    if (_x > SCREEN_WIDTH - _width)
        _x = SCREEN_WIDTH - _width;
    if (_y < 0)
        _y = 0;
    if (_y > SCREEN_HEIGHT - _height)
        _y = SCREEN_HEIGHT - _height;

    // Start a sword swing on space key press
    if (key_typed(SPACE_KEY) && !_attacking)
    {
        _attacking = true;
        _attack_anim.reset();
    }

    if (_attacking)
    {
        // Attack animation freezes the walk cycle while it plays out
        if (_attack_anim.step_once_reset())
        {
            _attacking = false;
        }
    }
    else if (was_moving)
    {
        _walk_anim.step_loop();
    }
    else
    {
        _walk_anim.reset();
    }
}

void Player::update_health()
{
    if (_hearts <= 0 && !_dying)
    {
        _dying = true;
        _death_anim.reset();
        write_line("Player is dying!");
    }

    if (_dying)
    {
        _death_anim.step_once_hold();
    }
}

void Player::draw() const
{
    if (_dying && _death_anim.is_loaded())
    {
        _death_anim.draw(_x, _y);
        return;
    }

    if (_attacking && _attack_anim.is_loaded())
    {
        _attack_anim.draw(_x, _y);
        return;
    }

    if (_walk_anim.is_loaded())
    {
        _walk_anim.draw(_x, _y);
    }
    else
    {
        fill_rectangle(COLOR_BLUE, _x, _y, _width, _height);
    }
}

void Player::lose_heart()
{
    if (_hearts > 0)
    {
        _hearts--;
        write_line("Player lost a heart! Hearts remaining: " + std::to_string(_hearts));
    }
}

void Player::reset_hearts()
{
    _hearts = 3;
}

bool Player::is_dying_animation_done() const
{
    return _death_anim.current_frame() >= _death_anim.frame_count() - 1;
}

rectangle Player::get_melee_range() const
{
    return rectangle_from(_x - MELEE_RANGE / 2, _y - MELEE_RANGE / 2,
                           _width + MELEE_RANGE, _height + MELEE_RANGE);
}

#include "Bullet.hpp"
#include <cmath>

Bullet::Bullet() : Entity(0, 0, WIDTH, HEIGHT), _dx(0), _dy(0)
{
    _active = false; // waits to be fired
}

void Bullet::fire(double start_x, double start_y, double target_x, double target_y)
{
    // Spawn from the center of a 40x40 player sprite
    _x = start_x + (40 - _width) / 2.0;
    _y = start_y + (40 - _height) / 2.0;
    _active = true;

    double dx = target_x - _x;
    double dy = target_y - _y;
    double distance = sqrt(dx * dx + dy * dy);

    if (distance != 0)
    {
        _dx = dx / distance;
        _dy = dy / distance;
    }
    else
    {
        _dx = 0;
        _dy = -1;
    }
}

void Bullet::update()
{
    if (!_active)
        return;

    _x += SPEED * _dx;
    _y += SPEED * _dy;

    if (_y < 0 || _x < 0 || _x > 800 || _y > 600)
    {
        _active = false;
    }
}

void Bullet::draw() const
{
    if (_active)
    {
        fill_rectangle(COLOR_BLUE, _x, _y, _width, _height);
    }
}

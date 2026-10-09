#include "Character.hpp"
#include "Screen.hpp"

Character::Character(double x, double y, int width, int height, double speed)
    : Entity(x, y, width, height), _speed(speed)
{
}

void Character::clamp_to_screen()
{
    if (_x < 0)
        _x = 0;
    if (_x > Screen::WIDTH - _width)
        _x = Screen::WIDTH - _width;
    if (_y < 0)
        _y = 0;
    if (_y > Screen::HEIGHT - _height)
        _y = Screen::HEIGHT - _height;
}

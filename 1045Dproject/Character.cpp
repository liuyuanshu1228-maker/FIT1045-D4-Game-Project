#include "Character.hpp"

Character::Character(double x, double y, int width, int height, double speed, int attack)
    : Entity(x, y, width, height), _speed(speed), _attack(attack)
{
}

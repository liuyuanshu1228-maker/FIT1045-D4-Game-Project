#include "Entity.hpp"

Entity::Entity(double x, double y, int width, int height)
    : _x(x), _y(y), _width(width), _height(height), _active(true)
{
}

rectangle Entity::get_bounds() const
{
    return rectangle_from(_x, _y, _width, _height);
}

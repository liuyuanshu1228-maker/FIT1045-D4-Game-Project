#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "splashkit.h"

// Common ground for anything that occupies space in the game world and can be drawn.
class Entity
{
public:
    Entity(double x, double y, int width, int height);
    virtual ~Entity() = default;

    virtual void draw() const = 0;

    bool is_active() const { return _active; }
    void deactivate() { _active = false; }

    double get_x() const { return _x; }
    double get_y() const { return _y; }
    double get_width() const { return _width; }
    double get_height() const { return _height; }

    rectangle get_bounds() const;

protected:
    double _x, _y;
    int _width, _height;
    bool _active;
};

#endif

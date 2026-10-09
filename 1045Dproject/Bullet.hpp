#ifndef BULLET_HPP
#define BULLET_HPP

#include "Entity.hpp"

class Bullet : public Entity
{
public:
    Bullet();

    // Fires from the center of a (start_width x start_height) shooter at
    // (start_x, start_y) towards (target_x, target_y).
    void fire(double start_x, double start_y, double start_width, double start_height,
              double target_x, double target_y);
    void update();
    void draw() const override;

private:
    static constexpr int WIDTH = 5;
    static constexpr int HEIGHT = 10;
    static constexpr double SPEED = 6.0;

    double _dx, _dy;
};

#endif

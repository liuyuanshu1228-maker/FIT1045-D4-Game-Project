#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Entity.hpp"

// Shared ground for any combatant: movement speed and attack strength.
// Subclasses decide what "alive" means for them (hearts vs. a health pool).
class Character : public Entity
{
public:
    Character(double x, double y, int width, int height, double speed, int attack);

    virtual bool is_alive() const = 0;

    double get_speed() const { return _speed; }
    int get_attack() const { return _attack; }

protected:
    // Keeps the character's bounding box fully inside the window.
    // Shared by Player and Enemy, which both move under their own logic
    // but need the same "don't walk off screen" rule.
    void clamp_to_screen();

    double _speed;
    int _attack;
};

#endif

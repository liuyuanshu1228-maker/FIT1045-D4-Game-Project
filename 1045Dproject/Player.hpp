#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "SpriteAnimator.hpp"
#include "Health.hpp"

class Player : public Character
{
public:
    Player();

    void set_start_position(double x, double y);

    void handle_input();  // movement + melee attack input
    void update_health(); // death-state bookkeeping
    void draw() const override;
    void draw_hud() const; // heart icons showing remaining lives

    bool is_alive() const override { return !_hearts.is_depleted(); }
    int get_hearts() const { return _hearts.current(); }
    void lose_heart();
    void reset_hearts();
    bool is_dead() const { return _hearts.is_depleted(); }

    bool is_attacking() const { return _attacking; }
    bool is_dying_animation_done() const;

    rectangle get_melee_range() const;

private:
    Health _hearts;
    bool _attacking;
    bool _dying;

    SpriteAnimator _walk_anim;
    SpriteAnimator _attack_anim;
    SpriteAnimator _death_anim;

    static constexpr double MELEE_RANGE = 60.0;
};

#endif

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "SpriteAnimator.hpp"

class Player : public Character
{
public:
    Player();

    void set_start_position(double x, double y);

    void handle_input();  // movement + melee attack input
    void update_health(); // death-state bookkeeping
    void draw() const override;

    bool is_alive() const override { return _hearts > 0; }
    int get_hearts() const { return _hearts; }
    void lose_heart();
    void reset_hearts();
    bool is_dead() const { return _hearts <= 0; }

    bool is_attacking() const { return _attacking; }
    bool is_dying_animation_done() const;

    rectangle get_melee_range() const;

private:
    int _hearts;
    bool _attacking;
    bool _dying;

    SpriteAnimator _walk_anim;
    SpriteAnimator _attack_anim;
    SpriteAnimator _death_anim;

    static constexpr double MELEE_RANGE = 60.0;
};

#endif

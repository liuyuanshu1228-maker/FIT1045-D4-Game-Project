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

    void handle_input();     // movement + melee attack input
    void update_cooldowns(); // ticks attack/invulnerability timers; call once per frame
    void update_health();    // death-state bookkeeping
    void draw() const override;
    void draw_hud() const; // heart icons showing remaining lives

    bool is_alive() const override { return !_hearts.is_depleted(); }
    int get_hearts() const { return _hearts.current(); }
    void lose_heart();
    void reset_hearts();
    bool is_dead() const { return _hearts.is_depleted(); }
    int get_attack() const { return _attack; }

    bool is_attacking() const { return _attacking; }
    bool is_dying_animation_done() const;

    rectangle get_melee_range() const;

    // Sword: only one hit may be registered per swing.
    bool can_register_melee_hit() const { return _attacking && _melee_hit_cooldown == 0; }
    void register_melee_hit() { _melee_hit_cooldown = MELEE_HIT_COOLDOWN; }

    // Ranged weapon: caps fire rate.
    bool can_fire() const { return _ranged_cooldown == 0; }
    void register_shot() { _ranged_cooldown = RANGED_COOLDOWN; }

    // Contact damage from touching an enemy, gated by a brief invulnerability window.
    bool try_take_contact_damage();

private:
    int _attack;
    Health _hearts;
    bool _attacking;
    bool _dying;

    SpriteAnimator _walk_anim;
    SpriteAnimator _attack_anim;
    SpriteAnimator _death_anim;

    int _melee_hit_cooldown;
    int _ranged_cooldown;
    int _hit_cooldown;

    static constexpr double MELEE_RANGE = 60.0;
    static constexpr int MELEE_HIT_COOLDOWN = 30;
    static constexpr int RANGED_COOLDOWN = 15;
    static constexpr int HIT_COOLDOWN = 60;
};

#endif

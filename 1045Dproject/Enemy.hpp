#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"
#include "SpriteAnimator.hpp"
#include <vector>
#include <memory>

class Enemy : public Character
{
public:
    Enemy();

    void set_start_position(double x, double y);

    // Chases/attacks the player when nearby, otherwise wanders autonomously.
    void update(double target_x, double target_y);
    void draw() const override;

    bool is_alive() const override { return _active; }
    void take_damage(int damage);
    int get_blood() const { return _blood; }
    int get_max_blood() const { return _blood_max; }

    bool is_attacking() const { return _attacking; }
    // Returns true exactly once per swing that landed a hit.
    bool consume_damage_flag();

    double distance_to(double px, double py) const;

    // Spawns a wave of enemies at valid random positions away from the player and each other.
    static void spawn_wave(std::vector<std::unique_ptr<Enemy>> &enemies, double player_x, double player_y);

private:
    bool update_wander_ai();
    void clamp_to_screen();

    int _blood_max;
    int _blood;
    double _detect_radius;

    SpriteAnimator _walk_anim;
    SpriteAnimator _attack_anim;
    bool _attacking;

    double _wander_target_x, _wander_target_y;
    int _wander_timer;
    int _wander_cooldown;
    double _wander_speed_modifier;
    int _rest_duration;
    int _rest_timer;
    bool _resting;
    double _laziness;

    double _melee_range;
    bool _damage_pending;
    bool _has_hit_this_swing;
    int _melee_cooldown_after;
    int _melee_cooldown_timer;
};

#endif

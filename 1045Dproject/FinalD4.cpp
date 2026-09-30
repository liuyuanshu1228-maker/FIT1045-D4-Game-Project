// FinalD4.cpp - Complete Game Implementation
// All components consolidated: Player, Enemy, Bullet, Logic, Main
#include "splashkit.h"
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>

using std::string;

// ============================================================================
// DYNAMIC ARRAY IMPLEMENTATION
// ============================================================================
#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP
#include <cstdlib>

template <typename T>
class dynamic_array
{
public:
    T *data;
    int size;
    int capacity;

    dynamic_array(int initial_capacity, const T &default_value)
    {
        capacity = (initial_capacity > 0) ? initial_capacity : 1;
        data = static_cast<T *>(malloc(capacity * sizeof(T)));
        size = 0;

        // Initialize with default values if needed
        for (int i = 0; i < initial_capacity; i++)
        {
            new (data + i) T(default_value);
            size++;
        }
    }

    ~dynamic_array()
    {
        // Call destructors for all elements
        for (int i = 0; i < size; i++)
        {
            data[i].~T();
        }
        free(data);
    }

    void add(const T &item)
    {
        if (size >= capacity)
        {
            resize(capacity * 2);
        }
        new (data + size) T(item);
        size++;
    }

    void remove(int index)
    {
        if (index < 0 || index >= size)
            return;

        // Call destructor for the element being removed
        data[index].~T();

        // Shift elements left
        for (int i = index; i < size - 1; i++)
        {
            new (data + i) T(data[i + 1]);
            data[i + 1].~T();
        }
        size--;
    }

private:
    void resize(int new_capacity)
    {
        T *new_data = static_cast<T *>(malloc(new_capacity * sizeof(T)));

        // Move existing elements
        for (int i = 0; i < size; i++)
        {
            new (new_data + i) T(data[i]);
            data[i].~T();
        }

        free(data);
        data = new_data;
        capacity = new_capacity;
    }
};
#endif

// ============================================================================
// BULLET CLASS
// ============================================================================
#ifndef BULLET_HPP
#define BULLET_HPP

class bullet
{
private:
    double _speed;
    double _x;
    double _y; // bullet position current
    double _dx, _dy;
    int _bullet_width;
    int _bullet_height; // bullet size
    bool _active;
    color _bullet_color;

public:
    bullet(); // constructor
    void bullet_fire(double x_startpoint, double y_startpoint, double target_x, double target_y);
    // fire towards target
    void bullet_movement();                    // track
    void draw() const;                         // procedure prepare for drawing the bullet
    double get_xbullet() const { return _x; }  // get bullet x position, after the bullet is fired
    double get_ybullet() const { return _y; }  // as well
    bool is_active() const { return _active; } // check if the bullet is active
    int get_b_width() const { return _bullet_width; }
    int get_b_height() const { return _bullet_height; }
    void deactivate() { _active = false; }
};

#endif

// ============================================================================
// ENEMY CLASS DECLARATION
// ============================================================================
#ifndef ENEMY_HPP
#define ENEMY_HPP

// Forward declaration
class enermy;

// Function declaration for spawning enemies (outside the class)
void random_enemies(dynamic_array<enermy> &, double, double);

// blueprint of the enemy
class enermy
{
private:
    int _attack;
    double _speed;

    int _enemysize_X; // enemy size
    int _enemysize_Y; // enemy size
    double _x;        // enemy position current
    double _y;        // enemy position current
    int _screen_width;
    int _screen_height;
    color _enemy_color; // enemy color
    bool _active;

    double _radius_detect; // enermy detect the player within this range

    int _blood_max;
    int _blood;

    // 动画相关变量（手动逐帧播放系统）
    bitmap _enemy_sheet;  // 整张敌人动画图片（Orc-Walk.png）
    int _current_frame;   // 当前显示的帧
    int _animation_timer; // 控制动画节奏的计时器
    int _frame_duration;  // 每隔多少帧切换一次
    int _frame_width;     // 每帧宽度（100）
    int _frame_height;    // 每帧高度（100）
    int _frame_count;     // 动画总帧数（8）

    // Wandering AI variables
    double _wander_targeted_x;  // 当前随机目标点 X
    double _wander_targeted_y;  // 当前随机目标点 Y
    int _wander_time_counter;   // 计时器，控制多久重新选择目标
    int _wander_cooldown_space; // 每隔多少帧换一次目标

    // Individual personality variables for varied behavior
    double _wander_speed_modifier; // Individual speed multiplier (0.3-0.8)
    int _rest_duration;            // How long to stay idle (0-180 frames)
    int _rest_timer_counter;       // Current rest countdown
    bool _enemy_is_resting;        // Currently resting/idle
    double _personality_laziness;  // 0.0-1.0, affects rest frequency

    // attack animation for enermy
    bitmap _attack_the_frame;         // 攻击动画图片
    int _attack_frame_width_enermy;   // 每帧宽度
    int _attack_frame_height_enermy;  // 每帧高度
    int _attack_frame_count_enermy;   // 总帧数
    int _attack_current_frame_enermy; // 当前播放帧
    int _attack_time_counting_enermy; // 每帧计时
    int _attack_frame_lasting_enermy; // 每帧持续时间
    bool _if_is_attacking;            // 是否正在攻击

    // 攻击命中与冷却
    double _attack_reach_range;   // 近战触及范围（像素，命中盒用）
    int _attack_hurt;             // 每次命中扣多少心（一般 1）
    bool _check_if_hurt;          // 本次挥击是否已造成过伤害（防多扣）
    bool _has_damaged_this_swing; // 本次攻击是否已造成伤害
    int _attack_cooldown_after;   // 攻击后冷却（帧数）
    int _attack_cooldown_counter; // 冷却计时器
    int _attack_cooldown_timer;   // 当前冷却计时器
    int _attack_frame_duration;   // 攻击动画每帧持续时间

public:                                                           // the system needs to control the enemy
    enermy();                                                     // constructor
    void enermy_startdir(float x_startpoint, float y_startpoint); // enemy initial position
    void enermy_movement(double target_x, double target_y);
    void enermy_attack();
    void enermy_health();
    void draw();                  // draw enermy
    bool be_active() const;       // check if enermy is active
    void kill_enermy();           // death of enermy
    double get_x() const;         // get enemy x position
    double get_y() const;         // get enemy y position
    double get_width() const;     // get enemy width
    double get_height() const;    // get enemy height
    void take_damage(int damage); // enemy takes damage
    int get_health() const;       // get current health
    int get_max_health() const;   // get maximum health
    bool is_attacking() const;    // check if enemy is attacking
    bool should_deal_damage();    // check if enemy should deal damage this frame

    // Wandering AI methods
    void enermy_wander();                                  // autonomous wandering behavior
    double distance_to_player(double px, double py) const; // calculate distance to player
};
#endif

// ============================================================================
// PLAYER CLASS DECLARATION
// ============================================================================
#ifndef PLAYER_HPP
#define PLAYER_HPP

// blueprint of the player
class player
{
private:
    int _attack;
    double _speed;
    int _heart;
    int _playersize_X; // player size
    int _playersize_Y; // player size
    double _x;         // player position current
    double _y;         // player position current
    int _screen_width;
    int _screen_height;
    color _player_color; // player color
    bool _alive;         // player alive status
    sprite _sprite;      // player sprite for animation

    // Manual animation variables
    bitmap _sprite_sheet; // The sprite sheet bitmap
    int _current_frame;   // Current animation frame (0-7)
    int _animation_timer; // Timer for frame switching
    int _frame_duration;  // How many game frames per animation frame
    int _total_frames;    // Total number of animation frames
    int _frame_width;     // Width of each frame
    int _frame_height;    // Height of each frame

    // Attack animation variables
    bitmap _attack_sheet;       // Attack sprite sheet
    int _attack_current_frame;  // Current attack frame
    int _attack_timer;          // Attack animation timer
    int _attack_frame_duration; // Duration per attack frame
    int _attack_frame_count;    // Total attack frames
    int _attack_frame_width;    // Attack frame width
    int _attack_frame_height;   // Attack frame height
    bool _attacking;            // Is currently attacking

    // Death animation
    bitmap _death_sheet;           // 死亡动画图片
    int _death_frame_width_enemy;  // 每帧宽度
    int _death_frame_height_enemy; // 每帧高度
    int _death_frame_count_enemy;  // 总帧数
    int _death_now_frame;          // 当前播放帧
    int _death_time_counter;       // 每帧计时
    int _death_frame_lating_time;  // 每帧持续时间
    bool _dying_player;            // 正在死亡动画中
    bool _dying;                   // Death state flag

public:
    player();                                                     // constructor
    void player_startdir(float x_startpoint, float y_startpoint); // player initial position
    void player_movement();
    void player_attack();
    void player_health();
    void draw();               // draw player
    double get_x() const;      // get player x position
    double get_y() const;      // get player y position
    double get_width() const;  // get player width
    double get_height() const; // get player height
    bool is_alive() const;     // check if player is alive
    void kill_player();        // kill the player
    int get_heart() const;     // get current hearts
    void lose_heart();         // lose one heart
    void reset_heart();        // reset hearts to 3
    bool is_dead() const;      // check if player is dead

    // calculate the rectangle area of player's attack range
    rectangle get_attack_range() const;
    bool is_attacking() const { return _attacking; } // let outside know if attacking

    rectangle get_the_attack_range_for_mouse(double mx, double my) const;

    bool is_dying_animation_done() const { return _death_now_frame >= _death_frame_count_enemy - 1; }
};
#endif

// ============================================================================
// LOGIC FUNCTIONS DECLARATION
// ============================================================================
#ifndef LOGIC_HPP
#define LOGIC_HPP

// check collision between the bullet and the enemy
bool collision_check(bullet &Bullet, enermy &e);

// check collision between player and enemy
bool player_hit_enemy(const player &pp, const enermy &ee);

// this enermy can be used in anywhere not just in logic.cpp
extern dynamic_array<enermy> enemies;
// spawn multiple enemies at random positions
void random_enemies(dynamic_array<enermy> &enemies, double player_x, double player_y);

#endif

// ============================================================================
// BULLET CLASS IMPLEMENTATION
// ============================================================================

bullet::bullet()
{
    _speed = 6.0;
    _bullet_width = 5;   // make bullet bigger and more visible
    _bullet_height = 10; // make bullet bigger and more visible
    _x = 0;              // bullet position current
    _y = 0;              // bullet position current， these will soon be changed by
    _bullet_color = COLOR_BLACK;
    _active = false; // unless push space key
}

void bullet::bullet_fire(double x_startpoint, double y_startpoint, double target_x, double target_y)
{                                                    // 让子弹从玩家中心射出
    _x = x_startpoint + (40 - _bullet_width) / 2.0;  // Assume player width is 40
    _y = y_startpoint + (40 - _bullet_height) / 2.0; // Assume player height is 40

    _active = true;

    // Calculate direction vector towards target
    double dx = target_x - _x;
    double dy = target_y - _y;
    double distance = sqrt(dx * dx + dy * dy);

    if (distance != 0)
    {
        // Normalize direction vector
        _dx = dx / distance;
        _dy = dy / distance;
    }
    else
    {
        // Default direction if target is at bullet position
        _dx = 0;
        _dy = -1; // move straight up
    }
}

void bullet::bullet_movement()
{
    if (_active)
    {
        // Move in the calculated direction
        _x += _speed * _dx;
        _y += _speed * _dy;

        // Deactivate if bullet goes off screen
        if (_y < 0 || _x < 0 || _x > 800 || _y > 600)
        {
            _active = false;
        }
    }
}

void bullet::draw() const
{
    if (_active)
    {
        fill_rectangle(COLOR_BLUE, _x, _y, _bullet_width, _bullet_height); // bright blue bullet for visibility
    }
}

// ============================================================================
// ENEMY CLASS IMPLEMENTATION
// ============================================================================

// constructor
enermy::enermy()
{
    _attack = 1;
    _speed = 1;
    _enemysize_X = 50;
    _enemysize_Y = 50;
    _x = 0;
    _y = 0;
    _screen_width = 800;
    _screen_height = 600;
    _blood_max = 100;
    _blood = _blood_max;
    _enemy_color = COLOR_BLACK;
    _active = true;
    _radius_detect = 200.0;

    _wander_targeted_x = _x;
    _wander_targeted_y = _y;
    _wander_time_counter = 0;
    _wander_cooldown_space = 120;

    // attack animation for enemy initialization
    _attack_the_frame = load_bitmap("enemy_attack", "Resources/sprites/Enemy/Orc_Attack02.png");
    if (_attack_the_frame != nullptr)
    {
        write_line("Enemy attack sprite loaded successfully!");
    }
    else
    {
        write_line("ERROR: Failed to load enemy attack sprite!");
    }
    _attack_frame_width_enermy = 100;
    _attack_frame_height_enermy = 100;
    _attack_frame_count_enermy = 8;
    _attack_current_frame_enermy = 0;
    _attack_time_counting_enermy = 0;
    _attack_frame_lasting_enermy = 10;
    _if_is_attacking = false;

    // Initialize attack system variables
    _attack_reach_range = 50.0;
    _attack_hurt = 1;
    _check_if_hurt = false;
    _has_damaged_this_swing = false;
    _attack_cooldown_after = 60;
    _attack_cooldown_counter = 0;
    _attack_cooldown_timer = 0;
    _attack_frame_duration = 10;

    // Initialize individual personality traits for varied behavior
    _wander_speed_modifier = 0.3 + (rand() % 50) / 100.0;
    _personality_laziness = (rand() % 80) / 100.0;
    _rest_duration = 0;
    _rest_timer_counter = 0;
    _enemy_is_resting = false;

    // Randomize wander cooldown space for each enemy (1-4 seconds)
    _wander_cooldown_space = 60 + (rand() % 180);

    // Load enemy sprite sheet with correct path
    write_line("Loading enemy sprite sheet...");
    _enemy_sheet = load_bitmap("enemy_sheet", "Resources/sprites/Enemy/Orc-Walk.png");

    if (_enemy_sheet != nullptr)
    {
        write_line("Enemy sprite sheet loaded successfully!");
    }
    else
    {
        write_line("Failed to load enemy sprite sheet");
    }

    _frame_width = 100;
    _frame_height = 100;
    _frame_count = 8;
    _current_frame = 0;
    _animation_timer = 0;
    _frame_duration = 10;
}

void enermy::enermy_startdir(float x_startpoint, float y_startpoint)
{
    _x = x_startpoint;
    _y = y_startpoint;
}

void enermy::enermy_movement(double target_x, double target_y)
{
    bool was_moving = false;

    // calculate vector from enemy to target (player)
    double dx = target_x - _x;
    double dy = target_y - _y;

    // calculate distance using pythagorean theorem
    double distance = sqrt(dx * dx + dy * dy);

    // Update attack cooldown
    if (_attack_cooldown_timer > 0)
        _attack_cooldown_timer--;

    if (distance < _radius_detect && distance > 0)
    {
        // Player detected - check if close enough to attack
        if (distance <= 50 && _attack_cooldown_timer == 0 && !_if_is_attacking)
        {
            // Start attack
            write_line("Enemy starting attack! Distance: " + std::to_string(distance));
            _if_is_attacking = true;
            _attack_current_frame_enermy = 0;
            _attack_time_counting_enermy = 0;
            _has_damaged_this_swing = false;
        }
        else if (!_if_is_attacking)
        {
            // Chase player
            _x += (_speed * dx / distance);
            _y += (_speed * dy / distance);
            was_moving = true;
        }

        // Update attack animation if attacking
        if (_if_is_attacking)
        {
            _attack_time_counting_enermy++;
            if (_attack_time_counting_enermy >= _attack_frame_duration)
            {
                _attack_time_counting_enermy = 0;
                _attack_current_frame_enermy++;

                // Hit frame - set damage flag
                if (_attack_current_frame_enermy == 3 && !_has_damaged_this_swing)
                {
                    write_line("Enemy attack hit frame reached!");
                    _has_damaged_this_swing = true;
                    _check_if_hurt = true;
                }

                // End of attack animation
                if (_attack_current_frame_enermy >= _attack_frame_count_enermy)
                {
                    _attack_current_frame_enermy = 0;
                    _if_is_attacking = false;
                    _has_damaged_this_swing = false;
                    _attack_cooldown_timer = _attack_cooldown_after;
                }
            }
        }
    }
    else // Player out of detection range - wander behavior
    {
        // Handle resting behavior
        if (_enemy_is_resting)
        {
            _rest_timer_counter--;
            if (_rest_timer_counter <= 0)
            {
                _enemy_is_resting = false;
            }
        }
        else
        {
            _wander_time_counter++;

            // Choose new wander target
            if (_wander_time_counter >= _wander_cooldown_space)
            {
                _wander_time_counter = 0;

                // Random chance to rest
                if ((rand() % 100) / 100.0 < _personality_laziness)
                {
                    _enemy_is_resting = true;
                    _rest_duration = 30 + (rand() % 150);
                    _rest_timer_counter = _rest_duration;
                }
                else
                {
                    // Choose new wander target
                    _wander_targeted_x = _x + (rand() % 200 - 100);
                    _wander_targeted_y = _y + (rand() % 200 - 100);

                    // Keep within screen bounds
                    if (_wander_targeted_x < 0)
                        _wander_targeted_x = 0;
                    if (_wander_targeted_x > _screen_width - _enemysize_X)
                        _wander_targeted_x = _screen_width - _enemysize_X;
                    if (_wander_targeted_y < 0)
                        _wander_targeted_y = 0;
                    if (_wander_targeted_y > _screen_height - _enemysize_Y)
                        _wander_targeted_y = _screen_height - _enemysize_Y;
                }
            }

            // Move towards wander target if not resting
            if (!_enemy_is_resting)
            {
                double wx = _wander_targeted_x - _x;
                double wy = _wander_targeted_y - _y;
                double wander_distance = sqrt(wx * wx + wy * wy);

                if (wander_distance > 5.0)
                {
                    double move_speed = _speed * _wander_speed_modifier;
                    _x += (move_speed * wx / wander_distance);
                    _y += (move_speed * wy / wander_distance);
                    was_moving = true;
                }
            }
        }
    }

    // Keep enemy within screen bounds
    if (_x < 0)
        _x = 0;
    if (_x > _screen_width - _enemysize_X)
        _x = _screen_width - _enemysize_X;
    if (_y < 0)
        _y = 0;
    if (_y > _screen_height - _enemysize_Y)
        _y = _screen_height - _enemysize_Y;

    // Update walking animation if moving
    if (was_moving && _enemy_sheet != nullptr)
    {
        _animation_timer++;
        if (_animation_timer >= _frame_duration)
        {
            _animation_timer = 0;
            _current_frame++;
            if (_current_frame >= _frame_count)
            {
                _current_frame = 0;
            }
        }
    }
    else if (!was_moving)
    {
        _current_frame = 0;
        _animation_timer = 0;
    }
}

void enermy::draw()
{
    if (!_active)
        return;

    // attacking animation of enemy
    if (_if_is_attacking && _attack_the_frame != nullptr)
    {
        rectangle src_rect = rectangle_from(
            _attack_current_frame_enermy * _attack_frame_width_enermy,
            0,
            _attack_frame_width_enermy,
            _attack_frame_height_enermy);
        draw_bitmap(_attack_the_frame, _x, _y, option_part_bmp(src_rect));

        // 绘制血条
        double bar_width = _enemysize_X * 0.3;
        double bar_height = 4;
        double health_ratio = static_cast<double>(_blood) / _blood_max;
        double current_bar_width = bar_width * health_ratio;
        double bar_x = _x + (_enemysize_X - bar_width) / 2;
        double bar_y = _y + 5;
        fill_rectangle(COLOR_RED, bar_x, bar_y, bar_width, bar_height);
        fill_rectangle(COLOR_GREEN, bar_x, bar_y, current_bar_width, bar_height);

        return; // 正在攻击时，不绘制行走动画
    }

    // Draw the animated enemy sprite
    if (_enemy_sheet != nullptr)
    {
        // Calculate source rectangle for current frame
        rectangle src_rect = rectangle_from(_current_frame * _frame_width, 0, _frame_width, _frame_height);
        draw_bitmap(_enemy_sheet, _x, _y, option_part_bmp(src_rect));
    }
    else
    {
        // Fallback to rectangle if sprite failed to load
        fill_rectangle(_enemy_color, _x, _y, _enemysize_X, _enemysize_Y);
    }

    // ===== draw health bar above head =====
    double bar_width = _enemysize_X * 0.3; // Make health bar 30% of enemy width
    double bar_height = 2;
    double health_ratio = static_cast<double>(_blood) / _blood_max;
    double current_bar_width = bar_width * health_ratio;

    double bar_x = _x + (_enemysize_X - bar_width) / 2;
    double bar_y = _y - 8; // Position above enemy

    fill_rectangle(COLOR_RED, bar_x, bar_y, bar_width, bar_height);           // Background (red for missing health)
    fill_rectangle(COLOR_GREEN, bar_x, bar_y, current_bar_width, bar_height); // Foreground (green for current health)
}

// Random enemy spawning function
void random_enemies(dynamic_array<enermy> &enemies, double player_x, double player_y)
{
    // Reset the array by setting size to 0 (manual clear)
    enemies.size = 0;

    int num_enemies = 4 + rand() % 3;                 // spawn 4 to 6 enemies each time
    const double MIN_DISTANCE_FROM_PLAYER = 100.0;    // minimum distance from player
    const double MIN_DISTANCE_BETWEEN_ENEMIES = 80.0; // minimum distance between enemies
    const int MAX_ATTEMPTS = 50;                      // prevent infinite loops

    for (int i = 0; i < num_enemies; i++)
    {
        bool valid_position = false;
        double start_x, start_y;
        int attempts = 0;

        // Keep trying until we find a valid position
        while (!valid_position && attempts < MAX_ATTEMPTS)
        {
            // Generate random position within screen bounds (with margins)
            start_x = 50 + rand() % 700; // 50 to 750 (screen width 800)
            start_y = 50 + rand() % 500; // 50 to 550 (screen height 600)

            // Check distance from player
            double dx = start_x - player_x;
            double dy = start_y - player_y;
            double dist_from_player = sqrt(dx * dx + dy * dy);

            if (dist_from_player < MIN_DISTANCE_FROM_PLAYER)
            {
                attempts++;
                continue; // Too close to player, try again
            }

            // Check distance from other enemies
            bool too_close_to_other = false;
            for (int j = 0; j < enemies.size; j++)
            {
                double ex = enemies.data[j].get_x();
                double ey = enemies.data[j].get_y();
                double enemy_dx = start_x - ex;
                double enemy_dy = start_y - ey;
                double dist_from_enemy = sqrt(enemy_dx * enemy_dx + enemy_dy * enemy_dy);

                if (dist_from_enemy < MIN_DISTANCE_BETWEEN_ENEMIES)
                {
                    too_close_to_other = true;
                    break;
                }
            }

            if (!too_close_to_other)
            {
                valid_position = true; // Found a good position
            }

            attempts++;
        }

        // Create enemy at the valid position (or fallback position if max attempts reached)
        enermy new_enemy;
        new_enemy.enermy_startdir(start_x, start_y);
        enemies.add(new_enemy);
    }
}

// Getter and setter methods
bool enermy::be_active() const
{
    return _active;
}

void enermy::kill_enermy()
{
    _active = false;
}

double enermy::get_x() const
{
    return _x;
}

double enermy::get_y() const
{
    return _y;
}

double enermy::get_width() const
{
    return _enemysize_X;
}

double enermy::get_height() const
{
    return _enemysize_Y;
}

void enermy::take_damage(int damage)
{
    _blood -= damage;
    if (_blood <= 0)
    {
        _blood = 0;
        kill_enermy(); // Deactivate enemy when health reaches 0
    }
}

int enermy::get_health() const
{
    return _blood;
}

int enermy::get_max_health() const
{
    return _blood_max;
}

bool enermy::is_attacking() const
{
    return _if_is_attacking;
}

bool enermy::should_deal_damage()
{
    if (_check_if_hurt)
    {
        _check_if_hurt = false; // Reset the flag so damage is only dealt once
        return true;
    }
    return false;
}

double enermy::distance_to_player(double px, double py) const
{
    double dx = px - _x;
    double dy = py - _y;
    return sqrt(dx * dx + dy * dy);
}

void enermy::enermy_wander()
{
    // This method is now integrated into enermy_movement
}

void enermy::enermy_attack()
{
    // Attack logic integrated into movement
}

void enermy::enermy_health()
{
    // Health logic integrated into take_damage
}

// ============================================================================
// PLAYER CLASS IMPLEMENTATION
// ============================================================================

player::player()
{
    _attack = 25; // player attack strength
    _speed = 2.5;
    _heart = 3; // initial 3 hearts
    _playersize_X = 40;
    _playersize_Y = 40;
    _x = 0;
    _y = 0;
    _screen_width = 800;
    _screen_height = 600;
    _player_color = COLOR_BLUE;
    _alive = true;

    // Initialize manual animation
    _sprite_sheet = load_bitmap("player_walk", "Resources/sprites/Soldier/Soldier_Walk.png");
    if (_sprite_sheet == nullptr)
    {
        write_line("ERROR: Failed to load player walking sprite sheet!");
    }
    else
    {
        write_line("Player walking sprite sheet loaded successfully!");
    }

    _current_frame = 0;
    _animation_timer = 0;
    _frame_duration = 8; // Change frame every 8 game frames (slower animation)
    _total_frames = 8;   // 8 frames in the walking animation
    _frame_width = 100;  // Each frame is 100 pixels wide
    _frame_height = 100; // Each frame is 100 pixels high

    // Initialize attack animation
    _attack_sheet = load_bitmap("player_attack", "Resources/sprites/Soldier/Soldier_Attack.png");
    if (_attack_sheet == nullptr)
    {
        write_line("ERROR: Failed to load player attack sprite sheet!");
    }
    else
    {
        write_line("Player attack sprite sheet loaded successfully!");
    }

    _attack_current_frame = 0;
    _attack_timer = 0;
    _attack_frame_duration = 6; // Faster attack animation
    _attack_frame_count = 6;    // 6 frames in attack animation
    _attack_frame_width = 100;
    _attack_frame_height = 100;
    _attacking = false;

    // Initialize death animation
    _death_sheet = load_bitmap("player_death", "Resources/sprites/Soldier/Soldier_Death.png");
    if (_death_sheet == nullptr)
    {
        write_line("ERROR: Failed to load player death sprite sheet!");
    }
    else
    {
        write_line("Player death sprite sheet loaded successfully!");
    }

    _death_frame_width_enemy = 100;
    _death_frame_height_enemy = 100;
    _death_frame_count_enemy = 4; // 4 frames for death animation
    _death_now_frame = 0;
    _death_time_counter = 0;
    _death_frame_lating_time = 15; // Slower death animation
    _dying_player = false;
    _dying = false;
}

void player::player_startdir(float x_startpoint, float y_startpoint)
{
    _x = x_startpoint;
    _y = y_startpoint;
}

void player::player_movement()
{
    if (_dying_player || _dying)
        return; // Don't move if dying

    bool was_moving = false;

    if (key_down(UP_KEY) || key_down(W_KEY))
    {
        _y -= _speed;
        was_moving = true;
    }
    if (key_down(DOWN_KEY) || key_down(S_KEY))
    {
        _y += _speed;
        was_moving = true;
    }
    if (key_down(LEFT_KEY) || key_down(A_KEY))
    {
        _x -= _speed;
        was_moving = true;
    }
    if (key_down(RIGHT_KEY) || key_down(D_KEY))
    {
        _x += _speed;
        was_moving = true;
    }

    // Keep player within screen bounds
    if (_x < 0)
        _x = 0;
    if (_x > _screen_width - _playersize_X)
        _x = _screen_width - _playersize_X;
    if (_y < 0)
        _y = 0;
    if (_y > _screen_height - _playersize_Y)
        _y = _screen_height - _playersize_Y;

    // Update walking animation only if moving and not attacking
    if (was_moving && !_attacking && _sprite_sheet != nullptr)
    {
        _animation_timer++;
        if (_animation_timer >= _frame_duration)
        {
            _animation_timer = 0;
            _current_frame++;
            if (_current_frame >= _total_frames)
            {
                _current_frame = 0;
            }
        }
    }
    else if (!was_moving && !_attacking)
    {
        // When not moving and not attacking, stay on first frame
        _current_frame = 0;
        _animation_timer = 0;
    }
}

void player::player_attack()
{
    if (_dying_player || _dying)
        return; // Don't attack if dying

    // Start attack on space key press
    if (key_typed(SPACE_KEY) && !_attacking)
    {
        _attacking = true;
        _attack_current_frame = 0;
        _attack_timer = 0;
    }

    // Update attack animation
    if (_attacking)
    {
        _attack_timer++;
        if (_attack_timer >= _attack_frame_duration)
        {
            _attack_timer = 0;
            _attack_current_frame++;

            if (_attack_current_frame >= _attack_frame_count)
            {
                _attacking = false;
                _attack_current_frame = 0;
            }
        }
    }
}

void player::player_health()
{
    if (_heart <= 0 && !_dying_player && !_dying)
    {
        _dying_player = true;
        _dying = true;
        _death_now_frame = 0;
        _death_time_counter = 0;
        write_line("Player is dying!");
    }

    // Update death animation
    if (_dying_player)
    {
        _death_time_counter++;
        if (_death_time_counter >= _death_frame_lating_time)
        {
            _death_time_counter = 0;
            _death_now_frame++;

            if (_death_now_frame >= _death_frame_count_enemy)
            {
                _death_now_frame = _death_frame_count_enemy - 1; // Stay on last frame
            }
        }
    }
}

void player::draw()
{
    // Draw death animation if dying
    if (_dying_player && _death_sheet != nullptr)
    {
        rectangle death_src = rectangle_from(
            _death_now_frame * _death_frame_width_enemy,
            0,
            _death_frame_width_enemy,
            _death_frame_height_enemy);
        draw_bitmap(_death_sheet, _x, _y, option_part_bmp(death_src));
        return;
    }

    // Draw attack animation if attacking
    if (_attacking && _attack_sheet != nullptr)
    {
        rectangle attack_src = rectangle_from(
            _attack_current_frame * _attack_frame_width,
            0,
            _attack_frame_width,
            _attack_frame_height);
        draw_bitmap(_attack_sheet, _x, _y, option_part_bmp(attack_src));
        return;
    }

    // Draw walking animation
    if (_sprite_sheet != nullptr)
    {
        rectangle src_rect = rectangle_from(
            _current_frame * _frame_width,
            0,
            _frame_width,
            _frame_height);
        draw_bitmap(_sprite_sheet, _x, _y, option_part_bmp(src_rect));
    }
    else
    {
        // Fallback to rectangle if sprite failed to load
        fill_rectangle(_player_color, _x, _y, _playersize_X, _playersize_Y);
    }
}

// Getter methods
double player::get_x() const { return _x; }
double player::get_y() const { return _y; }
double player::get_width() const { return _playersize_X; }
double player::get_height() const { return _playersize_Y; }
bool player::is_alive() const { return _alive; }
int player::get_heart() const { return _heart; }

void player::kill_player()
{
    _alive = false;
    _dying = true;
}

void player::lose_heart()
{
    if (_heart > 0)
    {
        _heart--;
        write_line("Player lost a heart! Hearts remaining: " + std::to_string(_heart));
    }
}

void player::reset_heart()
{
    _heart = 3;
}

bool player::is_dead() const
{
    return _heart <= 0;
}

rectangle player::get_attack_range() const
{
    const double ATTACK_RANGE = 60.0;
    return rectangle_from(_x - ATTACK_RANGE / 2, _y - ATTACK_RANGE / 2,
                          _playersize_X + ATTACK_RANGE, _playersize_Y + ATTACK_RANGE);
}

rectangle player::get_the_attack_range_for_mouse(double mx, double my) const
{
    const double ATTACK_RANGE = 60.0;
    double range_x = mx - ATTACK_RANGE / 2;
    double range_y = my - ATTACK_RANGE / 2;
    return rectangle_from(range_x, range_y, ATTACK_RANGE, ATTACK_RANGE);
}

// ============================================================================
// LOGIC IMPLEMENTATION
// ============================================================================

bool collision_check(bullet &Bullet, enermy &e)
{
    if (!Bullet.is_active() || !e.be_active())
        return false;

    rectangle bullet_rect = rectangle_from(Bullet.get_xbullet(), Bullet.get_ybullet(),
                                           Bullet.get_b_width(), Bullet.get_b_height());
    rectangle enemy_rect = rectangle_from(e.get_x(), e.get_y(), e.get_width(), e.get_height());

    return rectangles_intersect(bullet_rect, enemy_rect);
}

bool player_hit_enemy(const player &pp, const enermy &ee)
{
    if (!pp.is_alive() || !ee.be_active())
        return false;

    // Use smaller collision boxes for more precise collision
    const double COLLISION_RATIO = 0.25; // Use 25% of the sprite size for collision

    double player_collision_width = pp.get_width() * COLLISION_RATIO;
    double player_collision_height = pp.get_height() * COLLISION_RATIO;
    double enemy_collision_width = ee.get_width() * COLLISION_RATIO;
    double enemy_collision_height = ee.get_height() * COLLISION_RATIO;

    // Center the collision boxes
    double player_collision_x = pp.get_x() + (pp.get_width() - player_collision_width) / 2;
    double player_collision_y = pp.get_y() + (pp.get_height() - player_collision_height) / 2;
    double enemy_collision_x = ee.get_x() + (ee.get_width() - enemy_collision_width) / 2;
    double enemy_collision_y = ee.get_y() + (ee.get_height() - enemy_collision_height) / 2;

    rectangle player_rect = rectangle_from(player_collision_x, player_collision_y,
                                           player_collision_width, player_collision_height);
    rectangle enemy_rect = rectangle_from(enemy_collision_x, enemy_collision_y,
                                          enemy_collision_width, enemy_collision_height);

    return rectangles_intersect(player_rect, enemy_rect);
}

// ============================================================================
// UI FUNCTIONS
// ============================================================================

void draw_hearts(int lives)
{
    // Draw heart icons in the top-left corner
    for (int i = 0; i < lives; i++)
    {
        fill_circle(COLOR_RED, 30 + i * 40, 30, 15); // Simple red circles as hearts
        fill_circle(COLOR_RED, 50 + i * 40, 30, 15);
        fill_triangle(COLOR_RED, 20 + i * 40, 35, 60 + i * 40, 35, 40 + i * 40, 55);
    }
}

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

// 如果我不遍历整个数组，调用enermy_movement,敌人不会动
// 因为里面的所有敌人都是一个独立的对象
dynamic_array<enermy> enemies(0, enermy()); // initialize a dynamic array to hold enemies: 0
dynamic_array<bullet> bullets(0, bullet()); // initialize a dynamic array to hold bullets

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main()
{
    srand(time(nullptr)); // seed random number generator + random enermies position generated for each time

    // open the game window
    open_window("Survival War: Pixel", 800, 600);

    // Load and play background music
    music bgm = load_music("bgm", "Resources/sounds/cello.mp3");
    if (bgm != nullptr)
    {
        set_music_volume(0.6f); // 0.0 = silent, 1.0 = maximum volume
        play_music(bgm, -1);    // -1 means loop infinitely
        write_line("Background music loaded and playing successfully!");
    }
    else
    {
        write_line("Warning: Could not load background music file.");
    }

    // Load forest background
    bitmap forest_sprite = load_bitmap("forest_bg", "Resources/sprites/Forest/TX Tileset Grass.png");

    double background_scale = 1.0; // Default scale
    double background_offset_x = 0.0;
    double background_offset_y = 0.0;

    if (forest_sprite == nullptr)
    {
        write_line("ERROR: Failed 'Resources/sprites/Forest/TX Tileset Grass.png'");
        // No need to set a fallback color here, we'll do it in the loop if needed
    }
    else
    {
        write_line("Background bitmap loaded successfully!");
        double img_width = bitmap_width(forest_sprite);
        double img_height = bitmap_height(forest_sprite);
        write_line("Background image dimensions: " + std::to_string(img_width) + " x " + std::to_string(img_height));

        // *** Background Scaling Logic (Modified for Cover) ***
        double window_width = 800.0;
        double window_height = 600.0;

        // Calculate scales needed to cover the window width and height
        double scale_x = window_width / img_width;   // Scale to cover full width
        double scale_y = window_height / img_height; // Scale to cover full height
                                                     // Use the larger scale to ensure the image covers the entire screen
        background_scale = (scale_x > scale_y) ? scale_x : scale_y;

        // Calculate the dimensions of the scaled image
        double scaled_img_width = img_width * background_scale;
        double scaled_img_height = img_height * background_scale;

        // Calculate offsets to center the scaled image within the window
        background_offset_x = (window_width - scaled_img_width) / 2.0;
        background_offset_y = (window_height - scaled_img_height) / 2.0;
        write_line("Background scale: " + std::to_string(background_scale));
        write_line("Background offset: (" + std::to_string(background_offset_x) + ", " + std::to_string(background_offset_y) + ")");
        // *** End Background Scaling Logic ***
    }
    // create player
    player game_player;
    game_player.player_startdir(200, 100); // player initial position

    // Bullets are managed by the global dynamic array declared at the top

    // Spawn enemies ONCE before the game loop
    random_enemies(enemies, game_player.get_x(), game_player.get_y());

    // game main loop
    while (!quit_requested())
    {
        // process input
        process_events();

        // Clear screen first
        clear_screen(COLOR_BLACK);

        // Draw background using pre-calculated scaling values
        if (forest_sprite != nullptr)
        {
            drawing_options opts = option_scale_bmp(background_scale, background_scale);
            draw_bitmap(forest_sprite, background_offset_x, background_offset_y, opts);
        }
        else
        {
            clear_screen(COLOR_DARK_GREEN);
        }
        // update game logic
        game_player.player_movement();
        game_player.player_attack();
        game_player.player_health();

        // update all enemies
        for (int j = 0; j < enemies.size; j++)
        {
            enemies.data[j].enermy_movement(game_player.get_x(), game_player.get_y());
        }

        // Melee attack hit detection with cooldown
        static int attack_cooldown = 0;

        if (attack_cooldown > 0)
            attack_cooldown--;

        // Check if player just started attacking
        if (game_player.is_attacking() && attack_cooldown == 0)
        {
            rectangle atk_range = game_player.get_attack_range();

            for (int j = 0; j < enemies.size; j++)
            {
                if (enemies.data[j].be_active())
                {
                    rectangle enemy_rect = rectangle_from(enemies.data[j].get_x(), enemies.data[j].get_y(),
                                                          enemies.data[j].get_width(), enemies.data[j].get_height());
                    if (rectangles_intersect(atk_range, enemy_rect))
                    {
                        enemies.data[j].take_damage(25);
                        attack_cooldown = 30; // Prevent multiple hits
                        write_line("Player hit enemy with sword!");
                        break; // Only hit one enemy per attack
                    }
                }
            }
        }

        // Ranged attack: right-click fires a bullet toward the mouse cursor
        static int fire_cooldown = 0;
        if (fire_cooldown > 0)
            fire_cooldown--;

        if (mouse_clicked(RIGHT_BUTTON) && fire_cooldown == 0 && !game_player.is_dead())
        {
            bullet new_bullet;
            new_bullet.bullet_fire(game_player.get_x(), game_player.get_y(), mouse_x(), mouse_y());
            bullets.add(new_bullet);
            fire_cooldown = 15; // ~4 shots per second at 60 FPS
        }

        // Move all active bullets
        for (int i = 0; i < bullets.size; i++)
        {
            bullets.data[i].bullet_movement();
        }

        // Bullet-enemy collision
        const int BULLET_DAMAGE = 15;
        for (int i = 0; i < bullets.size; i++)
        {
            if (!bullets.data[i].is_active())
                continue;

            for (int j = 0; j < enemies.size; j++)
            {
                if (enemies.data[j].be_active() && collision_check(bullets.data[i], enemies.data[j]))
                {
                    enemies.data[j].take_damage(BULLET_DAMAGE);
                    bullets.data[i].deactivate();
                    write_line("Bullet hit enemy!");
                    break;
                }
            }
        }

        // Clean up inactive bullets so the array doesn't grow forever
        for (int i = bullets.size - 1; i >= 0; i--)
        {
            if (!bullets.data[i].is_active())
            {
                bullets.remove(i);
            }
        }

        // draw player
        game_player.draw();

        // visualize attack range
        if (game_player.is_attacking())
        {
            rectangle atk_range = game_player.get_attack_range();
            draw_rectangle(COLOR_RED, atk_range); // drwaw attack range
        }

        // draw all enemies
        for (int j = 0; j < enemies.size; j++)
        {
            if (enemies.data[j].be_active())
            {
                enemies.data[j].draw();
            }
        }

        // draw all active bullets
        for (int i = 0; i < bullets.size; i++)
        {
            bullets.data[i].draw();
        }

        // Check player-enemy collision (heart-based system)
        static int collision_cooldown = 0; // Prevent multiple hits per frame
        if (collision_cooldown > 0)
            collision_cooldown--;

        if (collision_cooldown == 0)
        {
            for (int j = 0; j < enemies.size; j++)
            {
                if (enemies.data[j].be_active() && player_hit_enemy(game_player, enemies.data[j]))
                {
                    game_player.lose_heart();
                    collision_cooldown = 60; // 1 second cooldown at 60 FPS
                    break; // Exit loop after first collision
                }
            }
        }

        // 敌人攻击动画造成伤害
        static int enemy_attack_cooldown = 0;
        if (enemy_attack_cooldown > 0)
            enemy_attack_cooldown--;

        for (int j = 0; j < enemies.size; j++)
        {
            if (enemies.data[j].be_active() && enemies.data[j].is_attacking())
            {
                // Check if this enemy should deal damage this frame
                if (enemies.data[j].should_deal_damage())
                {
                    // Calculate distance to see if player is in range
                    double dx = game_player.get_x() - enemies.data[j].get_x();
                    double dy = game_player.get_y() - enemies.data[j].get_y();
                    double distance = sqrt(dx * dx + dy * dy);

                    // Deal damage if player is within attack range
                    if (distance <= 50) // Attack range
                    {
                        game_player.lose_heart();
                        write_line("Enemy attack hit! Player health: " + std::to_string(game_player.get_heart()));
                    }
                }
            }
        }
        // Draw UI elements LAST so they appear on top
        draw_hearts(game_player.get_heart());
        draw_text("SPACE: sword  |  Right-click: shoot", COLOR_WHITE, "Arial", 14, 10, 570);

        // check dead status
        if (game_player.is_dead())
        {
            // 动画结束 + 停留2.5秒完毕后触发淡出
            if (game_player.is_dying_animation_done())
            {
                stop_music();

                // 屏幕淡出效果
                for (int alpha = 0; alpha <= 255; alpha += 3) // 每帧透明度+3 → ~85帧 ≈ 1.4秒
                {
                    process_events();
                    clear_screen(COLOR_BLACK);

                    // 重新绘制死亡最后一帧角色（保持画面）
                    game_player.draw();

                    // 在画面上叠加半透明黑幕
                    color fade = rgba_color(0, 0, 0, alpha);
                    fill_rectangle(fade, 0, 0, 800, 600);

                    refresh_screen(60);
                }

                // 再额外停顿0.5秒（总约2秒）
                delay(1000);

                // show "GAME OVER" text
                clear_screen(COLOR_BLACK);
                draw_text("GAME OVER", COLOR_RED, "Arial", 80, 250, 250);
                draw_text("Press ESC to exit", COLOR_WHITE, "Arial", 24, 280, 320);
                refresh_screen(60);

                // 等待 ESC 或窗口关闭
                while (!quit_requested() && !key_typed(ESCAPE_KEY))
                {
                    process_events();
                    delay(100);
                }

                break; // 退出游戏循环
            }
        }
        // refresh screen
        refresh_screen(60);
    }

    stop_music();
    // close window
    close_window("Survival War: Pixel");
    return 0;
}

#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

// Pure, stateless hit-testing between game entities.
namespace Collision
{
    bool bullet_hits_enemy(const Bullet &bullet, const Enemy &enemy);
    bool player_hits_enemy(const Player &player, const Enemy &enemy);
}

#endif

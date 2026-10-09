#include "Collision.hpp"
#include "splashkit.h"

namespace Collision
{
    bool bullet_hits_enemy(const Bullet &bullet, const Enemy &enemy)
    {
        if (!bullet.is_active() || !enemy.is_active())
            return false;

        return rectangles_intersect(bullet.get_bounds(), enemy.get_bounds());
    }

    bool player_hits_enemy(const Player &player, const Enemy &enemy)
    {
        if (!player.is_alive() || !enemy.is_active())
            return false;

        // Use smaller, centered collision boxes for more forgiving hit detection
        const double COLLISION_RATIO = 0.25;

        double pw = player.get_width() * COLLISION_RATIO;
        double ph = player.get_height() * COLLISION_RATIO;
        double ew = enemy.get_width() * COLLISION_RATIO;
        double eh = enemy.get_height() * COLLISION_RATIO;

        rectangle player_rect = rectangle_from(
            player.get_x() + (player.get_width() - pw) / 2,
            player.get_y() + (player.get_height() - ph) / 2,
            pw, ph);

        rectangle enemy_rect = rectangle_from(
            enemy.get_x() + (enemy.get_width() - ew) / 2,
            enemy.get_y() + (enemy.get_height() - eh) / 2,
            ew, eh);

        return rectangles_intersect(player_rect, enemy_rect);
    }
}

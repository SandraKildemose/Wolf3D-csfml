#include <SFML/Graphics.h>
#include <math.h>
#include "wolf3d.h"

void update_player(Player *player)
{
    float move_speed = 0.2f;  // 20% of original speed
    float rotation_speed = 0.005f;

    // Rotation controls (Q/D or Left/Right arrows)
    if (sfKeyboard_isKeyPressed(sfKeyQ) || sfKeyboard_isKeyPressed(sfKeyLeft))
        player->angle -= rotation_speed;
    if (sfKeyboard_isKeyPressed(sfKeyD) || sfKeyboard_isKeyPressed(sfKeyRight))
        player->angle += rotation_speed;

    if (player->angle < 0)
        player->angle += 2 * M_PI;
    if (player->angle > 2 * M_PI)
        player->angle -= 2 * M_PI;

    float next_x = player->x;
    float next_y = player->y;

    // Forward/Backward movement (Z/S or Up/Down arrows)
    if (sfKeyboard_isKeyPressed(sfKeyZ) || sfKeyboard_isKeyPressed(sfKeyUp)) {
        next_x += cos(player->angle) * move_speed;
        next_y += sin(player->angle) * move_speed;
    }
    if (sfKeyboard_isKeyPressed(sfKeyS) || sfKeyboard_isKeyPressed(sfKeyDown)) {
        next_x -= cos(player->angle) * move_speed;
        next_y -= sin(player->angle) * move_speed;
    }

    int map_x = (int)(next_x / TILE_SIZE);
    int map_y = (int)(next_y / TILE_SIZE);

    if (!is_wall(map_y, map_x)) {
        player->x = next_x;
        player->y = next_y;
    }
}

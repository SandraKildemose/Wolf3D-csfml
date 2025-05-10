/*
** EPITECH PROJECT, 2024
** wolf3d
** File description:
** Raycasting functions for 3D rendering
*/

#include <SFML/Graphics.h>
#include <math.h>
#include "wolf3d.h"

extern sfTexture* wall_texture;
extern sfImage* wall_image;

#define MINIMAP_SIZE 150
#define MINIMAP_TILE_SIZE (MINIMAP_SIZE / MAP_WIDTH)
#define PLAYER_DOT_SIZE 4

// Define colors
static const sfColor GATE_COLOR = {255, 215, 0, 255};  // Gold color for gates

void draw_minimap(sfRenderWindow *window, Player player)
{
    // Draw background
    sfRectangleShape *background = sfRectangleShape_create();
    sfRectangleShape_setPosition(background, (sfVector2f){10, 10});
    sfRectangleShape_setSize(background, (sfVector2f){MINIMAP_SIZE, MINIMAP_SIZE});
    sfRectangleShape_setFillColor(background, sfColor_fromRGBA(0, 0, 0, 180));
    sfRenderWindow_drawRectangleShape(window, background, NULL);
    sfRectangleShape_destroy(background);

    // Draw map tiles
    sfRectangleShape *tile = sfRectangleShape_create();
    sfRectangleShape_setSize(tile, (sfVector2f){MINIMAP_TILE_SIZE - 1, MINIMAP_TILE_SIZE - 1});

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            sfRectangleShape_setPosition(tile, (sfVector2f){
                10 + x * MINIMAP_TILE_SIZE,
                10 + y * MINIMAP_TILE_SIZE
            });

            if (map[y][x] == 1) {
                sfRectangleShape_setFillColor(tile, sfWhite);
            } else if (map[y][x] == 3) {
                sfRectangleShape_setFillColor(tile, GATE_COLOR);
            } else {
                sfRectangleShape_setFillColor(tile, sfColor_fromRGB(50, 50, 50));
            }

            sfRenderWindow_drawRectangleShape(window, tile, NULL);
        }
    }
    sfRectangleShape_destroy(tile);

    // Draw player
    sfCircleShape *player_dot = sfCircleShape_create();
    sfCircleShape_setRadius(player_dot, PLAYER_DOT_SIZE);
    sfCircleShape_setFillColor(player_dot, sfRed);
    sfCircleShape_setOrigin(player_dot, (sfVector2f){PLAYER_DOT_SIZE, PLAYER_DOT_SIZE});
    
    float player_minimap_x = 10 + (player.x / TILE_SIZE) * MINIMAP_TILE_SIZE;
    float player_minimap_y = 10 + (player.y / TILE_SIZE) * MINIMAP_TILE_SIZE;
    
    sfCircleShape_setPosition(player_dot, (sfVector2f){player_minimap_x, player_minimap_y});
    sfRenderWindow_drawCircleShape(window, player_dot, NULL);
    sfCircleShape_destroy(player_dot);

    // Draw player direction line
    sfVertexArray *direction_line = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(direction_line, sfLines);
    sfVertex start_vertex = {
        .position = {player_minimap_x, player_minimap_y},
        .color = sfRed
    };
    sfVertex end_vertex = {
        .position = {
            player_minimap_x + cos(player.angle) * MINIMAP_TILE_SIZE,
            player_minimap_y + sin(player.angle) * MINIMAP_TILE_SIZE
        },
        .color = sfRed
    };
    sfVertexArray_append(direction_line, start_vertex);
    sfVertexArray_append(direction_line, end_vertex);
    sfRenderWindow_drawVertexArray(window, direction_line, NULL);
    sfVertexArray_destroy(direction_line);
}

float cast_single_ray(Player player, float ray_angle, sfRenderWindow *window, sfColor *out_color, int *texture_x_out)
{
    (void)window;
    float ray_x = player.x;
    float ray_y = player.y;
    float ray_dir_x = cos(ray_angle);
    float ray_dir_y = sin(ray_angle);
    float distance = 0;
    float delta_dist_x = fabs(1 / ray_dir_x);
    float delta_dist_y = fabs(1 / ray_dir_y);
    int step_x = (ray_dir_x < 0) ? -1 : 1;
    int step_y = (ray_dir_y < 0) ? -1 : 1;
    int map_x = (int)(ray_x / TILE_SIZE);
    int map_y = (int)(ray_y / TILE_SIZE);
    float side_dist_x = (ray_dir_x < 0)
        ? (ray_x - map_x * TILE_SIZE) * delta_dist_x
        : ((map_x + 1) * TILE_SIZE - ray_x) * delta_dist_x;
    float side_dist_y = (ray_dir_y < 0)
        ? (ray_y - map_y * TILE_SIZE) * delta_dist_y
        : ((map_y + 1) * TILE_SIZE - ray_y) * delta_dist_y;
    int hit = 0;
    int side;
    while (!hit) {
        if (side_dist_x < side_dist_y) {
            side_dist_x += delta_dist_x * TILE_SIZE;
            map_x += step_x;
            side = 0;
        } else {
            side_dist_y += delta_dist_y * TILE_SIZE;
            map_y += step_y;
            side = 1;
        }
        if (map[map_y][map_x] > 0) {
            hit = 1;
        }
    }
    if (side == 0) {
        distance = side_dist_x - delta_dist_x * TILE_SIZE;
    } else {
        distance = side_dist_y - delta_dist_y * TILE_SIZE;
    }
    // Texture mapping için duvara çarpılan noktayı bul
    float wall_x;
    if (side == 0)
        wall_x = player.y + distance * sin(ray_angle);
    else
        wall_x = player.x + distance * cos(ray_angle);
    wall_x -= floor(wall_x / TILE_SIZE) * TILE_SIZE;
    int tex_width = wall_image ? sfImage_getSize(wall_image).x : 64;
    int texture_x = (int)(wall_x * ((float)tex_width / TILE_SIZE));
    if (texture_x < 0) texture_x = 0;
    if (texture_x >= tex_width) texture_x = tex_width - 1;
    if (texture_x_out) *texture_x_out = texture_x;
    *out_color = sfColor_fromRGB(128, 128, 128);
    return distance;
}

void render_wall_column(sfRenderWindow *window, int column, float wall_height, sfColor color, int texture_x)
{
    float start_y = (WINDOW_HEIGHT / 2.0f) - (wall_height / 2.0f);
    if (wall_texture && wall_image) {
        sfVector2u tex_size = sfTexture_getSize(wall_texture);
        sfSprite *wall_sprite = sfSprite_create();
        sfSprite_setTexture(wall_sprite, wall_texture, sfTrue);
        sfIntRect rect = {texture_x, 0, 1, (int)tex_size.y};
        sfSprite_setTextureRect(wall_sprite, rect);
        float scale_y = wall_height / (float)tex_size.y;
        sfSprite_setScale(wall_sprite, (sfVector2f){1, scale_y});
        sfSprite_setPosition(wall_sprite, (sfVector2f){column, start_y});
        sfRenderWindow_drawSprite(window, wall_sprite, NULL);
        sfSprite_destroy(wall_sprite);
    } else {
        sfRectangleShape *wall = sfRectangleShape_create();
        sfRectangleShape_setPosition(wall, (sfVector2f){column, start_y});
        sfRectangleShape_setSize(wall, (sfVector2f){1, wall_height});
        sfRectangleShape_setFillColor(wall, color);
        sfRenderWindow_drawRectangleShape(window, wall, NULL);
        sfRectangleShape_destroy(wall);
    }
}

void cast_all_rays(sfRenderWindow *window, Player player)
{
    float start_angle = player.angle - (FOV / 2.0f);
    float angle_step = FOV / NUM_RAYS;
    for (int i = 0; i < NUM_RAYS; i++) {
        float ray_angle = start_angle + i * angle_step;
        if (ray_angle < 0)
            ray_angle += 2 * M_PI;
        if (ray_angle > 2 * M_PI)
            ray_angle -= 2 * M_PI;
        sfColor wall_color;
        int texture_x;
        float distance = cast_single_ray(player, ray_angle, window, &wall_color, &texture_x);
        float wall_height = (TILE_SIZE * WINDOW_HEIGHT) / distance;
        render_wall_column(window, i, wall_height, wall_color, texture_x);
    }
    draw_minimap(window, player);
}

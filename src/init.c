#include <SFML/Graphics.h>
#include "wolf3d.h"

sfTexture* sky_texture = NULL;
sfTexture* floor_texture = NULL;
sfTexture* wall_texture = NULL;
sfImage* wall_image = NULL;
extern int player_health;

void init_player(Player *player)
{
    player->x = 0;
    player->y = 0;
    player->angle = 0;

    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            if (map[j][i] == 2) {
                player->x = i * TILE_SIZE + TILE_SIZE / 2;
                player->y = j * TILE_SIZE + TILE_SIZE / 2;
                player_health = 100;
            }
        }
    }
}

int is_wall(int x, int y)
{
    if (x < 0 || x >= MAP_HEIGHT || y < 0 || y >= MAP_WIDTH)
        return 1;
    return (map[x][y] == 1);
}

void cleanup_textures(void)
{
    if (sky_texture) sfTexture_destroy(sky_texture);
    if (floor_texture) sfTexture_destroy(floor_texture);
    if (wall_texture) sfTexture_destroy(wall_texture);
    if (wall_image) sfImage_destroy(wall_image);
    sky_texture = floor_texture = wall_texture = NULL;
    wall_image = NULL;
}

void load_level_textures(int level)
{
    cleanup_textures();
    LevelTextures tex = level_textures[level];
    
    sky_texture = sfTexture_createFromFile(tex.sky_path, NULL);
    floor_texture = sfTexture_createFromFile(tex.floor_path, NULL);
    wall_texture = sfTexture_createFromFile(tex.wall_path, NULL);
    wall_image = sfImage_createFromFile(tex.wall_path);
}

void draw_floor_and_ceiling(sfRenderWindow *window)
{
    LevelTextures tex = level_textures[current_level];
    // Draw floor with floor texture
    if (!floor_texture) {
        floor_texture = sfTexture_createFromFile("assets/textures/floor.png", NULL);
    }
    if (!wall_texture) {
        wall_texture = sfTexture_createFromFile("assets/textures/wall.png", NULL);
    }
    if (!wall_image) {
        wall_image = sfImage_createFromFile("assets/textures/wall.png");
    }
    if (floor_texture) {
        sfSprite *floor_sprite = sfSprite_create();
        sfSprite_setTexture(floor_sprite, floor_texture, sfTrue);
        sfVector2u texture_size = sfTexture_getSize(floor_texture);
        float scale_x = (float)WINDOW_WIDTH / texture_size.x;
        float scale_y = (float)(WINDOW_HEIGHT / 2.0) / texture_size.y;
        sfSprite_setScale(floor_sprite, (sfVector2f){scale_x, scale_y});
        sfSprite_setPosition(floor_sprite, (sfVector2f){0, WINDOW_HEIGHT / 2.0});
        sfRenderWindow_drawSprite(window, floor_sprite, NULL);
        sfSprite_destroy(floor_sprite);
    } else {
        sfRectangleShape *floor = sfRectangleShape_create();
        sfRectangleShape_setPosition(floor, (sfVector2f) {0, WINDOW_HEIGHT / 2.0});
        sfRectangleShape_setFillColor(floor, sfGreen);
        sfRectangleShape_setSize(floor, (sfVector2f) {WINDOW_WIDTH, WINDOW_HEIGHT / 2.0});
        sfRenderWindow_drawRectangleShape(window, floor, NULL);
        sfRectangleShape_destroy(floor);
    }
    // Draw ceiling with sky texture
    if (!sky_texture) {
        sky_texture = sfTexture_createFromFile("assets/textures/sky.png", NULL);
        if (!sky_texture) {
            sfRectangleShape *ceiling = sfRectangleShape_create();
            sfRectangleShape_setPosition(ceiling, (sfVector2f) {0, 0});
            sfRectangleShape_setFillColor(ceiling, sfBlue);
            sfRectangleShape_setSize(ceiling, (sfVector2f) {WINDOW_WIDTH, WINDOW_HEIGHT / 2.0});
            sfRenderWindow_drawRectangleShape(window, ceiling, NULL);
            sfRectangleShape_destroy(ceiling);
            return;
        }
    }
    sfSprite *sky_sprite = sfSprite_create();
    sfSprite_setTexture(sky_sprite, sky_texture, sfTrue);
    sfVector2u texture_size = sfTexture_getSize(sky_texture);
    float scale_x = (float)WINDOW_WIDTH / texture_size.x;
    float scale_y = (float)(WINDOW_HEIGHT / 2.0) / texture_size.y;
    sfSprite_setScale(sky_sprite, (sfVector2f){scale_x, scale_y});
    sfRenderWindow_drawSprite(window, sky_sprite, NULL);
    sfSprite_destroy(sky_sprite);
}

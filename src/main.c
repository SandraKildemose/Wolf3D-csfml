/*
** EPITECH PROJECT, 2024
** wolf3d
** File description:
** Main game loop and initialization
*/

#include <SFML/Graphics.h>
#include "wolf3d.h"
#include "hud.c"

// 0: empty, 1: wall, 2: player start, 3: gate
int levels[NUM_LEVELS][MAP_HEIGHT][MAP_WIDTH] = {
    { // Level 1
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 1, 0, 1},
        {1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 3, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
    },
    { // Level 2 (spiral)
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 0, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 0, 1},
        {1, 3, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
    },
    { // Level 3 (maze)
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 0, 1, 0, 0, 0, 1},
        {1, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 1, 0, 1, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 1},
        {1, 1, 0, 1, 1, 1, 3, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
    }
};

LevelTextures level_textures[NUM_LEVELS] = {
    { // Level 1 (기존)
        "assets/textures/sky.png",
        "assets/textures/floor.png", 
        "assets/textures/wall.png"
    },
    { // Level 2
        "assets/textures/level2/sky.png",
        "assets/textures/level2/floor.png",
        "assets/textures/level2/wall.png"
    },
    { // Level 3
        "assets/textures/level3/sky.png",
        "assets/textures/level3/floor.png",
        "assets/textures/level3/wall.png"
    }
};

int map[MAP_HEIGHT][MAP_WIDTH];
int current_level = 0;
Player player;  // Make player global
int score = 0;
int player_health = 100;

void load_level(int level, Player *player) {
    if (level >= NUM_LEVELS) return;
    load_level_textures(level);
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            map[y][x] = levels[level][y][x];
    init_player(player);
}

int main(void)
{
    sfVideoMode mode = {WINDOW_WIDTH, WINDOW_HEIGHT, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Wolf3D", sfResize | sfClose, NULL);

    Weapon weapon;
    load_level(current_level, &player);
    init_enemies();  // Initialize enemies
    init_menu(window);
    init_weapon(&weapon);
    score = 0;
    player_health = 100;

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            if (current_state == MENU) {
                handle_menu_input(window, &event);
            } else if (current_state == SETTINGS) {
                handle_settings_input(window, &event);
            } else if (current_state == GAME) {
                if (event.type == sfEvtKeyPressed) {
                    if (event.key.code == sfKeyEscape) {
                        current_state = MENU;
                    } else if (event.key.code == sfKeySpace) {
                        shoot_weapon(&weapon);
                    } else if (event.key.code == sfKeyNum1) {
                        switch_weapon(&weapon, WEAPON_GUN);
                    } else if (event.key.code == sfKeyNum2) {
                        switch_weapon(&weapon, WEAPON_KNIFE);
                    }
                } else if (event.type == sfEvtMouseButtonPressed) {
                    if (event.mouseButton.button == sfMouseLeft) {
                        shoot_weapon(&weapon);
                    }
                }
            } else if (current_state == WIN && event.type == sfEvtKeyPressed) {
                if (event.key.code == sfKeyEscape) {
                    current_state = MENU;
                }
            }
        }
    
        if (current_state == MENU) {
            draw_menu(window);
        } else if (current_state == SETTINGS) {
            draw_settings(window);
        } else if (current_state == GAME) {
            update_player(&player);
            update_weapon(&weapon);
            update_enemies(player);
            handle_enemy_shooting(player, &weapon);  // Handle enemy shooting
            
            // Check for gate (3) collision
            int px = (int)(player.y / TILE_SIZE);
            int py = (int)(player.x / TILE_SIZE);
            if (map[px][py] == 3) {
                current_level++;
                if (current_level < NUM_LEVELS) {
                    load_level(current_level, &player);
                } else {
                    current_state = WIN;
                }
            }
            sfRenderWindow_clear(window, sfBlack);
            draw_floor_and_ceiling(window);
            cast_all_rays(window, player);
            draw_enemies(window);  // Draw enemies
            draw_weapon(window, &weapon);
            draw_hud(window, current_level + 1, score, 4, player_health, 27, weapon.weapon_type);
            sfRenderWindow_display(window);
        } else if (current_state == WIN) {
            draw_win(window);
        }
    }
    
    cleanup_weapon(&weapon);
    return 0;
}

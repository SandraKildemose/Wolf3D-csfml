/*
** EPITECH PROJECT, 2024
** wolf3d
** File description:
** Enemy handling functions
*/

#include <SFML/Graphics.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "wolf3d.h"

Enemy enemies[4];
extern Player player;  // Add external reference to player
extern int score;

void init_enemies(void)
{
    // Only initialize enemies in first level
    if (current_level != 0) return;

    for (int i = 0; i < 4; i++) {
        enemies[i].is_active = false;
        enemies[i].health = ENEMY_SHOOT_HITS_NEEDED;
        
        // Load enemy textures
        char texture_path[50];
        snprintf(texture_path, 50, "assets/enemies/enemy%d.png", (i % 2) + 1);
        enemies[i].texture = sfTexture_createFromFile(texture_path, NULL);
        
        // Create sprite
        enemies[i].sprite = sfSprite_create();
        sfSprite_setTexture(enemies[i].sprite, enemies[i].texture, sfTrue);
        
        // Set sprite origin to center
        sfVector2u texture_size = sfTexture_getSize(enemies[i].texture);
        sfSprite_setOrigin(enemies[i].sprite, (sfVector2f){
            texture_size.x / 2.0f,
            texture_size.y / 2.0f
        });
        
        // Find random position in the last part of the map
        while (1) {
            // Spawn in the last 3 columns of the map
            int x = (rand() % 3) + (MAP_WIDTH - 3);
            int y = rand() % MAP_HEIGHT;
            if (map[y][x] == 0) {  // Empty space
                enemies[i].x = x * TILE_SIZE + TILE_SIZE / 2;
                enemies[i].y = y * TILE_SIZE + TILE_SIZE / 2;
                enemies[i].is_active = true;
                break;
            }
        }
    }
}

void update_enemies(Player player)
{
    // Only update enemies in first level
    if (current_level != 0) return;

    for (int i = 0; i < 4; i++) {
        if (!enemies[i].is_active) continue;
        
        // Calculate distance to player
        float dx = player.x - enemies[i].x;
        float dy = player.y - enemies[i].y;
        float distance = sqrt(dx * dx + dy * dy);
        
        // Calculate angle to player
        enemies[i].angle = atan2(dy, dx);
        
        // Move towards player if in detection range
        if (distance < ENEMY_DETECTION_RANGE && distance > ENEMY_ATTACK_RANGE) {
            float move_x = cos(enemies[i].angle) * ENEMY_SPEED;
            float move_y = sin(enemies[i].angle) * ENEMY_SPEED;
            
            // Check if next position is valid
            int next_x = (int)((enemies[i].x + move_x) / TILE_SIZE);
            int next_y = (int)((enemies[i].y + move_y) / TILE_SIZE);
            
            if (next_x >= 0 && next_x < MAP_WIDTH && next_y >= 0 && next_y < MAP_HEIGHT &&
                map[next_y][next_x] == 0) {
                enemies[i].x += move_x;
                enemies[i].y += move_y;
            }
        }
        
        // Update sprite position and rotation
        sfSprite_setPosition(enemies[i].sprite, (sfVector2f){enemies[i].x, enemies[i].y});
        sfSprite_setRotation(enemies[i].sprite, enemies[i].angle * 180 / M_PI);
    }
}

void draw_enemies(sfRenderWindow* window)
{
    // Only show enemies in first level
    if (current_level != 0) return;

    for (int i = 0; i < 4; i++) {
        if (!enemies[i].is_active) continue;
        
        // Calculate screen position based on player's view
        float dx = enemies[i].x - player.x;
        float dy = enemies[i].y - player.y;
        
        // Rotate enemy position based on player's angle
        float rotated_x = dx * cos(-player.angle) - dy * sin(-player.angle);
        float rotated_y = dx * sin(-player.angle) + dy * cos(-player.angle);
        
        if (rotated_x > 0) {  // Only draw if enemy is in front of player
            // Calculate screen position - moved to top of screen
            float screen_x = WINDOW_WIDTH / 2 + rotated_y * (WINDOW_WIDTH / 2) / rotated_x;
            float screen_y = WINDOW_HEIGHT * 0.05f; // Position enemies at 5% from top
            
            // Make enemies smaller by reducing the scale
            float scale = 20.0f / rotated_x; // Reduced from 25.0f to 20.0f
            sfSprite_setScale(enemies[i].sprite, (sfVector2f){scale, scale});
            
            // Center sprite
            sfVector2u texture_size = sfTexture_getSize(enemies[i].texture);
            sfSprite_setOrigin(enemies[i].sprite, (sfVector2f){
                texture_size.x / 2.0f,
                texture_size.y / 2.0f
            });
            
            // Set position and draw
            sfSprite_setPosition(enemies[i].sprite, (sfVector2f){screen_x, screen_y});
            sfRenderWindow_drawSprite(window, enemies[i].sprite, NULL);
        }
    }
}

void handle_enemy_shooting(Player player, Weapon* weapon)
{
    // Gun ile ateş
    if (weapon->weapon_type == WEAPON_GUN && weapon->is_shooting) {
        for (int i = 0; i < 4; i++) {
            if (!enemies[i].is_active) continue;
            float dx = enemies[i].x - player.x;
            float dy = enemies[i].y - player.y;
            float angle_to_enemy = atan2(dy, dx);
            float angle_diff = fabs(angle_to_enemy - player.angle);
            if (angle_diff > M_PI) angle_diff = 2 * M_PI - angle_diff;
            if (angle_diff < FOV / 2) {
                enemies[i].health--;
                if (enemies[i].health <= 0) {
                    enemies[i].is_active = false;
                    score += 50;
                }
            }
        }
    }
    // Knife ile saldırı (animasyon sırasında vur)
    if (weapon->weapon_type == WEAPON_KNIFE && weapon->knife_is_animating) {
        for (int i = 0; i < 4; i++) {
            if (!enemies[i].is_active) continue;
            float dx = enemies[i].x - player.x;
            float dy = enemies[i].y - player.y;
            float distance = sqrt(dx * dx + dy * dy);
            float angle_to_enemy = atan2(dy, dx);
            float angle_diff = fabs(angle_to_enemy - player.angle);
            if (angle_diff > M_PI) angle_diff = 2 * M_PI - angle_diff;
            // Yakın mesafe ve görüş açısında ise
            if (distance < 100 && angle_diff < FOV / 2) {
                enemies[i].health--;
                if (enemies[i].health <= 0) {
                    enemies[i].is_active = false;
                    score += 50;
                }
            }
        }
    }
}

bool check_player_death(Player player)
{
    for (int i = 0; i < 4; i++) {
        if (!enemies[i].is_active) continue;
        
        // Calculate distance to enemy
        float dx = player.x - enemies[i].x;
        float dy = player.y - enemies[i].y;
        float distance = sqrt(dx * dx + dy * dy);
        
        if (distance < ENEMY_ATTACK_RANGE) {
            return true;  // Player is dead
        }
    }
    return false;
} 
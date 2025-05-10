/*
** EPITECH PROJECT, 2024
** wolf3d
** File description:
** Weapon handling functions
*/

#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include "wolf3d.h"

void init_weapon(Weapon *weapon)
{
    printf("Loading gun from: assets/weapons/gun.png\n");
    weapon->texture = sfTexture_createFromFile("assets/weapons/gun.png", NULL);
    if (!weapon->texture) {
        fprintf(stderr, "Error: Could not load gun image\n");
        exit(1);
    }
    printf("Successfully loaded gun image\n");
    
    printf("Loading gun2 from: assets/weapons/gun2.png\n");
    weapon->shoot_texture = sfTexture_createFromFile("assets/weapons/gun2.png", NULL);
    if (!weapon->shoot_texture) {
        fprintf(stderr, "Error: Could not load gun2 image\n");
        exit(1);
    }
    printf("Successfully loaded gun2 image\n");

    weapon->knife_texture = sfTexture_createFromFile("assets/weapons/PinKnife.png", NULL);
    weapon->knife_attack_texture = sfTexture_createFromFile("assets/weapons/PinKnifea.png", NULL);

    weapon->sprite = sfSprite_create();
    weapon->weapon_type = WEAPON_GUN;
    weapon->attack_clock = sfClock_create();
    weapon->shoot_clock = sfClock_create();
    weapon->knife_clock = sfClock_create();
    weapon->knife_is_animating = false;
    sfSprite_setTexture(weapon->sprite, weapon->texture, sfTrue);
    
    sfSprite_setScale(weapon->sprite, (sfVector2f){0.3f, 0.3f});
    
    weapon->is_shooting = false;

    // Gunshot sound
    weapon->gunshot_buffer = sfSoundBuffer_createFromFile("assets/sounds/gunshot.wav");
    weapon->gunshot_sound = sfSound_create();
    if (weapon->gunshot_buffer && weapon->gunshot_sound)
        sfSound_setBuffer(weapon->gunshot_sound, weapon->gunshot_buffer);
}

void switch_weapon(Weapon* weapon, WeaponType new_type) {
    weapon->weapon_type = new_type;
    if (new_type == WEAPON_GUN) {
        sfSprite_setTexture(weapon->sprite, weapon->texture, sfTrue);
    } else {
        sfSprite_setTexture(weapon->sprite, weapon->knife_texture, sfTrue);
    }
}

void shoot_weapon(Weapon *weapon)
{
    if (weapon->weapon_type == WEAPON_KNIFE) {
        weapon->knife_is_animating = true;
        sfSprite_setTexture(weapon->sprite, weapon->knife_attack_texture, sfTrue);
        sfClock_restart(weapon->knife_clock);
        return;
    }
    weapon->is_shooting = true;
    sfSprite_setTexture(weapon->sprite, weapon->shoot_texture, sfTrue);
    sfClock_restart(weapon->shoot_clock);
}

void update_weapon(Weapon *weapon)
{
    if (weapon->weapon_type == WEAPON_KNIFE && weapon->knife_is_animating) {
        float elapsed = sfTime_asSeconds(sfClock_getElapsedTime(weapon->knife_clock));
        if (elapsed > 0.18f) {
            weapon->knife_is_animating = false;
            sfSprite_setTexture(weapon->sprite, weapon->knife_texture, sfTrue);
        }
    }
    if (weapon->weapon_type == WEAPON_GUN && weapon->is_shooting) {
        float elapsed = sfTime_asSeconds(sfClock_getElapsedTime(weapon->shoot_clock));
        if (elapsed > 0.1f) {
            weapon->is_shooting = false;
            sfSprite_setTexture(weapon->sprite, weapon->texture, sfTrue);
        }
    }
}

void draw_weapon(sfRenderWindow *window, Weapon *weapon)
{
    sfVector2u window_size = sfRenderWindow_getSize(window);
    sfVector2f position;
    sfVector2f scale = (sfVector2f){0.3f, 0.3f};
    if (weapon->weapon_type == WEAPON_KNIFE) {
        if (weapon->is_attacking) {
            position.x = window_size.x / 2.0f - (weapon->is_shooting ? 87 : 200) + 20;
            position.y = window_size.y - (weapon->is_shooting ? 413 : 300) - 20;
        } else {
            position.x = window_size.x / 2.0f - (weapon->is_shooting ? 87 : 200);
            position.y = window_size.y - (weapon->is_shooting ? 413 : 300);
        }
    } else {
        position.x = window_size.x / 2.0f - (weapon->is_shooting ? 87 : 200);
        position.y = window_size.y - (weapon->is_shooting ? 413 : 300);
    }
    sfSprite_setPosition(weapon->sprite, position);
    sfSprite_setScale(weapon->sprite, scale);
    sfRenderWindow_drawSprite(window, weapon->sprite, NULL);
}

void cleanup_weapon(Weapon *weapon)
{
    if (weapon->texture)
        sfTexture_destroy(weapon->texture);
    if (weapon->shoot_texture)
        sfTexture_destroy(weapon->shoot_texture);
    if (weapon->sprite)
        sfSprite_destroy(weapon->sprite);
    if (weapon->knife_texture)
        sfTexture_destroy(weapon->knife_texture);
    if (weapon->knife_attack_texture)
        sfTexture_destroy(weapon->knife_attack_texture);
    if (weapon->attack_clock)
        sfClock_destroy(weapon->attack_clock);
    if (weapon->shoot_clock)
        sfClock_destroy(weapon->shoot_clock);
    if (weapon->knife_clock)
        sfClock_destroy(weapon->knife_clock);
    if (weapon->gunshot_sound)
        sfSound_destroy(weapon->gunshot_sound);
    if (weapon->gunshot_buffer)
        sfSoundBuffer_destroy(weapon->gunshot_buffer);
}

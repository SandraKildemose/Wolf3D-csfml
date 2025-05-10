#include <SFML/Graphics.h>
#include "wolf3d.h"
#include <stdio.h>

static sfFont *hud_font = NULL;
static sfTexture *face_texture = NULL;
static sfSprite *face_sprite = NULL;
static sfTexture *gun_icon_texture = NULL;
static sfTexture *knife_icon_texture = NULL;
static sfSprite *weapon_icon_sprite = NULL;

void load_hud_assets() {
    if (!hud_font) {
        hud_font = sfFont_createFromFile("assets/font.ttf");
        if (!hud_font) {
            hud_font = sfFont_createFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
        }
    }
    if (!face_texture) {
        face_texture = sfTexture_createFromFile("assets/hud/face.png", NULL);
        if (!face_texture) {
            printf("Warning: Could not load face.png\n");
        }
    }
    if (!face_sprite && face_texture) {
        face_sprite = sfSprite_create();
        sfSprite_setTexture(face_sprite, face_texture, sfTrue);
        sfSprite_setScale(face_sprite, (sfVector2f){1.0f, 1.0f});
    }
    if (!gun_icon_texture) {
        gun_icon_texture = sfTexture_createFromFile("assets/hud/gun_icon.png", NULL);
    }
    if (!knife_icon_texture) {
        knife_icon_texture = sfTexture_createFromFile("assets/hud/knife_icon.png", NULL);
    }
    if (!weapon_icon_sprite) {
        weapon_icon_sprite = sfSprite_create();
    }
}

void draw_hud(sfRenderWindow *window, int floor, int score, int lives, int health, int ammo, WeaponType weapon_type) {
    load_hud_assets();
    float hud_height = 80;
    sfRectangleShape *panel = sfRectangleShape_create();
    sfRectangleShape_setSize(panel, (sfVector2f){WINDOW_WIDTH, hud_height});
    sfRectangleShape_setPosition(panel, (sfVector2f){0, WINDOW_HEIGHT - hud_height});
    sfRectangleShape_setFillColor(panel, sfColor_fromRGB(20, 40, 160));
    sfRenderWindow_drawRectangleShape(window, panel, NULL);
    sfRectangleShape_destroy(panel);

    // Draw white border
    sfRectangleShape *border = sfRectangleShape_create();
    sfRectangleShape_setSize(border, (sfVector2f){WINDOW_WIDTH-4, hud_height-4});
    sfRectangleShape_setPosition(border, (sfVector2f){2, WINDOW_HEIGHT - hud_height + 2});
    sfRectangleShape_setOutlineThickness(border, 2);
    sfRectangleShape_setOutlineColor(border, sfWhite);
    sfRectangleShape_setFillColor(border, sfColor_fromRGBA(0,0,0,0));
    sfRenderWindow_drawRectangleShape(window, border, NULL);
    sfRectangleShape_destroy(border);

    // Draw text fields
    char buf[64];
    sfText *text = sfText_create();
    sfText_setFont(text, hud_font);
    sfText_setCharacterSize(text, 28);
    sfText_setFillColor(text, sfWhite);

    // FLOOR
    snprintf(buf, sizeof(buf), "FLOOR\n%2d", floor);
    sfText_setString(text, buf);
    sfText_setPosition(text, (sfVector2f){18, WINDOW_HEIGHT - hud_height + 8});
    sfRenderWindow_drawText(window, text, NULL);

    // SCORE
    snprintf(buf, sizeof(buf), "SCORE\n%06d", score);
    sfText_setString(text, buf);
    sfText_setPosition(text, (sfVector2f){120, WINDOW_HEIGHT - hud_height + 8});
    sfRenderWindow_drawText(window, text, NULL);

    // LIVES
    snprintf(buf, sizeof(buf), "LIVES\n%1d", lives);
    sfText_setString(text, buf);
    sfText_setPosition(text, (sfVector2f){320, WINDOW_HEIGHT - hud_height + 8});
    sfRenderWindow_drawText(window, text, NULL);

    // FACE FRAME (no fill, thin outline)
    float face_frame_x = 400, face_frame_y = WINDOW_HEIGHT - hud_height + 16, face_frame_size = 36;
    sfRectangleShape *face_frame = sfRectangleShape_create();
    sfRectangleShape_setSize(face_frame, (sfVector2f){face_frame_size, face_frame_size});
    sfRectangleShape_setPosition(face_frame, (sfVector2f){face_frame_x, face_frame_y});
    sfRectangleShape_setFillColor(face_frame, sfColor_fromRGBA(0,0,0,0));
    sfRectangleShape_setOutlineThickness(face_frame, 2);
    sfRectangleShape_setOutlineColor(face_frame, sfWhite);
    sfRenderWindow_drawRectangleShape(window, face_frame, NULL);
    sfRectangleShape_destroy(face_frame);

    // FACE (fill frame exactly)
    if (face_sprite && face_texture) {
        sfVector2u face_tex_size = sfTexture_getSize(face_texture);
        float face_scale = face_frame_size / (float)face_tex_size.x;
        sfSprite_setScale(face_sprite, (sfVector2f){face_scale, face_scale});
        sfSprite_setPosition(face_sprite, (sfVector2f){face_frame_x, face_frame_y});
        sfRenderWindow_drawSprite(window, face_sprite, NULL);
    }

    // HEALTH
    snprintf(buf, sizeof(buf), "HEALTH\n%3d%%", health);
    sfText_setString(text, buf);
    sfText_setPosition(text, (sfVector2f){500, WINDOW_HEIGHT - hud_height + 8});
    sfRenderWindow_drawText(window, text, NULL);

    // AMMO
    snprintf(buf, sizeof(buf), "AMMO\n%3d", ammo);
    sfText_setString(text, buf);
    sfText_setPosition(text, (sfVector2f){650, WINDOW_HEIGHT - hud_height + 8});
    sfRenderWindow_drawText(window, text, NULL);

    // WEAPON ICON FRAME (no fill, thin outline)
    float icon_frame_x = 740, icon_frame_y = WINDOW_HEIGHT - hud_height + 22, icon_frame_size = 32;
    sfRectangleShape *icon_frame = sfRectangleShape_create();
    sfRectangleShape_setSize(icon_frame, (sfVector2f){icon_frame_size, icon_frame_size});
    sfRectangleShape_setPosition(icon_frame, (sfVector2f){icon_frame_x, icon_frame_y});
    sfRectangleShape_setFillColor(icon_frame, sfColor_fromRGBA(0,0,0,0));
    sfRectangleShape_setOutlineThickness(icon_frame, 2);
    sfRectangleShape_setOutlineColor(icon_frame, sfWhite);
    sfRenderWindow_drawRectangleShape(window, icon_frame, NULL);
    sfRectangleShape_destroy(icon_frame);

    // Weapon icon (fill frame exactly)
    if (weapon_icon_sprite) {
        sfTexture* icon_tex = NULL;
        if (weapon_type == WEAPON_GUN && gun_icon_texture) {
            icon_tex = gun_icon_texture;
            sfSprite_setTexture(weapon_icon_sprite, gun_icon_texture, sfTrue);
        } else if (weapon_type == WEAPON_KNIFE && knife_icon_texture) {
            icon_tex = knife_icon_texture;
            sfSprite_setTexture(weapon_icon_sprite, knife_icon_texture, sfTrue);
        }
        if (icon_tex) {
            sfVector2u icon_tex_size = sfTexture_getSize(icon_tex);
            float icon_scale = icon_frame_size / (float)icon_tex_size.x;
            sfSprite_setScale(weapon_icon_sprite, (sfVector2f){icon_scale, icon_scale});
            sfSprite_setPosition(weapon_icon_sprite, (sfVector2f){icon_frame_x, icon_frame_y});
            sfRenderWindow_drawSprite(window, weapon_icon_sprite, NULL);
        }
    }

    sfText_destroy(text);
} 
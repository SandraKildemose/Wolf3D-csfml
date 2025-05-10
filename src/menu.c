/*
** EPITECH PROJECT, 2024
** wolf3d
** File description:
** Menu and settings handling functions
*/

#include <SFML/Graphics.h>
#include <stdio.h>
#include "wolf3d.h"

GameState current_state = MENU;
Settings game_settings = {
    .music_volume = 50.0f,
    .sound_volume = 50.0f,
    .window_width = WINDOW_WIDTH,
    .window_height = WINDOW_HEIGHT
};

static sfFont *menu_font = NULL;
static sfText *title_text;
static sfText *start_text;
static sfText *settings_text;
static sfText *volume_text;
static sfText *window_text;
static sfRectangleShape *menu_background;
static sfRectangleShape *button_background;

// Helper function to center text horizontally
static void center_text_horizontally(sfText *text)
{
    sfFloatRect bounds = sfText_getGlobalBounds(text);
    float x = (WINDOW_WIDTH - bounds.width) / 2;
    sfVector2f pos = sfText_getPosition(text);
    sfText_setPosition(text, (sfVector2f){x, pos.y});
}

void init_menu(sfRenderWindow *window)
{
    (void)window;  // Mark parameter as intentionally unused
    menu_background = sfRectangleShape_create();
    sfRectangleShape_setSize(menu_background, (sfVector2f){WINDOW_WIDTH, WINDOW_HEIGHT});
    sfRectangleShape_setFillColor(menu_background, sfColor_fromRGBA(0, 0, 0, 200));

    button_background = sfRectangleShape_create();
    sfRectangleShape_setSize(button_background, (sfVector2f){300, 60});
    sfRectangleShape_setFillColor(button_background, sfColor_fromRGB(50, 50, 50));

    menu_font = sfFont_createFromFile("assets/font.ttf");
    if (!menu_font) {
        menu_font = sfFont_createFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
        if (!menu_font) {
            printf("Error: Could not load any font!\n");
            return;
        }
    }
    
    // Create title text
    title_text = sfText_create();
    sfText_setString(title_text, "WOLF3D");
    sfText_setFont(title_text, menu_font);
    sfText_setCharacterSize(title_text, 100);
    sfText_setFillColor(title_text, sfWhite);
    sfText_setPosition(title_text, (sfVector2f){0, WINDOW_HEIGHT * 0.2});
    center_text_horizontally(title_text);
    
    // Create start game button
    start_text = sfText_create();
    sfText_setString(start_text, "Start Game");
    sfText_setFont(start_text, menu_font);
    sfText_setCharacterSize(start_text, 40);
    sfText_setFillColor(start_text, sfWhite);
    sfText_setPosition(start_text, (sfVector2f){0, WINDOW_HEIGHT * 0.5});
    center_text_horizontally(start_text);
    
    // Create settings button
    settings_text = sfText_create();
    sfText_setString(settings_text, "Settings");
    sfText_setFont(settings_text, menu_font);
    sfText_setCharacterSize(settings_text, 40);
    sfText_setFillColor(settings_text, sfWhite);
    sfText_setPosition(settings_text, (sfVector2f){0, WINDOW_HEIGHT * 0.65});
    center_text_horizontally(settings_text);
}

void draw_menu(sfRenderWindow *window)
{
    if (!menu_font) return;

    sfRenderWindow_clear(window, sfBlack);
    sfRenderWindow_drawRectangleShape(window, menu_background, NULL);
    
    // Draw button backgrounds
    sfFloatRect bounds;
    
    // Start game button background
    bounds = sfText_getGlobalBounds(start_text);
    sfRectangleShape_setPosition(button_background, 
        (sfVector2f){(WINDOW_WIDTH - 300) / 2, bounds.top - 10});
    sfRenderWindow_drawRectangleShape(window, button_background, NULL);
    
    // Settings button background
    bounds = sfText_getGlobalBounds(settings_text);
    sfRectangleShape_setPosition(button_background, 
        (sfVector2f){(WINDOW_WIDTH - 300) / 2, bounds.top - 10});
    sfRenderWindow_drawRectangleShape(window, button_background, NULL);
    
    sfRenderWindow_drawText(window, title_text, NULL);
    sfRenderWindow_drawText(window, start_text, NULL);
    sfRenderWindow_drawText(window, settings_text, NULL);
    
    sfRenderWindow_display(window);
}

void handle_menu_input(sfRenderWindow *window, sfEvent *event)
{
    if (!menu_font) return;

    if (event->type == sfEvtMouseButtonPressed) {
        sfVector2i mouse_pos = sfMouse_getPositionRenderWindow(window);
        sfFloatRect start_bounds = sfText_getGlobalBounds(start_text);
        sfFloatRect settings_bounds = sfText_getGlobalBounds(settings_text);
        
        if (sfFloatRect_contains(&start_bounds, mouse_pos.x, mouse_pos.y)) {
            current_state = GAME;
        } else if (sfFloatRect_contains(&settings_bounds, mouse_pos.x, mouse_pos.y)) {
            current_state = SETTINGS;
        }
    }
}

void draw_settings(sfRenderWindow *window)
{
    if (!menu_font) return;

    sfRenderWindow_clear(window, sfBlack);
    sfRenderWindow_drawRectangleShape(window, menu_background, NULL);
    
    // Settings title
    sfText *settings_title = sfText_create();
    sfText_setString(settings_title, "Settings");
    sfText_setFont(settings_title, menu_font);
    sfText_setCharacterSize(settings_title, 60);
    sfText_setFillColor(settings_title, sfWhite);
    sfText_setPosition(settings_title, (sfVector2f){0, WINDOW_HEIGHT * 0.1});
    center_text_horizontally(settings_title);
    
    char volume_str[50];
    snprintf(volume_str, 50, "Music Volume: %.0f%%", game_settings.music_volume);
    volume_text = sfText_create();
    sfText_setString(volume_text, volume_str);
    sfText_setFont(volume_text, menu_font);
    sfText_setCharacterSize(volume_text, 30);
    sfText_setFillColor(volume_text, sfWhite);
    sfText_setPosition(volume_text, (sfVector2f){0, WINDOW_HEIGHT * 0.4});
    center_text_horizontally(volume_text);
    
    char window_str[50];
    snprintf(window_str, 50, "Window Size: %dx%d", game_settings.window_width, game_settings.window_height);
    window_text = sfText_create();
    sfText_setString(window_text, window_str);
    sfText_setFont(window_text, menu_font);
    sfText_setCharacterSize(window_text, 30);
    sfText_setFillColor(window_text, sfWhite);
    sfText_setPosition(window_text, (sfVector2f){0, WINDOW_HEIGHT * 0.5});
    center_text_horizontally(window_text);
    
    sfRenderWindow_drawText(window, settings_title, NULL);
    sfRenderWindow_drawText(window, volume_text, NULL);
    sfRenderWindow_drawText(window, window_text, NULL);
    
    sfRenderWindow_display(window);
    
    sfText_destroy(settings_title);
    sfText_destroy(volume_text);
    sfText_destroy(window_text);
}

void handle_settings_input(sfRenderWindow *window, sfEvent *event)
{
    (void)window;

    if (event->type == sfEvtKeyPressed) {
        if (event->key.code == sfKeyEscape) {
            current_state = MENU;
        }
    }
    
    if (event->type == sfEvtMouseWheelScrolled) {
        game_settings.music_volume += event->mouseWheelScroll.delta * 5;
        if (game_settings.music_volume > 100) game_settings.music_volume = 100;
        if (game_settings.music_volume < 0) game_settings.music_volume = 0;
    }
}

void draw_win(sfRenderWindow *window)
{
    sfRenderWindow_clear(window, sfBlack);
    static sfFont *font = NULL;
    if (!font) font = sfFont_createFromFile("assets/font.ttf");
    sfText *text = sfText_create();
    sfText_setString(text, "You Win!\nPress ESC to return to menu");
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 50);
    sfText_setFillColor(text, sfWhite);
    sfFloatRect bounds = sfText_getGlobalBounds(text);
    sfText_setPosition(text, (sfVector2f){(WINDOW_WIDTH - bounds.width) / 2, (WINDOW_HEIGHT - bounds.height) / 2});
    sfRenderWindow_drawText(window, text, NULL);
    sfText_destroy(text);
    sfRenderWindow_display(window);
} 
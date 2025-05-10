#ifndef INCLUDES_WOLF_H_
#define INCLUDES_WOLF_H_

#define TILE_SIZE 64
#define MAP_WIDTH 8
#define MAP_HEIGHT 8
#define NUM_LEVELS 3

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define FOV (M_PI / 3)
#define NUM_RAYS 800

// Weapon constants
#define TOTAL_FRAMES 4
#define FRAME_WIDTH 64
#define FRAME_HEIGHT 64

// Enemy constants
#define ENEMY_SPEED 1.0f
#define ENEMY_DETECTION_RANGE 200.0f
#define ENEMY_ATTACK_RANGE 50.0f
#define ENEMY_SHOOT_HITS_NEEDED 5

#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <math.h>
#include <stdbool.h>

typedef enum {
    MENU,
    GAME,
    SETTINGS,
    WIN
} GameState;

typedef struct {
    const char *sky_path;
    const char *floor_path;
    const char *wall_path;
} LevelTextures;

extern LevelTextures level_textures[NUM_LEVELS];

typedef struct {
    float x;
    float y;
    float angle;
} Player;

typedef struct {
    float music_volume;
    float sound_volume;
    int window_width;
    int window_height;
} Settings;

typedef enum {
    WEAPON_GUN,
    WEAPON_KNIFE
} WeaponType;

typedef struct {
    sfSprite* sprite;
    sfTexture* texture;
    sfTexture* shoot_texture;
    sfIntRect frames[TOTAL_FRAMES];
    int current_frame;
    sfClock* anim_clock;
    sfClock* shoot_clock;
    bool is_shooting;
    WeaponType weapon_type;
    sfTexture* knife_texture;
    sfTexture* knife_attack_texture;
    bool is_attacking;
    sfClock* attack_clock;
    bool knife_damage_done;
    sfSoundBuffer* gunshot_buffer;
    sfSound* gunshot_sound;
    sfClock* knife_clock;
    bool knife_is_animating;
} Weapon;

typedef struct {
    float x;
    float y;
    float angle;
    int health;
    bool is_active;
    sfSprite* sprite;
    sfTexture* texture;
    sfTexture* texture2;
} Enemy;

extern int levels[NUM_LEVELS][MAP_HEIGHT][MAP_WIDTH];
extern int map[MAP_HEIGHT][MAP_WIDTH];
extern int current_level;
extern GameState current_state;
extern Settings game_settings;
extern sfTexture* sky_texture;
extern sfTexture* floor_texture;
extern Enemy enemies[4];  // Four enemies

void load_level(int level, Player *player);
void init_player(Player *player);
int is_wall(int x, int y);
void draw_floor_and_ceiling(sfRenderWindow *window);
void load_level_textures(int level);

// for raycasting
void cast_all_rays(sfRenderWindow *window, Player player);
float cast_single_ray(Player player, float ray_angle, sfRenderWindow *window, sfColor *out_color, int *texture_x_out);
void render_wall_column(sfRenderWindow *window, int column, float wall_height, sfColor color, int texture_x);
void draw_minimap(sfRenderWindow *window, Player player);

void update_player(Player *player);

// Menu functions
void init_menu(sfRenderWindow *window);
void draw_menu(sfRenderWindow *window);
void handle_menu_input(sfRenderWindow *window, sfEvent *event);
void draw_settings(sfRenderWindow *window);
void handle_settings_input(sfRenderWindow *window, sfEvent *event);
void draw_win(sfRenderWindow *window);

// Weapon functions
void init_weapon(Weapon* weapon);
void shoot_weapon(Weapon* weapon);
void update_weapon(Weapon* weapon);
void draw_weapon(sfRenderWindow* window, Weapon* weapon);
void cleanup_weapon(Weapon* weapon);
void switch_weapon(Weapon* weapon, WeaponType new_type);

// Enemy functions
void init_enemies(void);
void update_enemies(Player player);
void draw_enemies(sfRenderWindow* window);
void handle_enemy_shooting(Player player, Weapon* weapon);
bool check_player_death(Player player);

void draw_hud(sfRenderWindow *window, int floor, int score, int lives, int health, int ammo, WeaponType weapon_type);

#endif /* INCLUDES_WOLF_H_ */

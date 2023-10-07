#ifndef PLAYING_H
#define PLAYING_H

#include <gb/gb.h>
#include <types.h>

#include "tileset_may.h"
#include "util.h"

#define TILESET_MAY_BEGIN 0
#define TILESET_MAY_W 15
#define TILESET_MAY_AT(x, y)                                                   \
    (TILESET_MAY_BEGIN + ((x)*2) + ((y) / 2) * (TILESET_MAY_W * 2))

#define TILESET_CRIB_BEGIN TILESET_MAY_TILE_COUNT
#define TILESET_CRIB_W 4
#define TILESET_CRIB_AT(x, y)                                                  \
    (TILESET_CRIB_BEGIN + ((x)*2) + ((y) / 2) * (TILESET_CRIB_W * 2))

#define TILESET_AREA_AT(x, y) (TILEMAP_AREA[TILEMAP_AREA_WIDTH * (y) + (x)])

#define TILESET_INFOS_TILE_BASE TILESET_AREA_TILE_COUNT
#define TILESET_INFOS_W 10
#define TILESET_INFOS_AT(x, y)                                                 \
    (TILESET_INFOS_TILE_BASE + (x) + ((y)*TILESET_INFOS_W))

#define AREA_PADDING_Y 2

#define PLAYER_SPR_BASE 0
#define PLAYER_SPR_SIZE 6
#define PLAYER_SPR_END (PLAYER_SPR_SIZE - 1)

#define ENEMY_SPR_BASE (PLAYER_SPR_END + 1)
#define ENEMY_SPR_SIZE 2
#define ENEMY_SPR_COUNT_4 4
#define ENEMY_SPR_END (ENEMY_SPR_BASE + ENEMY_SPR_SIZE * ENEMY_SPR_COUNT_4 - 1)

typedef struct {
    INT8 x;
    INT8 y;
    INT8 dx;
    INT8 dy;
    UINT8 speed;
    UINT8 move_timer;
    UINT8 move_timer_max;
} MovableTrait;

#define BOX16_16 16

BOOL is_movable_stopped(MovableTrait *movable);
void reset_movable(MovableTrait *movable, UINT8 speed, UINT8 timer_max);
BOOL move_movable(MovableTrait *movable);
BOOL can_box16_pass_area(INT8 x, INT8 y);
void proceed_dir(INT8 *x, INT8 *y, UINT8 dir, INT8 step);

typedef struct {
    UINT8 dir_cache;
    UINT8 dir_cache_horizontal;
    UINT8 anim_timer;
    UINT8 anim_frame;
    MovableTrait movable;
} Player;

typedef struct {
    BOOL is_using;
    UINT8 spr_base;
    UINT8 anim_timer;
    UINT8 anim_frame;
    MovableTrait movable;
} Enemy;
typedef struct {
    UINT8 timer;
    Enemy enemies[ENEMY_SPR_COUNT_4];
} EnemyManager;

typedef struct {
    Player player;
    EnemyManager enemy_manager;
} PlayingScene;

// Reset functions
void reset_player(Player *player);
void reset_enemy_manager(EnemyManager *manager);

// Update functions
void update_player(Player *player);
void update_enemy_manger(EnemyManager *manager);

static void reset_playing_scene(PlayingScene *scene) {
    reset_player(&scene->player);
    reset_enemy_manager(&scene->enemy_manager);
}

static void update_playing_scene(PlayingScene *scene) {
    update_player(&scene->player);
    update_enemy_manger(&scene->enemy_manager);
}

#endif // PLAYING_H

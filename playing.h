#ifndef PLAYING_H
#define PLAYING_H

#include <gb/gb.h>
#include <types.h>

#include "types.h"
#include "util.h"

#define TILESET_MAY_W 15
#define TILESET_MAY_AT(x, y) (((x)*2) + ((y) / 2) * (TILESET_MAY_W * 2))

#define TILESET_AREA_AT(x, y) (TILEMAP_AREA[TILEMAP_AREA_WIDTH * (y) + (x)])

#define TILESET_INFOS_TILE_BASE TILESET_AREA_TILE_COUNT
#define TILESET_INFOS_W 10
#define TILESET_INFOS_AT(x, y)                                                 \
    (TILESET_INFOS_TILE_BASE + (x) + ((y)*TILESET_INFOS_W))

#define AREA_PADDING_Y 2

#define PLAYER_SPR_BASE 0
#define PLAYER_SPR_SIZE 6

typedef struct {
    INT8 x;
    INT8 y;
    INT8 dx;
    INT8 dy;
    UINT8 speed;
    UINT8 move_timer;
    UINT8 move_timer_max;
} MovableTrait;

BOOL is_movable_active(MovableTrait *movable) {
    return movable->dx == 0 && movable->dy == 0;
}

void reset_movable(MovableTrait *movable, UINT8 speed, UINT8 timer_max) {
    movable->x = 0;
    movable->y = 0;
    movable->dx = 0;
    movable->dy = 0;
    movable->speed = speed;
    movable->move_timer = 0;
    movable->move_timer_max = timer_max;
}

BOOL move_movable(MovableTrait *movable) {
    INT8 dx, dy;
    dx = movable->dx;
    dy = movable->dy;

    if (dx == 0 && dy == 0)
        return FALSE;

    if (movable->move_timer == 0) {
        movable->move_timer = movable->move_timer_max;
        if (dx > 0) {
            movable->dx--;
            movable->x += movable->speed;
        } else if (dx < 0) {
            movable->dx++;
            movable->x -= movable->speed;
        } else if (dy < 0) {
            movable->dy++;
            movable->y -= movable->speed;
        } else if (dy > 0) {
            movable->dy--;
            movable->y += movable->speed;
        }
        return TRUE;
    } else {
        movable->move_timer--;
        return FALSE;
    }
}

#define BOX16_16 16

BOOL can_box16_pass_area(INT8 x, INT8 y) {
    if (MOD_P2(DIV_P2((x + 8), BOX16_16), 2) == 0 &&
        (MOD_P2(DIV_P2(x + 8 + BOX16_16 - 1, BOX16_16), 2) == 0))
        return TRUE;
    if (MOD_P2(DIV_P2(y, BOX16_16), 2) != 0 &&
        MOD_P2(DIV_P2(y + BOX16_16 - 1, BOX16_16), 2) != 0)
        return TRUE;
    return FALSE;
}

void proceed_dir(INT8 *x, INT8 *y, UINT8 dir, INT8 step) {
    switch (dir) {
    case DIR_LEFT:
        *x -= step;
        break;
    case DIR_RIGHT:
        *x += step;
        break;
    case DIR_UP:
        *y -= step;
        break;
    case DIR_DOWN:
        *y += step;
        break;
    default:
        break;
    }
}

#endif // PLAYING_H

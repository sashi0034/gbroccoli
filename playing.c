#include "playing.h"

BOOL is_movable_stopped(MovableTrait *movable) {
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

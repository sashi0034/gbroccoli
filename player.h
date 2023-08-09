#ifndef PLAYER_H
#define PLAYER_H

#include "playing.h"

void fill_player_tiles(UINT8 base_u, BOOL is_flip) {
    UINT8 i, flag;

    flag = is_flip ? S_PALETTE | S_FLIPX : S_PALETTE;

    for (i = 0; i < PLAYER_SPR_SIZE; ++i)
        set_sprite_prop(PLAYER_SPR_BASE + i, flag);

    if (is_flip) {
        set_sprite_tile(
            PLAYER_SPR_BASE + 0,
            TILESET_MAY_AT(base_u + (PLAYER_SPR_SIZE / 2 - 1) - 0, 0));
        set_sprite_tile(
            PLAYER_SPR_BASE + 1,
            TILESET_MAY_AT(base_u + (PLAYER_SPR_SIZE / 2 - 1) - 1, 0));
        set_sprite_tile(
            PLAYER_SPR_BASE + 2,
            TILESET_MAY_AT(base_u + (PLAYER_SPR_SIZE / 2 - 1) - 2, 0));
        set_sprite_tile(
            PLAYER_SPR_BASE + 3,
            TILESET_MAY_AT(base_u + (PLAYER_SPR_SIZE / 2 - 1) - 0, 2));
        set_sprite_tile(
            PLAYER_SPR_BASE + 4,
            TILESET_MAY_AT(base_u + (PLAYER_SPR_SIZE / 2 - 1) - 1, 2));
        set_sprite_tile(
            PLAYER_SPR_BASE + 5,
            TILESET_MAY_AT(base_u + (PLAYER_SPR_SIZE / 2 - 1) - 2, 2));
    } else {
        set_sprite_tile(PLAYER_SPR_BASE + 0, TILESET_MAY_AT(base_u + 0, 0));
        set_sprite_tile(PLAYER_SPR_BASE + 1, TILESET_MAY_AT(base_u + 1, 0));
        set_sprite_tile(PLAYER_SPR_BASE + 2, TILESET_MAY_AT(base_u + 2, 0));
        set_sprite_tile(PLAYER_SPR_BASE + 3, TILESET_MAY_AT(base_u + 0, 2));
        set_sprite_tile(PLAYER_SPR_BASE + 4, TILESET_MAY_AT(base_u + 1, 2));
        set_sprite_tile(PLAYER_SPR_BASE + 5, TILESET_MAY_AT(base_u + 2, 2));
    }
}

void anim_player_down(UINT8 frame) {
    if (frame == 0) {
        fill_player_tiles(0, FALSE);
    } else if (frame == 1) {
        fill_player_tiles(3, FALSE);
    } else if (frame == 2) {
        fill_player_tiles(0, TRUE);
    } else {
        fill_player_tiles(3, TRUE);
    }
}

void anim_player_horizontal(UINT8 frame, BOOL is_left) {
    if (frame == 0) {
        fill_player_tiles(6, is_left);
    } else if (frame == 1) {
        fill_player_tiles(9, is_left);
    }
    if (frame == 2) {
        fill_player_tiles(6, is_left);
    } else if (frame == 3) {
        fill_player_tiles(12, is_left);
    }
}

void locate_player(UINT8 player_x, UINT8 player_y) {
    player_x = player_x + SPR_GAP_X - 4;
    player_y = player_y + SPR_GAP_Y - 16 - AREA_PADDING_Y;

    move_sprite(PLAYER_SPR_BASE + 0, player_x, player_y);
    move_sprite(PLAYER_SPR_BASE + 1, player_x + 8, player_y);
    move_sprite(PLAYER_SPR_BASE + 2, player_x + 16, player_y);
    move_sprite(PLAYER_SPR_BASE + 3, player_x, player_y + 16);
    move_sprite(PLAYER_SPR_BASE + 4, player_x + 8, player_y + 16);
    move_sprite(PLAYER_SPR_BASE + 5, player_x + 16, player_y + 16);
}

typedef struct {
    UINT8 dir_cache;
    UINT8 dir_cache_horizontal;
    UINT8 anim_timer;
    UINT8 anim_frame;
    MovableTrait movable;
} Player;

void reset_player(Player *player) {
    player->dir_cache = DIR_DOWN;
    player->dir_cache_horizontal = DIR_LEFT;
    player->anim_timer = 0;
    player->anim_frame = 0;

    reset_movable(&player->movable, 1, 0);
    player->movable.x = 56;
    player->movable.y = 48;

    locate_player(player->movable.x, player->movable.y);
    anim_player_down(0);
}

#define PLAYER_STRIDE_8 8

void input_player_dir(Player *player) {
    UINT8 joyp;
    INT8 next_dir, next_x, next_y;

    if (is_movable_stopped(&(player->movable)) == FALSE)
        return;

    joyp = joypad();

    // ジョイパッド入力
    next_dir = -1;
    if (joyp & J_UP) {
        next_dir = DIR_UP;
    } else if (joyp & J_DOWN) {
        next_dir = DIR_DOWN;
    } else if (joyp & J_LEFT) {
        next_dir = DIR_LEFT;
    } else if (joyp & J_RIGHT) {
        next_dir = DIR_RIGHT;
    }

    if (next_dir == -1)
        return;

    next_x = player->movable.x;
    next_y = player->movable.y;
    proceed_dir(&next_x, &next_y, next_dir, PLAYER_STRIDE_8);

    if (can_box16_pass_area(next_x, next_y) == FALSE) {
        return;
    }

    // 移動方向決定
    if (next_dir == DIR_UP) {
        if (player->dir_cache != player->dir_cache_horizontal)
            player->anim_timer = 0;
        player->dir_cache = player->dir_cache_horizontal;
        player->movable.dy = -PLAYER_STRIDE_8;
    } else if (next_dir == DIR_DOWN) {
        if (player->dir_cache != DIR_DOWN)
            player->anim_timer = 0;
        player->dir_cache = DIR_DOWN;
        player->movable.dy = PLAYER_STRIDE_8;
    } else if (next_dir == DIR_LEFT) {
        if (player->dir_cache != DIR_LEFT)
            player->anim_timer = 0;
        player->dir_cache = DIR_LEFT;
        player->dir_cache_horizontal = DIR_LEFT;
        player->movable.dx = -PLAYER_STRIDE_8;
    } else if (next_dir == DIR_RIGHT) {
        if (player->dir_cache != DIR_RIGHT)
            player->anim_timer = 0;
        player->dir_cache = DIR_RIGHT;
        player->dir_cache_horizontal = DIR_RIGHT;
        player->movable.dx = PLAYER_STRIDE_8;
    }
}

void update_player(Player *player) {
    // 入力
    input_player_dir(player);

    // 移動
    if (move_movable(&player->movable)) {
        locate_player(player->movable.x, player->movable.y);
    }

    // アニメーション
    if (player->anim_timer == 0) {
        player->anim_timer = 15;
        player->anim_frame = MOD_P2((player->anim_frame + 1), 4);

        if (player->dir_cache == DIR_DOWN) {
            anim_player_down(player->anim_frame);
        } else {
            anim_player_horizontal(player->anim_frame,
                                   player->dir_cache == DIR_LEFT);
        }
    } else {
        player->anim_timer--;
    }
}

#endif // PLAYER_H

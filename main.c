// Original Source: https://github.com/flozz/gameboy-examples/tree/master

#define INT_16_BITS

#include <types.h>
#include <gb/gb.h>
#include "tileset_may.h"
#include "tileset_area.h"
#include "tilemap_area.h"

#define BOOL UINT8

#define TILESET_MAY_W 15
#define TILESET_MAY_AT(x, y) (((x)*2) + ((y) / 2) * (TILESET_MAY_W * 2))

#define TILESET_AREA_AT(x, y) (TILEMAP_AREA[TILEMAP_AREA_WIDTH * (y) + (x)])

// Id ("nb") of the two sprites used to represent the player
#define PLAYER_SPRITE_BASE 0
#define PLAYER_SPRITE_SIZE 6

enum Direction4
{
    DIR_DOWN,
    DIR_UP,
    DIR_LEFT,
    DIR_RIGHT
};

// Variables containing player state
UINT8 player_x;
UINT8 player_y;
UINT8 player_direction;
UINT8 player_anim_dir;
UINT8 player_anim_frame;
UINT8 is_player_walking;

#define BG_BUFFER_W 20
#define BG_BUFFER_H 20
UINT8 bg_buffer[BG_BUFFER_W][BG_BUFFER_H];

void fill_player_tiles(UINT8 base_u, BOOL is_flip)
{
    UINT8 i, flag;

    flag = is_flip
               ? S_PALETTE | S_FLIPX
               : S_PALETTE;

    for (i = 0; i < PLAYER_SPRITE_SIZE; ++i)
        set_sprite_prop(PLAYER_SPRITE_BASE + i, flag);

    if (is_flip)
    {
        for (i = 0; i < PLAYER_SPRITE_SIZE / 2; ++i)
            set_sprite_tile(PLAYER_SPRITE_BASE + i, TILESET_MAY_AT(base_u + (PLAYER_SPRITE_SIZE / 2 - 1) - i, 0));
        for (i = 0; i < PLAYER_SPRITE_SIZE / 2; ++i)
            set_sprite_tile(PLAYER_SPRITE_BASE + (PLAYER_SPRITE_SIZE / 2) + i, TILESET_MAY_AT(base_u + (PLAYER_SPRITE_SIZE / 2 - 1) - i, 2));
    }
    else
    {
        for (i = 0; i < PLAYER_SPRITE_SIZE / 2; ++i)
            set_sprite_tile(PLAYER_SPRITE_BASE + i, TILESET_MAY_AT(base_u + i, 0));
        for (i = 0; i < PLAYER_SPRITE_SIZE / 2; ++i)
            set_sprite_tile(PLAYER_SPRITE_BASE + (PLAYER_SPRITE_SIZE / 2) + i, TILESET_MAY_AT(base_u + i, 2));
    }
}

void anim_player_down(UINT8 frame)
{
    if (frame == 0)
    {
        fill_player_tiles(0, FALSE);
    }
    else if (frame == 1)
    {
        fill_player_tiles(3, FALSE);
    }
    else if (frame == 2)
    {
        fill_player_tiles(0, TRUE);
    }
    else
    {
        fill_player_tiles(3, TRUE);
    }
}

void anim_player_horizontal(UINT8 frame, BOOL is_left)
{
    if (frame == 0)
    {
        fill_player_tiles(6, is_left);
    }
    else if (frame == 1)
    {
        fill_player_tiles(9, is_left);
    }
    if (frame == 2)
    {
        fill_player_tiles(6, is_left);
    }
    else if (frame == 3)
    {
        fill_player_tiles(12, is_left);
    }
}

void main(void)
{
    UINT8 i, j;
    UINT8 keys = 0;
    UINT8 frame_skip = 8; // Update player's animation every 8 frame to
                          // slow down the animation (8 frames = ~133 ms
                          // between each animation frames)

    // Initialize player's state
    player_x = 80;
    player_y = 72;
    player_direction = DIR_DOWN;
    player_anim_frame = 0;
    is_player_walking = 0;

    // Load tiles in video memory
    set_bkg_data(0, TILESET_AREA_TILE_COUNT, (UINT8 *)TILESET_AREA);
    set_sprite_data(0, TILESET_MAY_TILE_COUNT, (UINT8 *)TILESET_MAY);

    for (i = 0; i < BG_BUFFER_W; ++i)
        for (j = 0; j < BG_BUFFER_H; ++j)
            bg_buffer[i][j] = TILESET_AREA_AT(0, 0);

    bg_buffer[3 + 0][3 + 0] = TILESET_AREA_AT(0, 2);
    bg_buffer[3 + 1][3 + 0] = TILESET_AREA_AT(1, 2);
    bg_buffer[3 + 0][3 + 1] = TILESET_AREA_AT(0, 3);
    bg_buffer[3 + 1][3 + 1] = TILESET_AREA_AT(1, 3);

    for (i = 0; i < BG_BUFFER_W; ++i)
    {
        set_bkg_tiles(i, 0, 1, BG_BUFFER_H, bg_buffer[i]);
    }
    // set_bkg_tiles(0, 0, TILEMAP_AREA_WIDTH, TILEMAP_AREA_HEIGHT, (UINT8 *)TILEMAP_AREA);

    SPRITES_8x16;
    SHOW_SPRITES;
    SHOW_BKG;

    move_sprite(PLAYER_SPRITE_BASE + 0, player_x, player_y);
    set_sprite_prop(PLAYER_SPRITE_BASE, S_PALETTE);

    move_sprite(PLAYER_SPRITE_BASE + 1, player_x + 8, player_y);
    set_sprite_prop(PLAYER_SPRITE_BASE + 1, S_PALETTE);

    move_sprite(PLAYER_SPRITE_BASE + 2, player_x + 16, player_y);
    set_sprite_prop(PLAYER_SPRITE_BASE + 2, S_PALETTE);

    move_sprite(PLAYER_SPRITE_BASE + 3, player_x, player_y + 16);
    set_sprite_prop(PLAYER_SPRITE_BASE + 3, S_PALETTE);

    move_sprite(PLAYER_SPRITE_BASE + 4, player_x + 8, player_y + 16);
    set_sprite_prop(PLAYER_SPRITE_BASE + 4, S_PALETTE);

    move_sprite(PLAYER_SPRITE_BASE + 5, player_x + 16, player_y + 16);
    set_sprite_prop(PLAYER_SPRITE_BASE + 5, S_PALETTE);

    // デバッグ用
    for (i = 0; i < 8; ++i)
    {
        set_sprite_prop(i + 2, S_PALETTE);
        move_sprite(i + 2, 40 + i * 8, 40);
        set_sprite_tile(i + 2, TILESET_MAY_AT(i, 0));
    }

    while (1)
    {
        // Wait for v-blank (screen refresh)
        wait_vbl_done();

        // Read joypad keys to know if the player is walking
        // and in which direction
        keys = joypad();
        if (keys & J_UP)
        {
            player_direction = DIR_UP;
            is_player_walking = 1;
        }
        else if (keys & J_DOWN)
        {
            player_direction = DIR_DOWN;
            is_player_walking = 1;
        }
        else if (keys & J_LEFT)
        {
            player_direction = DIR_LEFT;
            is_player_walking = 1;
        }
        else if (keys & J_RIGHT)
        {
            player_direction = DIR_RIGHT;
            is_player_walking = 1;
        }
        else
        {
            is_player_walking = 0;
        }

        // Update the player position if it is walking

        if (is_player_walking)
        {
            if (player_direction == DIR_RIGHT)
                player_x += 1;
            else if (player_direction == DIR_LEFT)
                player_x -= 1;
            else if (player_direction == DIR_UP)
                player_y -= 1;
            else if (player_direction == DIR_DOWN)
                player_y += 1;
        }
        move_sprite(PLAYER_SPRITE_BASE + 0, player_x, player_y);
        move_sprite(PLAYER_SPRITE_BASE + 1, player_x + 8, player_y);
        move_sprite(PLAYER_SPRITE_BASE + 2, player_x + 16, player_y);
        move_sprite(PLAYER_SPRITE_BASE + 3, player_x, player_y + 16);
        move_sprite(PLAYER_SPRITE_BASE + 4, player_x + 8, player_y + 16);
        move_sprite(PLAYER_SPRITE_BASE + 5, player_x + 16, player_y + 16);

        // We do not update the animation on each frame: the animation
        // will be too quick. So we skip frames
        frame_skip -= 1;
        if (!frame_skip)
        {
            frame_skip = 20;
        }
        else
        {
            continue;
        }

        if (player_direction != DIR_UP)
        {
            player_anim_dir = player_direction;
        }

        if (player_anim_dir == DIR_LEFT)
        {
            anim_player_horizontal(player_anim_frame, TRUE);
        }
        else if (player_anim_dir == DIR_RIGHT)
        {
            anim_player_horizontal(player_anim_frame, FALSE);
        }
        else
        {
            anim_player_down(player_anim_frame);
        }

        if ((++player_anim_frame) >= 4)
            player_anim_frame = 0;
    }
}

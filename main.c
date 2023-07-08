// Original Source: https://github.com/flozz/gameboy-examples/tree/master

#define INT_16_BITS

#include <types.h>
#include <gb/gb.h>
#include "tileset_may.h"

#define PLAYER_SPRITE_TILE_COUNT 48

// clang-format on
#define PLAYER_SPRITE_MAP_W 6
#define PLAYER_SPRITE_MAP_AT(x, y) ((x * 2) + (y / 2) * (PLAYER_SPRITE_MAP_W * 2))

// Id ("nb") of the two sprites used to represent the player
#define PLAYER_SPRITE_BASE_ID 0

// Offset of the animation of each direction in the global animation data
#define PLAYER_DIRECTION_DOWN 0
#define PLAYER_DIRECTION_UP 6
#define PLAYER_DIRECTION_RIGHT 12
#define PLAYER_DIRECTION_LEFT 18

// Variables containing player state
UINT8 player_x;
UINT8 player_y;
UINT8 player_direction;
UINT8 player_animation_frame;
UINT8 is_player_walking;

// Flip the given sprite on X axis.
//
// sprite_id: the id ("nb") of the sprite to update.
void flip_sprite_horiz(UINT8 sprite_id)
{
    set_sprite_prop(sprite_id, get_sprite_prop(sprite_id) | S_FLIPX);
}

// Remove the flip the given sprite on X axis.
//
// sprite_id: the id ("nb") of the sprite to update.
void unflip_sprite_horiz(UINT8 sprite_id)
{
    set_sprite_prop(sprite_id, get_sprite_prop(sprite_id) & ~S_FLIPX);
}

// Update the tiles of the sprite to animate it.
//
// sprite_id: the id ("nb") of the sprite to update
// anim:      pointer to the animation data
// direction: direction of the animation (= offset of the animation in the global animation data)
// frame:     the new frame of the animation that will be displayed
//
// Returns the next frame of the animation.
UINT8 update_sprite_animation(UINT8 sprite_id, UINT8 *anim, UINT8 direction, UINT8 frame)
{
    UINT8 len = anim[direction];
    UINT8 flip = anim[direction + 1];
    UINT8 tile_id = anim[direction + 2 + frame];

    if (flip)
    {
        flip_sprite_horiz(sprite_id);
    }
    else
    {
        unflip_sprite_horiz(sprite_id);
    }

    set_sprite_tile(sprite_id, tile_id);

    return (frame + 1) % len;
}

void main(void)
{
    UINT8 i = 0;
    UINT8 keys = 0;
    UINT8 frame_skip = 8; // Update player's animation every 8 frame to
                          // slow down the animation (8 frames = ~133 ms
                          // between each animation frames)

    // Initialize player's state
    player_x = 80;
    player_y = 72;
    player_direction = PLAYER_DIRECTION_DOWN;
    player_animation_frame = 0;
    is_player_walking = 0;

    // Load sprites' tiles in video memory
    set_sprite_data(0, PLAYER_SPRITE_TILE_COUNT, (UINT8 *)TILESET_MAY);

    // Use 8x16 sprites
    SPRITES_8x16;

    // Makes sprites "layer" visible
    SHOW_SPRITES;

    move_sprite(PLAYER_SPRITE_BASE_ID + 0, player_x, player_y);
    set_sprite_prop(PLAYER_SPRITE_BASE_ID, S_PALETTE);

    move_sprite(PLAYER_SPRITE_BASE_ID + 1, player_x + 8, player_y);
    set_sprite_prop(PLAYER_SPRITE_BASE_ID + 1, S_PALETTE);

    move_sprite(PLAYER_SPRITE_BASE_ID + 2, player_x + 16, player_y);
    set_sprite_prop(PLAYER_SPRITE_BASE_ID + 2, S_PALETTE);

    move_sprite(PLAYER_SPRITE_BASE_ID + 3, player_x, player_y + 16);
    set_sprite_prop(PLAYER_SPRITE_BASE_ID + 3, S_PALETTE);

    move_sprite(PLAYER_SPRITE_BASE_ID + 4, player_x + 8, player_y + 16);
    set_sprite_prop(PLAYER_SPRITE_BASE_ID + 4, S_PALETTE);

    move_sprite(PLAYER_SPRITE_BASE_ID + 5, player_x + 16, player_y + 16);
    set_sprite_prop(PLAYER_SPRITE_BASE_ID + 5, S_PALETTE);

    // デバッグ用
    for (i = 0; i < 8; ++i)
    {
        set_sprite_prop(i + 2, S_PALETTE);
        move_sprite(i + 2, 40 + i * 8, 40);
        set_sprite_tile(i + 2, PLAYER_SPRITE_MAP_AT(i, 0));
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
            player_direction = PLAYER_DIRECTION_UP;
            is_player_walking = 1;
        }
        else if (keys & J_DOWN)
        {
            player_direction = PLAYER_DIRECTION_DOWN;
            is_player_walking = 1;
        }
        else if (keys & J_LEFT)
        {
            player_direction = PLAYER_DIRECTION_LEFT;
            is_player_walking = 1;
        }
        else if (keys & J_RIGHT)
        {
            player_direction = PLAYER_DIRECTION_RIGHT;
            is_player_walking = 1;
        }
        else
        {
            is_player_walking = 0;
        }

        // Update the player position if it is walking

        if (is_player_walking)
        {
            if (player_direction == PLAYER_DIRECTION_RIGHT)
                player_x += 1;
            else if (player_direction == PLAYER_DIRECTION_LEFT)
                player_x -= 1;
            else if (player_direction == PLAYER_DIRECTION_UP)
                player_y -= 1;
            else if (player_direction == PLAYER_DIRECTION_DOWN)
                player_y += 1;
        }
        move_sprite(PLAYER_SPRITE_BASE_ID + 0, player_x, player_y);
        move_sprite(PLAYER_SPRITE_BASE_ID + 1, player_x + 8, player_y);
        move_sprite(PLAYER_SPRITE_BASE_ID + 2, player_x + 16, player_y);
        move_sprite(PLAYER_SPRITE_BASE_ID + 3, player_x, player_y + 16);
        move_sprite(PLAYER_SPRITE_BASE_ID + 4, player_x + 8, player_y + 16);
        move_sprite(PLAYER_SPRITE_BASE_ID + 5, player_x + 16, player_y + 16);

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

        if (player_animation_frame == 0)
        {
            set_sprite_prop(PLAYER_SPRITE_BASE_ID + 0, get_sprite_prop(PLAYER_SPRITE_BASE_ID + 0) & ~S_FLIPX);
            set_sprite_prop(PLAYER_SPRITE_BASE_ID + 1, get_sprite_prop(PLAYER_SPRITE_BASE_ID + 1) & ~S_FLIPX);
            set_sprite_prop(PLAYER_SPRITE_BASE_ID + 2, get_sprite_prop(PLAYER_SPRITE_BASE_ID + 2) & ~S_FLIPX);
            set_sprite_prop(PLAYER_SPRITE_BASE_ID + 3, get_sprite_prop(PLAYER_SPRITE_BASE_ID + 3) & ~S_FLIPX);
            set_sprite_prop(PLAYER_SPRITE_BASE_ID + 4, get_sprite_prop(PLAYER_SPRITE_BASE_ID + 4) & ~S_FLIPX);
            set_sprite_prop(PLAYER_SPRITE_BASE_ID + 5, get_sprite_prop(PLAYER_SPRITE_BASE_ID + 5) & ~S_FLIPX);

            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 0, PLAYER_SPRITE_MAP_AT(0, 0));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 1, PLAYER_SPRITE_MAP_AT(1, 0));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 2, PLAYER_SPRITE_MAP_AT(2, 0));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 3, PLAYER_SPRITE_MAP_AT(0, 2));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 4, PLAYER_SPRITE_MAP_AT(1, 2));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 5, PLAYER_SPRITE_MAP_AT(2, 2));
        }
        else if (player_animation_frame == 1)
        {
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 0, PLAYER_SPRITE_MAP_AT(3, 0));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 1, PLAYER_SPRITE_MAP_AT(4, 0));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 2, PLAYER_SPRITE_MAP_AT(5, 0));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 3, PLAYER_SPRITE_MAP_AT(3, 2));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 4, PLAYER_SPRITE_MAP_AT(4, 2));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 5, PLAYER_SPRITE_MAP_AT(5, 2));
        }
        if (player_animation_frame == 2)
        {
            set_sprite_prop(PLAYER_SPRITE_BASE_ID + 0, get_sprite_prop(PLAYER_SPRITE_BASE_ID + 0) | S_FLIPX);
            set_sprite_prop(PLAYER_SPRITE_BASE_ID + 1, get_sprite_prop(PLAYER_SPRITE_BASE_ID + 1) | S_FLIPX);
            set_sprite_prop(PLAYER_SPRITE_BASE_ID + 2, get_sprite_prop(PLAYER_SPRITE_BASE_ID + 2) | S_FLIPX);
            set_sprite_prop(PLAYER_SPRITE_BASE_ID + 3, get_sprite_prop(PLAYER_SPRITE_BASE_ID + 3) | S_FLIPX);
            set_sprite_prop(PLAYER_SPRITE_BASE_ID + 4, get_sprite_prop(PLAYER_SPRITE_BASE_ID + 4) | S_FLIPX);
            set_sprite_prop(PLAYER_SPRITE_BASE_ID + 5, get_sprite_prop(PLAYER_SPRITE_BASE_ID + 5) | S_FLIPX);

            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 0, PLAYER_SPRITE_MAP_AT(2, 0));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 1, PLAYER_SPRITE_MAP_AT(1, 0));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 2, PLAYER_SPRITE_MAP_AT(0, 0));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 3, PLAYER_SPRITE_MAP_AT(2, 2));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 4, PLAYER_SPRITE_MAP_AT(1, 2));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 5, PLAYER_SPRITE_MAP_AT(0, 2));
        }
        else if (player_animation_frame == 3)
        {
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 0, PLAYER_SPRITE_MAP_AT(5, 0));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 1, PLAYER_SPRITE_MAP_AT(4, 0));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 2, PLAYER_SPRITE_MAP_AT(3, 0));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 3, PLAYER_SPRITE_MAP_AT(5, 2));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 4, PLAYER_SPRITE_MAP_AT(4, 2));
            set_sprite_tile(PLAYER_SPRITE_BASE_ID + 5, PLAYER_SPRITE_MAP_AT(3, 2));
        }

        if ((++player_animation_frame) >= 4)
            player_animation_frame = 0;
    }
}

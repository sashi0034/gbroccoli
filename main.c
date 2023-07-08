// Original Source: https://github.com/flozz/gameboy-examples/tree/master

#define INT_16_BITS

#include <types.h>
#include <gb/gb.h>

#define PLAYER_SPRITE_TILE_COUNT 16

// clang-format off
UINT8 PLAYER_SPRITE[] = {
    0x10, 0x00, 0x28, 0x10, 0x3F, 0x00, 0x7F, 0x00, 0xC0, 0x3F, 0x9F, 0x60, 0xBF, 0x40, 0xA7, 0x58,
    0xBD, 0x42, 0xBA, 0x45, 0x9F, 0x60, 0xC0, 0x3F, 0x7F, 0x00, 0x3F, 0x00, 0x14, 0x08, 0x1C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xF2, 0x0C, 0x09, 0xF6, 0xE3, 0x1C, 0xF5, 0x0A, 0x33, 0xCC,
    0xF1, 0x0E, 0xF3, 0x0C, 0xE1, 0x1E, 0x03, 0xFC, 0xFE, 0x00, 0xFC, 0x00, 0x28, 0x10, 0x38, 0x00,
    0x08, 0x00, 0x14, 0x08, 0x3F, 0x00, 0x7F, 0x00, 0xC0, 0x3F, 0x9F, 0x60, 0xBF, 0x40, 0xB3, 0x4C,
    0xBE, 0x41, 0xBD, 0x42, 0x9F, 0x60, 0xC0, 0x3F, 0x7F, 0x00, 0x3F, 0x00, 0x14, 0x08, 0x1C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xF2, 0x0C, 0x09, 0xF6, 0xE3, 0x1C, 0xF5, 0x0A, 0x93, 0x6C,
    0xF1, 0x0E, 0x73, 0x8C, 0xE1, 0x1E, 0x03, 0xFC, 0xFE, 0x00, 0xFC, 0x00, 0x14, 0x08, 0x1C, 0x00,
    0x10, 0x00, 0x28, 0x10, 0x3F, 0x00, 0x7F, 0x00, 0xC0, 0x3F, 0x9F, 0x60, 0xBF, 0x40, 0xA7, 0x58,
    0xBD, 0x42, 0xBA, 0x45, 0x9F, 0x60, 0xC0, 0x3F, 0x7F, 0x00, 0x3F, 0x00, 0x14, 0x08, 0x1C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xF2, 0x0C, 0x09, 0xF6, 0xE3, 0x1C, 0xF5, 0x0A, 0x33, 0xCC,
    0xF1, 0x0E, 0xF3, 0x0C, 0xE1, 0x1E, 0x03, 0xFC, 0xFE, 0x00, 0xFC, 0x00, 0x14, 0x08, 0x1C, 0x00,
    0x00, 0x00, 0x10, 0x00, 0x28, 0x10, 0x3F, 0x00, 0x7F, 0x00, 0xC0, 0x3F, 0x9F, 0x60, 0xBF, 0x40,
    0xA7, 0x58, 0xBD, 0x42, 0xBA, 0x45, 0xDF, 0x20, 0x7F, 0x00, 0x3F, 0x00, 0x28, 0x10, 0x38, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xF2, 0x0C, 0x0B, 0xF4, 0xE5, 0x1A, 0xF3, 0x0C,
    0x31, 0xCE, 0xF3, 0x0C, 0xF1, 0x0E, 0xE3, 0x1C, 0xFE, 0x00, 0xFC, 0x00, 0x28, 0x10, 0x38, 0x00,
};

UINT8 PLAYER_SPRITE_MAP[] = {
    0x00, 0x02, 0x04, 0x06, 0x00, 0x02, 0x0C, 0x0E,
    0x01, 0x03, 0x05, 0x07, 0x01, 0x0B, 0x0D, 0x0F,
};
// clang-format on
#define PLAYER_SPRITE_MAP_W 8
#define PLAYER_SPRITE_MAP_AT(x, y) ((x * 2) + (y / 2) * (PLAYER_SPRITE_MAP_W * 2))

// Id ("nb") of the two sprites used to represent the player
#define PLAYER_SPRITE_L_ID 0
#define PLAYER_SPRITE_R_ID 1

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
    set_sprite_data(0, PLAYER_SPRITE_TILE_COUNT, PLAYER_SPRITE);

    // Use 8x16 sprites
    SPRITES_8x16;

    // Makes sprites "layer" visible
    SHOW_SPRITES;

    // Init the two sprites used for the player
    move_sprite(PLAYER_SPRITE_L_ID, player_x, player_y);
    set_sprite_prop(PLAYER_SPRITE_L_ID, S_PALETTE);

    move_sprite(PLAYER_SPRITE_R_ID, player_x + 8, player_y);
    set_sprite_prop(PLAYER_SPRITE_R_ID, S_PALETTE);

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
            frame_skip = 1; // Force refresh of the animation frame
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
            move_sprite(PLAYER_SPRITE_L_ID, player_x, player_y);
            move_sprite(PLAYER_SPRITE_R_ID, player_x + 8, player_y);

            // We do not update the animation on each frame: the animation
            // will be too quick. So we skip frames
            frame_skip -= 1;
            if (!frame_skip)
            {
                frame_skip = 8;
            }
            else
            {
                continue;
            }
        }
        else
        {
            player_animation_frame = 0;
        }

        if ((++player_animation_frame) >= 4)
            player_animation_frame = 0;

        if (player_animation_frame == 0)
        {
            set_sprite_tile(PLAYER_SPRITE_L_ID, PLAYER_SPRITE_MAP_AT(0, 0));
            set_sprite_tile(PLAYER_SPRITE_R_ID, PLAYER_SPRITE_MAP_AT(1, 0));
        }
        else if (player_animation_frame == 1)
        {
            set_sprite_tile(PLAYER_SPRITE_L_ID, PLAYER_SPRITE_MAP_AT(2, 0));
            set_sprite_tile(PLAYER_SPRITE_R_ID, PLAYER_SPRITE_MAP_AT(3, 0));
        }
        else if (player_animation_frame == 2)
        {
            set_sprite_tile(PLAYER_SPRITE_L_ID, PLAYER_SPRITE_MAP_AT(4, 0));
            set_sprite_tile(PLAYER_SPRITE_R_ID, PLAYER_SPRITE_MAP_AT(5, 0));
        }
        else
        {
            set_sprite_tile(PLAYER_SPRITE_L_ID, PLAYER_SPRITE_MAP_AT(6, 0));
            set_sprite_tile(PLAYER_SPRITE_R_ID, PLAYER_SPRITE_MAP_AT(7, 0));
        }
    }
}

#ifndef UTIL_H
#define UTIL_H

#include <gb/gb.h>
#include <types.h>

#define BOOL UINT8

#define SCREEN_CHIP_W (160 / 8)
#define SCREEN_CHIP_H (144 / 8)

#define STAT_LY_EQ_LYC 0x40

#define SPR_GAP_X (8)
#define SPR_GAP_Y (16)

enum Direction4 { DIR_DOWN, DIR_UP, DIR_LEFT, DIR_RIGHT };

static void set_bgk_tile(UBYTE x, UBYTE y, unsigned char tile) {
    set_bkg_tiles(x, y, 1, 1, &tile);
}

static void set_win_tile(UBYTE x, UBYTE y, unsigned char tile) {
    set_win_tiles(x, y, 1, 1, &tile);
}

#define EXP_P2(pow2)                                                           \
    (((pow2) == 1)     ? 0                                                     \
     : ((pow2) == 2)   ? 1                                                     \
     : ((pow2) == 4)   ? 2                                                     \
     : ((pow2) == 8)   ? 3                                                     \
     : ((pow2) == 16)  ? 4                                                     \
     : ((pow2) == 32)  ? 5                                                     \
     : ((pow2) == 64)  ? 6                                                     \
     : ((pow2) == 128) ? 7                                                     \
                       : 0)

#define MUL_P2(x, pow2) ((x) << (EXP_P2((pow2))))
#define DIV_P2(x, pow2) ((x) >> (EXP_P2((pow2))))
#define MOD_P2(x, pow2) ((x) & (2 * EXP_P2((pow2)) - 1))

#endif // UTIL_H

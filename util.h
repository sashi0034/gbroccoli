#ifndef UTIL_H
#define UTIL_H

#define BOOL UINT8

#define SCREEN_CHIP_W (160 / 8)
#define SCREEN_CHIP_H (144 / 8)

#define SPR_GAP_X (8)
#define SPR_GAP_Y (16)

enum Direction4 { DIR_DOWN, DIR_UP, DIR_LEFT, DIR_RIGHT };

void set_bgk_tile(UBYTE x, UBYTE y, UBYTE tile) {
    set_bkg_tiles(x, y, 1, 1, &tile);
}

void set_win_tile(UBYTE x, UBYTE y, UBYTE tile) {
    set_win_tiles(x, y, 1, 1, &tile);
}

#define EXP_P2(x)                                                              \
    (((x) == 1)     ? 0                                                        \
     : ((x) == 2)   ? 1                                                        \
     : ((x) == 4)   ? 2                                                        \
     : ((x) == 8)   ? 3                                                        \
     : ((x) == 16)  ? 4                                                        \
     : ((x) == 32)  ? 5                                                        \
     : ((x) == 64)  ? 6                                                        \
     : ((x) == 128) ? 7                                                        \
                    : 0)

#define MUL_P2(x, pow2) ((x) << (EXP_P2((pow2))))
#define DIV_P2(x, pow2) ((x) >> (EXP_P2((pow2))))
#define MOD_P2(x, pow2) ((x) & (EXP_P2((pow2))))

#endif // UTIL_H

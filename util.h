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

#endif // UTIL_H

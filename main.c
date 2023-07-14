// reference: https://github.com/flozz/gameboy-examples/tree/master

#define INT_16_BITS

#include <gb/gb.h>
#include <types.h>

#include "player.h"
#include "playing.h"
#include "tilemap_area.h"
#include "tileset_area.h"
#include "tileset_infos.h"
#include "tileset_may.h"

#define TO_PALETTE(c0, c1, c2, c3) c0 | c1 << 2 | c2 << 4 | c3 << 6

// Variables global state
Player g_player;

void set_bkg_area_tile16(UBYTE x, UBYTE y, UBYTE u, UBYTE v) {
    UINT8 i, j, tile;

    for (i = 0; i < 2; ++i)
        for (j = 0; j < 2; ++j) {
            tile = TILESET_AREA_AT(u + i, v + j);
            set_bkg_tiles(x + i, y + j, 1, 1, &tile);
        }
}

void interrupt_lcd() {
    // Window以下はスプライトを非表示にする
    HIDE_SPRITES;
}

void main(void) {
    UINT8 i, j;

    // Initialize player's state
    reset_player(&g_player);

    // Load tiles in video memory
    set_sprite_data(0, TILESET_MAY_TILE_COUNT, (UINT8 *)TILESET_MAY);
    set_bkg_data(0, TILESET_AREA_TILE_COUNT, (UINT8 *)TILESET_AREA);
    set_win_data(TILESET_INFOS_TILE_BASE, TILESET_INFOS_TILE_COUNT,
                 (UINT8 *)TILESET_INFOS);

    for (i = 0; i < SCREEN_CHIP_W; i += 2)
        for (j = 0; j < SCREEN_CHIP_H; j += 2)
            set_bkg_area_tile16(i, j, 0, 0);

    // 木
    for (i = 1; i < SCREEN_CHIP_W; i += 2 * 2)
        for (j = 0; j < SCREEN_CHIP_H; j += 2 * 2)
            set_bkg_area_tile16(i, j, 0, 2);

    // 雑草など
    for (i = 3; i < SCREEN_CHIP_W - 1; i += 2 * 2)
        for (j = 2; j < SCREEN_CHIP_H - 1; j += 2 * 2)
            set_bkg_area_tile16(i, j, 2 + 2 * ((i + j) % 3), 0);

    for (i = 0; i < SCREEN_CHIP_W; ++i)
        for (j = 0; j < 2; ++j)
            set_win_tile(i, j, TILESET_INFOS_AT(0, 1));

    // テキスト
    set_win_tile(1, 0, TILESET_INFOS_AT(1, 1)); // す
    set_win_tile(2, 0, TILESET_INFOS_AT(2, 1)); // こ
    set_win_tile(3, 0, TILESET_INFOS_AT(3, 1)); // あ
    set_win_tile(4, 0, TILESET_INFOS_AT(4, 1)); // →
    set_win_tile(5, 0, TILESET_INFOS_AT(0, 0));
    set_win_tile(6, 0, TILESET_INFOS_AT(5, 1)); // て
    set_win_tile(7, 0, TILESET_INFOS_AT(6, 1)); // ん

    set_win_tile(14, 0, TILESET_INFOS_AT(7, 1)); // れ
    set_win_tile(15, 0, TILESET_INFOS_AT(8, 1)); // べ
    set_win_tile(16, 0, TILESET_INFOS_AT(9, 1)); // る
    set_win_tile(17, 0, TILESET_INFOS_AT(4, 1)); // →
    set_win_tile(18, 0, TILESET_INFOS_AT(1, 0));

    // ちょっとだけBGを上にずらす
    scroll_bkg(0, AREA_PADDING_Y);
    move_win(7, 136 - AREA_PADDING_Y);

    SPRITES_8x16;
    SHOW_SPRITES;
    SHOW_BKG;
    SHOW_WIN;

    OBP0_REG = TO_PALETTE(0, 3, 2, 1);
    OBP1_REG = TO_PALETTE(0, 3, 2, 0);

    // LY=LYC割り込みの設定
    STAT_REG = STAT_LY_EQ_LYC;
    LYC_REG = 136 - AREA_PADDING_Y;
    set_interrupts(VBL_IFLAG | LCD_IFLAG);
    add_LCD(interrupt_lcd);

    while (1) {
        // Wait for v-blank (screen refresh)
        wait_vbl_done();
        SHOW_SPRITES;

        update_player(&g_player);
    }
}

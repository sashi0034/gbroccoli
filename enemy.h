
#include "playing.h"
#include "tileset_crib.h"

typedef struct {
    BOOL is_using;
    UINT8 spr_base;
    UINT8 anim_timer;
    UINT8 anim_frame;
    MovableTrait movable;
} Enemy;

typedef struct {
    UINT8 timer;
    Enemy enemies[ENEMY_SPR_COUNT_4];
} EnemyManager;

void fill_enemy_tiles(UINT8 base_spr, UINT8 base_u, BOOL is_flip) {
    if (is_flip) {
        set_sprite_prop(base_spr + 0, S_PALETTE | S_FLIPX);
        set_sprite_prop(base_spr + 1, S_PALETTE | S_FLIPX);
        set_sprite_tile(base_spr + 0, TILESET_CRIB_AT(base_u + 1, 0));
        set_sprite_tile(base_spr + 1, TILESET_CRIB_AT(base_u, 1));
    } else {
        set_sprite_prop(base_spr + 0, S_PALETTE);
        set_sprite_prop(base_spr + 1, S_PALETTE);
        set_sprite_tile(base_spr + 0, TILESET_CRIB_AT(base_u, 0));
        set_sprite_tile(base_spr + 1, TILESET_CRIB_AT(base_u + 1, 1));
    }
}

void locate_enemy(UINT8 base_spr, UINT8 x, UINT8 y) {
    x += SPR_GAP_X;
    y += SPR_GAP_Y - 16 - AREA_PADDING_Y;

    move_sprite(base_spr + 0, x, y);
    move_sprite(base_spr + 1, x + 8, y);
}

void update_enemy(Enemy *enemy) {
    if (enemy->is_using == FALSE)
        return;

    // 移動処理
    if (move_movable(&enemy->movable)) {
        locate_enemy(enemy->spr_base, enemy->movable.x, enemy->movable.y);
        // 止まったら解放
    } else if (is_movable_stopped(&(enemy->movable))) {
        enemy->is_using = FALSE;
        return;
    }
}

void pop_enemy(Enemy *enemy) {
    enemy->is_using = TRUE;

    reset_movable(&enemy->movable, 1, 2);
    enemy->movable.x = -16;
    enemy->movable.y = 64;
    enemy->movable.dx = 96;

    locate_enemy(enemy->spr_base, enemy->movable.x, enemy->movable.y);
    fill_enemy_tiles(enemy->spr_base, 0, FALSE);
}

void reset_enemy_manager(EnemyManager *manager) {
    UINT8 i;
    for (i = 0; i < ENEMY_SPR_COUNT_4; ++i) {
        manager->enemies[i].is_using = FALSE;
        manager->enemies[i].spr_base = ENEMY_SPR_BASE + ENEMY_SPR_COUNT_4 * i;
    }
}

void update_enemy_manger(EnemyManager *manager) {
    UINT8 i;
    manager->timer++;

    // スポーン
    if (manager->timer == 120) {
        manager->timer = 0;

        for (i = 0; i < ENEMY_SPR_COUNT_4; ++i) {
            if (manager->enemies[i].is_using == FALSE) {
                pop_enemy(&manager->enemies[i]);
                break;
            }
        }
    }

    update_enemy(&manager->enemies[0]);
    update_enemy(&manager->enemies[1]);
    update_enemy(&manager->enemies[2]);
    update_enemy(&manager->enemies[3]); // ENEMY_SPR_COUNT_4 - 1
}

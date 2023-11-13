#include "playing.h"
#include "tileset_crib.h"

typedef struct {
    BOOL is_using;
    UINT8 spr_base;
    UINT8 anim_timer;
    UINT8 anim_frame;
    UINT8 dir;
    UINT8 remaining_move;
    MovableTrait movable;
} Enemy;

typedef struct {
    UINT8 timer;
    Enemy enemies[ENEMY_SPR_COUNT_4];
} EnemyManager;

EnemyManager s_enemy_manager;

static void fill_enemy_tiles(UINT8 base_spr, UINT8 base_u, BOOL is_flip) {
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

static void locate_enemy(UINT8 base_spr, UINT8 x, UINT8 y) {
    x += SPR_GAP_X;
    y += SPR_GAP_Y - 16 - AREA_PADDING_Y;

    move_sprite(base_spr + 0, x, y);
    move_sprite(base_spr + 1, x + 8, y);
}

static void set_enemy_move_delta(Enemy *enemy) {
    switch (enemy->dir) {
    case DIR_RIGHT:
        enemy->movable.dx = 32;
        break;
    case DIR_LEFT:
        enemy->movable.dx = -32;
        break;
    case DIR_DOWN:
        enemy->movable.dy = 32;
        break;
    case DIR_UP:
        enemy->movable.dy = -32;
        break;
    default:
        break;
    }
}

void update_enemy(Enemy *enemy) {
    if (enemy->is_using == FALSE)
        return;

    if (move_movable(&enemy->movable)) {
        // 移動処理
        locate_enemy(enemy->spr_base, enemy->movable.x, enemy->movable.y);
    } else if (is_movable_stopped(&(enemy->movable))) {
        // 止まったら解放
        enemy->remaining_move--;
        if (enemy->remaining_move > 0) {
            set_enemy_move_delta(enemy);
        } else {
            enemy->is_using = FALSE;
        }
    }

    enemy->anim_timer++;
    if ((enemy->anim_timer & 15) == 0) {
        enemy->anim_frame = (enemy->anim_frame + 1) & 3;
        fill_enemy_tiles(enemy->spr_base, MUL_P2(enemy->anim_frame >> 1, 2),
                         enemy->anim_frame & 1);
    }
}

static void pop_enemy(Enemy *enemy) {
    UINT8 dir;
    enemy->is_using = TRUE;

    reset_movable(&enemy->movable, 1, 2);

    dir = get_rand() & 3;
    enemy->dir = dir;
    switch (dir) {
    case DIR_UP:
        enemy->remaining_move = 6;
        enemy->movable.x = 24 + MUL_P2(get_rand() & 3, 32);
        enemy->movable.y = SCH_144 + 8;
        break;
    case DIR_DOWN:
        enemy->remaining_move = 6;
        enemy->movable.x = 24 + MUL_P2(get_rand() & 3, 32);
        enemy->movable.y = 0;
        break;
    case DIR_RIGHT:
        enemy->remaining_move = 7;
        enemy->movable.x = -24;
        enemy->movable.y = 32 + MUL_P2(get_rand() & 3, 32);
        break;
    case DIR_LEFT:
        enemy->remaining_move = 7;
        enemy->movable.x = SCW_160;
        enemy->movable.y = 32 + MUL_P2(get_rand() & 3, 32);
        break;
    default:
        break;
    }

    locate_enemy(enemy->spr_base, enemy->movable.x, enemy->movable.y);
    fill_enemy_tiles(enemy->spr_base, 0, FALSE);
}

void reset_enemy_manager() {
    EnemyManager *manager = &s_enemy_manager;
    UINT8 i;
    for (i = 0; i < ENEMY_SPR_COUNT_4; ++i) {
        manager->enemies[i].is_using = FALSE;
        manager->enemies[i].spr_base = ENEMY_SPR_BASE + ENEMY_SPR_COUNT_4 * i;
    }
}

void update_enemy_manger() {
    EnemyManager *manager = &s_enemy_manager;
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

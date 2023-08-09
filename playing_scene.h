#ifndef PLAYING_SCENE_H
#define PLAYING_SCENE_H

#include "enemy.h"
#include "player.h"

typedef struct {
    Player player;
    EnemyManager enemy_manager;
} PlayingScene;

void reset_playing_scene(PlayingScene *scene) {
    reset_player(&scene->player);
    reset_enemy_manager(&scene->enemy_manager);
}

void update_playing_scene(PlayingScene *scene) {
    update_player(&scene->player);
    update_enemy_manger(&scene->enemy_manager);
}

#endif // PLAYING_SCENE_H

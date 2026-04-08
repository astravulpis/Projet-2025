#ifndef GAME_H_
#define GAME_H_

#include "bullets.h"
#include "common.h"
#include "gui.h"
#include "level.h"
#include "music.h"
#include "player.h"

typedef struct {
    sdl_ctx_t *sdl_ctx;
    level_t **levels;
    size_t level_count;
    size_t loadedLevelIdx;
    player_t *player;
    bullets bullet_arr;
    sfxs audios;

    gui_menu **menus;
    size_t menu_count;
} gameContext;

#define LEVEL_COUNT 5

void closeGame(gameContext *ctx);
level_t *getLoadedLevel(gameContext *ctx);
room_t *beginLevel(int argc, char **argv, gameContext *ctx);
bool addMenu(gameContext *ctx, gui_menu *menu, menu_kind kind);
bool gameLoop(gameContext *ctx, int argc, char **argv);

#endif // GAME_H_

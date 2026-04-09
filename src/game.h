#ifndef GAME_H_
#define GAME_H_

#include "bullets.h"
#include "common.h"
#include "gui.h"
#include "guns.h"
#include "level.h"
#include "player.h"

/**
 * @typedef struct gameContext
 * @brief contains all the game context variables, and by all i do mean all, from the SDL context itself to the player and bullets
 */
typedef struct {
    sdl_ctx_t *sdl_ctx;
    level_t **levels;
    size_t level_count;
    size_t loadedLevelIdx;
    player_t *player;
    bullets bullet_arr;
    Guns_t *guns;
    gui_menu **menus;
    size_t menu_count;
} gameContext;

#define LEVEL_COUNT 5

/**
 * @fn closeGame(gameContext *ctx)
 * @param[in] ctx our context variable
 * @brief destroys every single object that needs to be destroyed and closes the game
 */
void closeGame(gameContext *ctx);

/**
 * @fn getLoadedLevel(gameContext *ctx)
 * @param[in] ctx our context variable
 * @brief returns the currently loaded level
 */
level_t *getLoadedLevel(gameContext *ctx);

/**
 * @fn beginLevel(int argc, char **argv, gameContext *ctx)
 * @param[in] argc amount of variables
 * @param[in] argv list of function arguments
 * @param[in] ctx our context variable
 * @brief begins the level found in ctx
 */
room_t *beginLevel(int argc, char **argv, gameContext *ctx);

/**
 * @fn loadAllLevels(gameContext *ctx)
 * @param[in] ctx the context of the game to load all the levels on the levels tab
 * @brief load all of the levels (not render)
 */
bool loadAllLevels(gameContext *ctx);

/**
 * @fn addMenu(gameContext *ctx, gui_menu *menu, menu_kind kind)
 * @param[in] ctx our context variable
 * @param[in] menu menu structure
 * @param[in] kind type of menu
 * @brief returns the currently loaded level
 */
bool addMenu(gameContext *ctx, gui_menu *menu, menu_kind kind);

/**
 * @fn gameLoop(gameContext *ctx, int argc, char **argv)
 * @param[in] ctx our context variable
 * @param[in] argc amount of variables
 * @param[in] argv list of function arguments
 * @brief main game loop with every single relevant function call
 */
bool gameLoop(gameContext *ctx, int argc, char **argv);

#endif // GAME_H_

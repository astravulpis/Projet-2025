#ifndef GAME_H_
#define GAME_H_

#include "bullets.h"
#include "common.h"
#include "gui.h"
#include "guns.h"
#include "level.h"
#include "player.h"

typedef struct {
    V2f position;
    int currState;
    int prevState;
} mouseDevice;

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
    mouseDevice mouse;
} gameContext;

#define LEVEL_COUNT 5

/**
 * @fn initGameContext(gameContext **ctx)
 * @param[in] ctx our context variable
 * @brief initializes every single object that needs to be initialized
 */
bool initGameContext(gameContext **ctx);

/**
 * @fn closeGame(gameContext **ctx)
 * @param[in] ctx our context variable
 * @brief destroys every single object that needs to be destroyed and closes the game
 *
 * This first updates the JSON file with the player's new score.
 * Then it goes through the list of levels and destorys them with \ref destroyLevel.
 * Then it does the same thing with the menus by callingl \ref destroyMenu.
 * Then it destroys the guns with \ref destroyGuns.
 * Then it destorys the bullets array with \ref deleteBullets.
 * Then it destroys the player with \ref destroyPlayer.
 * Finally it closes the SDL context with \ref closeCtx.
 */
void closeGame(gameContext **ctx);

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
 *
 * Loads the level data by calling \ref parseFlag, then it allocates the \ref ctx->levels and memsets it.
 * After that it puts the newly loaded level in the the list of levels in the ctx and sets the currently loaded level Idx to the new level's ID.
 * Finally, it moves the playerr to the starting location of the new level with \ref movePlayer and returns the current room we are in.
 */
room_t *beginLevel(int argc, char **argv, gameContext *ctx);

/**
 * @fn loadAllLevels(gameContext *ctx)
 * @param[in] ctx the context of the game to load all the levels on the levels tab
 * @brief load all of the levels (not render)
 *
 * Goes through the list of levels we currently have (this is hard coded at the moment)
 * and gets their information with \ref parseFlag and updates the level_count and levels array found in the \ref ctx.
 */
bool loadAllLevels(gameContext *ctx);

/**
 * @fn addMenu(gameContext *ctx, gui_menu *menu, menu_kind kind)
 * @param[in] ctx our context variable
 * @param[in] menu menu structure
 * @param[in] kind type of menu
 * @brief adds a menu to the game context
 *
 * Allocates all the space needed for the menu structure in the ctx if it hasn't been done yet
 * and updates \ref ctx->menus and \ref ctx->menu_count.
 */
bool addMenu(gameContext *ctx, gui_menu *menu, menu_kind kind);

/**
 * @fn gameLoop(gameContext *ctx, int argc, char **argv)
 * @param[in] ctx our context variable
 * @param[in] argc amount of variables
 * @param[in] argv list of function arguments
 * @brief main game loop with every single relevant function call
 *
 * First initialises the important values for the game to work.
 * Then it creates the SDL context with \ref createCtx.
 * After that recuperates the mouse position with \ref SDL_GetMouseState.
 * It disables the vsync with \ref disableVsync for more frames.
 * Then it creates the player with \ref createPlayer.
 * Then it creates the boxes for the status barsa the bottom of the screen and adatps
 * them to the screen ration with \ref boxToScale.
 * Then it loads all the levels with \ref loadAllLevels.
 * Then it creates the status bars with \ref createPlayerStatusBar.
 * Then it loads the first level with \ref beginLevel.
 * Then it loads all the levels with \ref loadAllLevels.
 * Then it creates all the menus by calling \ref addMenu.
 * Then it creates all the guns for the player with \ref initialiseGuns.
 * Then it adds the background music with \ref playTrack.
 * Finally it enters the main game loop where we can find all the different
 * functions calls and updates for the game.
 */
bool gameLoop(gameContext *ctx, int argc, char **argv);

#endif // GAME_H_

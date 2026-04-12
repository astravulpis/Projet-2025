#ifndef GAME_H_
#define GAME_H_

#include "bullets.h"
#include "common.h"
#include "bars.h"
#include "gui.h"
#include "guns.h"
#include "level.h"
#include "player.h"

typedef struct {
    SDL_FRect footerBox;
    SDL_FRect currWeaponBox;
    SDL_FRect styleMeterBox;
    bar *hpBar;
    bar *dashBar1;
    bar *dashBar2;
    bar *dashBar3;
} footer_t;
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
    footer_t footer;
} gameContext;

#define LEVEL_COUNT 6

/**
 * @fn initGameContext(gameContext **ctx)
 * @param[in] ctx our context variable
 * @brief initializes every single object that needs to be initialized
 */
bool initGameContext(gameContext **gameCtx, int xs_sz, char **xs);

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
bool gameLoop(gameContext *ctx);

#endif // GAME_H_

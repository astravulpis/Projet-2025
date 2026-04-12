/**
 * @file event.c
 * @brief Implementation of the different event handler
 *
 * @author Liam B. <liam.berge72@gmail.com>
 * @date 2026-03-15
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Reeves Guillaume <greeves2306@gmail.com>
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#include "event.h"

/**
 * @fn basicKeyboardEvents(sdl_ctx_t *sdl_ctx)
 * @param[in] sdl_ctx sdl context variable
 * @brief tests for exit input
 *
 * for now this is just to quit the game but we might add menu buttons and such in the future unless we go with a new function?
 */
void basicKeyboardEvents(sdl_ctx_t *sdl_ctx, Guns_t *guns)
{
    const bool *state = SDL_GetKeyboardState(NULL);
    static bool prev[SDL_SCANCODE_COUNT] = {0};

    if (sdl_ctx->currMenu == NONE_MENU || sdl_ctx->currMenu == PAUSE_MENU) {
        if (isKeyPressed(SDL_SCANCODE_ESCAPE, state, prev)) {
            if (sdl_ctx->currMenu != NONE_MENU) {
                sdl_ctx->currMenu = NONE_MENU;
                MIX_ResumeAllTracks(sdl_ctx->mixer);
            } else {
                MIX_PauseAllTracks(sdl_ctx->mixer);
                sdl_ctx->currMenu = PAUSE_MENU;
            }
        }
    }

    // if (isKeyPressed(SDL_SCANCODE_H, state, prev)) {
    //         if (sdl_ctx->currMenu != LEVEL_SELECTION_MENU) { // on ne va pas rendre un le menu pause si on est déja dans un
    //         menu
    //             MIX_PauseAllTracks(sdl_ctx->mixer);
    //             sdl_ctx->currMenu = START_MENU;
    //         }
    //     }
    // }

    // if (isKeyPressed(SDL_SCANCODE_Q, state, prev)) {
    //     level->currentLoadedRoomID -= 1;
    //     if (level->currentLoadedRoomID > level->count)
    //         level->currentLoadedRoomID = level->count - 1; // Underflow since uint = 2^31-1
    //     movePlayer(player, getLoadedRoom(level)->startPos);
    // } else if (isKeyPressed(SDL_SCANCODE_E, state, prev)) {
    //     level->currentLoadedRoomID += 1;
    //     if (level->currentLoadedRoomID > level->count - 1) level->currentLoadedRoomID = 0;
    //     movePlayer(player, getLoadedRoom(level)->startPos);
    // }
    if (isKeyPressed(SDL_SCANCODE_1, state, prev)) setCurrentGun(guns, 0);
    if (isKeyPressed(SDL_SCANCODE_2, state, prev)) setCurrentGun(guns, 1);
    if (isKeyPressed(SDL_SCANCODE_3, state, prev)) setCurrentGun(guns, 2);
    if (isKeyPressed(SDL_SCANCODE_4, state, prev)) setCurrentGun(guns, 3);
    if (isKeyPressed(SDL_SCANCODE_5, state, prev)) setCurrentGun(guns, 4);
    if (isKeyPressed(SDL_SCANCODE_6, state, prev)) setCurrentGun(guns, 5);
    memcpy(prev, state, SDL_SCANCODE_COUNT);
}

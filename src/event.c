/**
 * @file event.c
 * @brief Implementation of the different event handler
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Date: 2026-03-15
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Reeves Guillaume <greeves2306@gmail.com>
 **/

#include "event.h"

/**
 * @fn basicKeyboardEvents(sdl_ctx_t *sdl_ctx)
 * @param[in] sdl_ctx sdl context variable
 * @brief tests for exit input
 *
 * for now this is just to quit the game but we might add menu buttons and such in the future unless we go with a new function?
 */
void basicKeyboardEvents(sdl_ctx_t *sdl_ctx, level_t *level, player_t *player, Guns_t * guns)
{
    const bool *state = SDL_GetKeyboardState(NULL);
    static bool prev[SDL_SCANCODE_COUNT] = {0};

    if (isKeyPressed(SDL_SCANCODE_ESCAPE, state, prev)) {
        if (sdl_ctx->currMenu != NONE_MENU) {
            sdl_ctx->currMenu = NONE_MENU;
            MIX_ResumeAllTracks(sdl_ctx->mixer);
        } else {
            MIX_PauseAllTracks(sdl_ctx->mixer);
            sdl_ctx->currMenu = PAUSE_MENU;
        }
    }
    if (isKeyPressed(SDL_SCANCODE_Q, state, prev)) {
        level->currentLoadedRoomID -= 1;
        if (level->currentLoadedRoomID > level->count)
            level->currentLoadedRoomID = level->count - 1; // Underflow since uint = 2^31-1
        movePlayer(player, getLoadedRoom(level)->startPos);
    } else if (isKeyPressed(SDL_SCANCODE_E, state, prev)) {
        level->currentLoadedRoomID += 1;
        if (level->currentLoadedRoomID > level->count - 1) level->currentLoadedRoomID = 0;
        movePlayer(player, getLoadedRoom(level)->startPos);
    }
    if (state[SDL_SCANCODE_1] && !prev[SDL_SCANCODE_1]) {
        setCurrentGun(guns, 0);
    }
    if (state[SDL_SCANCODE_2] && !prev[SDL_SCANCODE_2]) {
        setCurrentGun(guns, 1);
    }
    if (state[SDL_SCANCODE_3] && !prev[SDL_SCANCODE_3]) {
        setCurrentGun(guns, 2);
    }
    if (state[SDL_SCANCODE_4] && !prev[SDL_SCANCODE_4]) {
        setCurrentGun(guns, 3);
    }
    if (state[SDL_SCANCODE_5] && !prev[SDL_SCANCODE_5]) {
        setCurrentGun(guns, 4);
    }
    if (state[SDL_SCANCODE_6] && !prev[SDL_SCANCODE_6]) {
        setCurrentGun(guns, 5);
    }
    memcpy(prev, state, SDL_SCANCODE_COUNT);
}

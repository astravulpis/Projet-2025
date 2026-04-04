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
#include "level.h"
#include "player.h"

/**
 * @fn basicKeyboardEvents(sdl_ctx_t *sdl_ctx)
 * @param[in] sdl_ctx sdl context variable
 * @brief tests for exit input
 *
 * for now this is just to quit the game but we might add menu buttons and such in the future unless we go with a new function?
 */
void basicKeyboardEvents(sdl_ctx_t *sdl_ctx, level_t *level, player_t *player)
{
    const bool *state = SDL_GetKeyboardState(NULL);
    static bool prev[SDL_SCANCODE_COUNT] = {0};

    if (state[SDL_SCANCODE_ESCAPE] && !prev[SDL_SCANCODE_ESCAPE]) {
        sdl_ctx->paused = !sdl_ctx->paused;
    }
    if (state[SDL_SCANCODE_Q] && !prev[SDL_SCANCODE_Q]) {
        level->currentLoadedRoomID =
            (level->currentLoadedRoomID - 1 < 0) ? (int)(level->count - 1) : level->currentLoadedRoomID - 1;
        movePlayer(player, level->items[level->currentLoadedRoomID]->startPos);
    } else if (state[SDL_SCANCODE_E] && !prev[SDL_SCANCODE_E]) {
        level->currentLoadedRoomID =
            (level->currentLoadedRoomID + 1 > (int)(level->count - 1)) ? 0 : level->currentLoadedRoomID + 1;
        movePlayer(player, level->items[level->currentLoadedRoomID]->startPos);
    }
    memcpy(prev, state, SDL_SCANCODE_COUNT);
}

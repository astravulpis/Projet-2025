/**
 * @file event.c
 * @brief Implementation of the different event handler
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-03-15
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Reeves Guillaume <greeves2306@gmail.com>
 **/

#include "event.h"

// void resolveOverlap(SDL_FRect *a, SDL_FRect *b)
// {
//     // getting all the overlaps
//     float overlap_left = (a->x + a->w) - b->x;
//     float overlap_right = (b->x + b->w) - a->x;
//     float overlap_top = (a->y + a->h) - b->y;
//     float overlap_bottom = (b->y + b->h) - a->y;
//
//     // find smaller overlap to find the one to fix
//     float min_overlap = fminf(fminf(overlap_left, overlap_right), fminf(overlap_top, overlap_bottom));
//
//     if (min_overlap == overlap_left) {
//         // Resolve by moving left
//         a->x = b->x - a->w;
//     } else if (min_overlap == overlap_right) {
//         a->x = b->x + b->w; // moving right
//     } else if (min_overlap == overlap_top) {
//         a->y = b->y - a->h; // moving up
//     } else if (min_overlap == overlap_bottom) {
//         a->y = b->y + b->h; // moving down
//     }
// }


/**
 * @fn basicKeyboardEvents(sdl_ctx_t *sdl_ctx)
 * @param[in] sdl_ctx sdl context variable
 * @brief tests for exit input
 *
 * for now this is just to quit the game but we might add menu buttons and such in the future unless we go with a new function?
 */
void basicKeyboardEvents(sdl_ctx_t *sdl_ctx)
{
    while (SDL_PollEvent(&sdl_ctx->event)) {
        if (sdl_ctx->event.type == SDL_EVENT_KEY_DOWN) {
            switch (sdl_ctx->event.key.scancode) {
            case SDL_SCANCODE_Q:
                sdl_ctx->quit = true;
                break;
            }
        }
    }
}
/*void basicKeyboardEvents(sdl_ctx_t *sdl_ctx)
{
    const bool *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_Q]) {
        sdl_ctx->quit = true;
    }
    else if(state[SDL_SCANCODE_ESCAPE]) {
        if (sdl_ctx->pause == true){
            sdl_ctx->pause = false;
        }
        else{
            sdl_ctx->pause = true;
        }
    }
}*/

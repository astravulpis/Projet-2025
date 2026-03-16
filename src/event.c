#include "event.h"
#include <math.h>



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

void basicKeyboardEvents(sdl_ctx_t *sdl_ctx)
{
    SDL_Event e = sdl_ctx->event;
    if (e.type == SDL_EVENT_KEY_DOWN) {
        switch (e.key.scancode) {
        case SDL_SCANCODE_Q:
            sdl_ctx->quit = true;
            break;
        default:
            break;
        }
    }
}

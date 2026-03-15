#include "event.h"
#include <math.h>

// keep rectangle in the given bounds by window height and window width
void keepPlayerInbound(SDL_FRect *r, float minX, float minY, float maxX, float maxY)
{
    if (r == NULL) return;
    if (r->x < minX) r->x = minX;               // Left
    if (r->y < minY) r->y = minY;               // Down
    if (r->x + r->w > maxX) r->x = maxX - r->w; // Right
    if (r->y + r->h > maxY) r->y = maxY - r->h; // Up
}

int collision(SDL_FRect *a, SDL_FRect *b)
{
    return (a->x < b->x + b->w && a->x + a->w > b->x && a->y < b->y + b->h && a->y + a->h > b->y);
}

void basicMovementEvents(struct sdl_context_s *ctx, float *dx, float *dy, SDL_FRect *a, SDL_FRect *b)
{
    const bool *keyboard_state = SDL_GetKeyboardState(NULL);
    *dx = 0.0f;
    *dy = 0.0f;
    // looks for left and right
    if (keyboard_state[SDL_SCANCODE_A]) {
        *dx -= 1.0f;
    }
    if (keyboard_state[SDL_SCANCODE_D]) {
        *dx += 1.0f;
    }
    // looks for up and down
    if (keyboard_state[SDL_SCANCODE_W]) {
        *dy -= 1.0f;
    }
    if (keyboard_state[SDL_SCANCODE_S]) {
        *dy += 1.0f;
    }
    // checks the exit (Q for now since why not?)
    if (keyboard_state[SDL_SCANCODE_Q]) {
        ctx->quit = true;
    }
    // if we are overlapping we disregard the input and rectify the overalp
    if (collision(a, b)) {
        *dy = 0;
        *dx = 0;
    }
}

void resolveOverlap(SDL_FRect *a, SDL_FRect *b)
{
    // getting all the overlaps
    float overlap_left = (a->x + a->w) - b->x;
    float overlap_right = (b->x + b->w) - a->x;
    float overlap_top = (a->y + a->h) - b->y;
    float overlap_bottom = (b->y + b->h) - a->y;

    // find smaller overlap to find the one to fix
    float min_overlap = fminf(fminf(overlap_left, overlap_right), fminf(overlap_top, overlap_bottom));

    if (min_overlap == overlap_left) {
        // Resolve by moving left
        a->x = b->x - a->w;
    } else if (min_overlap == overlap_right) {
        a->x = b->x + b->w; // moving right
    } else if (min_overlap == overlap_top) {
        a->y = b->y - a->h; // moving up
    } else if (min_overlap == overlap_bottom) {
        a->y = b->y + b->h; // moving down
    }
}

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

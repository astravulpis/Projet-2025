#include "event.h"

// keep rectangle in the given bounds by window height and window width
void keep_player_inbound(SDL_FRect *r, float minX, float minY, float maxX, float maxY)
{
    if (r == NULL) return;
    if (r->x < minX) r->x = minX;               // Left
    if (r->y < minY) r->y = minY;               // Down
    if (r->x + r->w > maxX) r->x = maxX - r->w; // Right
    if (r->y + r->h > maxY) r->y = maxY - r->h; // Up
}

void basic_movement(struct sdl_context_s *ctx, float *dx, float *dy)
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
}

void basic_keyboard_events(sdl_ctx_t *sdl_ctx)
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

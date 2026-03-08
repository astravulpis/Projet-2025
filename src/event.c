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

int basic_movements(sdl_ctx_t *sdl_ctx)
{
    SDL_Event e = sdl_ctx->event;
    if (e.type == SDL_EVENT_KEY_DOWN) {
        switch (e.key.scancode) {
        case SDL_SCANCODE_A:
            return 'A';
            break;
        case SDL_SCANCODE_D:
            return 'D';
            break;
        case SDL_SCANCODE_W:
            return 'W';
            break;
        case SDL_SCANCODE_S:
            return 'S';
            break;
        default:
            break;
        }
    }
    return 0;
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

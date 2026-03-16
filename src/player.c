#include "player.h"
#include "common.h"
#include <math.h>

void UpdatePlayer(sdl_ctx_t *sdl_ctx, SDL_FRect *player, SDL_FRect *object, float deltaTime)
{
    float dx = 0;
    float dy = 0;
    float speed = 500.0f; // To be put into a player struct
    SDL_FRect temp = *player;

    basicMovementEvents(sdl_ctx, &dx, &dy);

    dx *= speed * deltaTime / sqrt(2);
    dy *= speed * deltaTime / sqrt(2);

    temp.x += dx;
    if (!SDL_HasRectIntersectionFloat(&temp, object)) {
        player->x += dx;
    }

    temp = *player;
    temp.y += dy;
    if (!SDL_HasRectIntersectionFloat(&temp, object)) {
        player->y += dy;
    }

    keepPlayerInbound(player, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void keepPlayerInbound(SDL_FRect *r, float minX, float minY, float maxX, float maxY)
{
    if (r == NULL) return;
    if (r->x < minX) r->x = minX;               // Left
    if (r->y < minY) r->y = minY;               // Down
    if (r->x + r->w > maxX) r->x = maxX - r->w; // Right
    if (r->y + r->h > maxY) r->y = maxY - r->h; // Up
}

void basicMovementEvents(struct sdl_context_s *ctx, float *dx, float *dy)
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
    // // if we are overlapping we disregard the input and rectify the overalp
    // if (collision(a, b)) {
    //     *dy = 0;
    //     *dx = 0;
    // }
}

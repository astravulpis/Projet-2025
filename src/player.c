#include "player.h"
#include "common.h"
#include "sdl_helpers.h"

/**
 * @fn keep_player_inbound(SDL_FRect *r, float minX, float minY, float maxX, float maxY)
 * @param[in] r The SDL rect given to check
 * @param[in] minX Minimum X bound
 * @param[in] minY Minimum Y bound
 * @param[in] maxX Maximum X bound
 * @param[in] maxY Maximum Y bound
 * @brief keep rectangle in the given bounds
 *
 * Takes the location of the rect we are moving then compares it to the bounds given.
 */
void keepPlayerInbound(SDL_FRect *r, float minX, float minY, float maxX, float maxY)
{
    if (r == NULL) return;
    if (r->x < minX) r->x = minX;               // Left
    if (r->y < minY) r->y = minY;               // Down
    if (r->x + r->w > maxX) r->x = maxX - r->w; // Right
    if (r->y + r->h > maxY) r->y = maxY - r->h; // Up
}

bool createPlayer(player_t **player, V2f playerSize, sdl_ctx_t **sdl_ctx, const char *path)
{
    bool result = true;
    (*player) = calloc(1, sizeof(player_t));
    if ((*player) == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocated space for a `player_t`", __FILE__, __LINE__);
        return_defer(false);
    }
    player_t *p = (*player);

    if (sdl_ctx == NULL) {
        nob_log(ERROR, "%s:%d: sdl_ctx is NULL. (how?)", __FILE__, __LINE__);
        return_defer(false);
    }

    p->ctx = sdl_ctx;
    p->tex = IMG_LoadTexture((*p->ctx)->renderer, path);
    p->boundingBox = createRect(0, 0, playerSize.x, playerSize.y);

    p->speed = 500.0f;
    p->jumpForce = -4.5f;
    p->velocity = (V2f){0.0f, 0.0f};
    p->onGround = true;

defer:
    return result;
}

void destroyPlayer(player_t **p)
{
    if (*p != NULL) {
        free((*p)->boundingBox);
        (*p)->boundingBox = NULL;

        SDL_DestroyTexture((*p)->tex);
        (*p)->boundingBox = NULL;
    }

    free(*p);
    (*p) = NULL;
}

void UpdatePlayer(player_t *p, SDL_FRect *object, float deltaTime)
{
    float dx = 0;
    float gravity = 9.2f;
    const Uint8 *keyboard_state = (Uint8 *)SDL_GetKeyboardState(NULL); // Not a bool. Just a bit-wise mask

    // Horizontal movement
    if (keyboard_state[SDL_SCANCODE_A]) dx -= p->speed * deltaTime;
    if (keyboard_state[SDL_SCANCODE_D]) dx += p->speed * deltaTime;

    // Vertical movement
    if (keyboard_state[SDL_SCANCODE_SPACE] && p->onGround) {
        p->velocity.y += p->jumpForce; // Up is towards negatives in SDL
        p->onGround = false;
    }

    p->velocity.y += gravity * deltaTime;
    float dy = p->velocity.y;

    SDL_FRect temp = *(p->boundingBox);
    temp.x += dx;
    if (!SDL_HasRectIntersectionFloat(&temp, object)) {
        p->boundingBox->x += dx;
    }

    // Vertical collision checking
    temp = *(p->boundingBox);
    temp.y += dy;
    if (SDL_HasRectIntersectionFloat(&temp, object)) {
        // THIS CONDITION IS A BIT TOO HACK-ISH MAY CHANGE IN THE FUTURE
        if (p->velocity.y > 0.0f && p->boundingBox->y + p->boundingBox->h <= object->y + object->h + 5.0f) {
            p->onGround = true;
            p->velocity.y = 0;
            p->boundingBox->y = object->y - p->boundingBox->h;
        } else if (p->velocity.y < 0) {
            p->velocity.y = 0;
        }
    } else {
        p->onGround = false;
        p->boundingBox->y += dy;
    }

    keepPlayerInbound(p->boundingBox, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

/**
 * @fn renderPlayer(player_t *p)
 * @param[in] p this is the player
 * @brief drenders the player image
 *
 * renders the player what more do you want?
 */
void renderPlayer(player_t *p)
{
    renderImage(*p->ctx, p->tex, p->boundingBox);
}

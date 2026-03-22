#include "player.h"
#include "SDL3/SDL_rect.h"
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

void movePlayer(player_t *p, V2f newPos)
{
    p->boundingBox->x = newPos.x;
    p->boundingBox->y = newPos.y;
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

    p->speed = 250.0f;
    p->jumpForce = -4.5f;
    p->velocity = (V2f){0.0f, 0.0f};
    p->onGround = false;

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

V2f inputUpdate(player_t *p, const float dt)
{
    V2f deltaPos = {0};
    const bool *keyboard_state = SDL_GetKeyboardState(NULL); // Not a bool. Just a bit-wise mask

    // Horizontal movement
    if (keyboard_state[SDL_SCANCODE_A]) deltaPos.x -= p->speed * dt;
    if (keyboard_state[SDL_SCANCODE_D]) deltaPos.x += p->speed * dt;
    if (keyboard_state[SDL_SCANCODE_S]) deltaPos.y += p->speed * dt;
    if (keyboard_state[SDL_SCANCODE_W]) deltaPos.y -= p->speed * dt;

    // Vertical movement
    if (keyboard_state[SDL_SCANCODE_SPACE] && p->onGround) {
        p->velocity.y += p->jumpForce; // Up is towards negatives in SDL
    }

    if (keyboard_state[SDL_SCANCODE_LCTRL] && !p->onGround) {
        p->velocity.y -= p->jumpForce; // Up is towards negatives in SDL
    }
    return deltaPos;
}

objs collision_test(player_t *p, objs *tiles)
{
    objs collisions = {0};
    da_foreach(obj, tile, tiles) {
        if (SDL_HasRectIntersectionFloat(getBB(p), getBB(tile))) {
            da_append(&collisions, *tile);
        }
    }

    return collisions;
}

void UpdatePlayer(player_t *p, objs *arr, float deltaTime)
{
    float gravity = 9.2f;
    SDL_FRect *rect = getBB(p);
    V2f movement = inputUpdate(p, deltaTime);
    V2f frame_movement = {movement.x + p->velocity.x, movement.y + p->velocity.y};
    p->onGround = false;

    rect->x += frame_movement.x;
    objs collisions = collision_test(p, arr);
    da_foreach(obj, it, &collisions) {
        SDL_FRect *tile = it->boundingBox;
        if (frame_movement.x > 0) {
            rect->x = Left(tile) - rect->w - 0.01f; // Set the player's right edge to the tile's left edge
        }
        if (frame_movement.x < 0) {
            rect->x = Right(tile) + 0.01f; // Set the player's left edge to the tile's right edge
        }
    }

    rect->y += frame_movement.y;
    collisions = collision_test(p, arr);
    da_foreach(obj, it, &collisions) {
        SDL_FRect *tile = it->boundingBox;
        if (frame_movement.y > 0) {
            rect->y = Top(tile) - rect->h - 0.01f; // Set the player's right edge to the tile's left edge
        }
        if (frame_movement.y < 0) {
            rect->y = Bottom(tile) + 0.01f; // Set the player's left edge to the tile's right edge
        }
    }

    // p->velocity.y = MIN(5, p->velocity.y + (gravity * deltaTime));

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

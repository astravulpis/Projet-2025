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
    p->jumpForce = -7.0f;
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

    // Vertical movement
    if (keyboard_state[SDL_SCANCODE_SPACE] && p->onGround) {
        p->velocity.y += p->jumpForce; // Up is towards negatives in SDL
        p->onGround = false;
    }

    if (keyboard_state[SDL_SCANCODE_LCTRL] && !p->onGround) {
        p->velocity.y -= p->jumpForce; // Up is towards negatives in SDL
    }
    return deltaPos;
}

void UpdatePlayer(player_t *p, SDL_FRect *object, float deltaTime)
{
    float gravity = 9.2f;
    V2f movement = inputUpdate(p, deltaTime);
    V2f frame_movement = {movement.x + p->velocity.x, movement.y + p->velocity.y};
    p->onGround = false;

    getBB(p)->x += frame_movement.x;
    SDL_FRect entity_rect = *getBB(p);
    if (SDL_HasRectIntersectionFloat(&entity_rect, object)) {
        if (Bottom(&entity_rect) <= Top(object) || Top(&entity_rect) >= Top(object)) {
            if (frame_movement.x > 0)  // Moving right
                entity_rect.x = Left(object) - entity_rect.w - 1.0f;
            if (frame_movement.x < 0)  // Moving left
                entity_rect.x = Right(object) + 1.0f;

            getBB(p)->x = entity_rect.x;
        }
    }

    getBB(p)->y += frame_movement.y;
    entity_rect = *getBB(p);
    if (SDL_HasRectIntersectionFloat(&entity_rect, object)) {
        if (frame_movement.y > 0) { // Moving down
            entity_rect.y = Top(object) - entity_rect.h;
            p->onGround = true;
            p->velocity.y = 0;
        }
        if (frame_movement.y < 0)  // Moving up
            entity_rect.y = Bottom(object);

        getBB(p)->y = entity_rect.y;
    }

    p->velocity.y = MIN(5, p->velocity.y + (gravity * deltaTime));

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

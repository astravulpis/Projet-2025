#include "player.h"
#include "common.h"
#include "sdl_helpers.h"
#include <math.h>

#define gravity 9.2f       // units: pixels / s^2 (tune as needed)
#define jumpForce -4.5f

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

/**
 * @fn basic_movements(struct sdl_context_s *ctx)
 * @brief Listens to keyboard inputs (specifically A and D)
 * @param[in] dx Delta x position for the next x position
 * @param[in] dy Delta y position for the next y position
 * @param[out] direction Returns the character pressed
 */
void basicMovementEvents(player_t *p, float *dx, float *dy, bool *jump)
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
    if (keyboard_state[SDL_SCANCODE_SPACE]) {
        *jump=true;
    }
    // if (keyboard_state[SDL_SCANCODE_E]) {
    //     p->stunnedTimer = 0.15f;
    // }
}

// bool updateStunned(player_t *p)
// {
//     float reduceAmount = p->stunnedTimer - p->stunnedTimer / 5.0f;
//     if (p->stunnedTimer > 0.0f) {
//         p->stunnedTimer = (reduceAmount <= 0.0f) ? 0.0f : reduceAmount;
//         return true;
//     }
//     p->stunnedTimer = 0.0f;
//     return false;
// }

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
    // result->stunned = false;
    p->stunnedTimer = 0.0f;

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

/**
 * @fn UpdatePlayer(player_t *p, SDL_FRect *objects, int object_count, float deltaTime)
 * @param[in] p pointer to the player
 * @param[in] objects array of objects that we will be going through to check collisions
 * @param[in] object_count so we know how many elements are in objects
 * @param[in] deltaTime this is so we buffer the next frame (I think?)
 * @brief updates the player position
 *
 * updates the player position and checks for collisions with all other textures on the map
 */
void UpdatePlayer(player_t *p, SDL_FRect *objects, int object_count, float deltaTime)
{
    // if (updateStunned(p)) return; // Player is currently stunned. No action can occur.

    float dx = 0;
    float dy = 0;
    bool jump = false;
    SDL_FRect temp = *(p->boundingBox);

    basicMovementEvents(p, &dx, &dy, &jump);

    dx *= p->speed * deltaTime / sqrt(2);
    dy *= p->speed * deltaTime / sqrt(2);

    float velocity;

    temp.x += dx;
    bool canMoveX = true;
    for (int i = 0; i < object_count; i++) {
        if (SDL_HasRectIntersectionFloat(&temp, &objects[i])) {
            canMoveX = false;
            break;
        }
    }
    if (canMoveX) {
        p->boundingBox->x += dx;
    }

    temp = *(p->boundingBox);
    temp.y += dy;
    bool canMoveY = true;
    for (int i = 0; i < object_count; i++) {
        if (SDL_HasRectIntersectionFloat(&temp, &objects[i])) {
            canMoveY = false;
            break;
        }
    }
    if (canMoveY) {
        p->boundingBox->y += dy;
    }

    if (jump==true) {
            p->boundingBox->y = jumpForce;
            printf("mewhen\n");
        }
    p->boundingBox->y += gravity * deltaTime;
    p->boundingBox->y += p->boundingBox->y;
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
/**
 * @file player.c
 * @brief Implementation of the different player related functions
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-03-16
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Reeves Guillaume <greeves2306@gmail.com>
 **/

#include "player.h"
#include "SDL3/SDL_scancode.h"
#include "common.h"

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
    memset(&p->audios, 0, sizeof(sfxs));

    // Initialising base values
    p->speed = 325.0f;
    p->jumpForce = -685.0f;
    p->velocity = (V2f){0.0f, 0.0f};
    p->onGround = false;
    p->stamina = 3.0f;
    p->lastKey = SDL_SCANCODE_UNKNOWN;
    p->hp = 100;
    p->flight = false;
    p->noclip = false;

    // Loading audios sfx
    loadSfx((*sdl_ctx), &p->audios, SFX_PLAYER_MOVE, "jump", "./assets/audio/SFX/jump.wav");
    loadSfx((*sdl_ctx), &p->audios, SFX_PLAYER_FALL, "softFall", "./assets/audio/SFX/softFall.wav");
    loadSfx((*sdl_ctx), &p->audios, SFX_PLAYER_FALL, "hardFall", "./assets/audio/SFX/hardFall.wav");
    loadSfx((*sdl_ctx), &p->audios, SFX_PLAYER_MOVE, "dash", "./assets/audio/SFX/dash.wav");
    loadSfx((*sdl_ctx), &p->audios, SFX_PLAYER_MOVE, "notEnoughStamina", "./assets/audio/SFX/notEnoughStamina.wav");
    loadSfx((*sdl_ctx), &p->audios, SFX_PLAYER_INTERACTIONS, "staminaRegen", "./assets/audio/SFX/staminaRegen.wav");

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

        destroySfxs(&(*p)->audios);
    }

    free(*p);
    (*p) = NULL;
}

bool isKeyPressed(SDL_Scancode scancode, const bool *currState, bool *prevState)
{
    bool result = currState[scancode] && !prevState[scancode];
    return result;
}

bool isKeyDown(SDL_Scancode scancode, const bool *currState)
{
    return currState[scancode];
}

V2f inputUpdate(player_t *p, const float dt)
{
    V2f deltaPos = {0};
    float dashSpeed = (p->speed * 1.65f) * dt;
    float wallJumpStrenght = (p->speed * 1.3f) * dt;
    const bool *currState = SDL_GetKeyboardState(NULL); // Not a bool. Just a bit-wise mask
    static bool prevState[SDL_SCANCODE_COUNT] = {0};

    // Flight & pass-through modes
    if (isKeyPressed(SDL_SCANCODE_LEFTBRACKET, currState, prevState)) {
        p->noclip = !p->noclip;
    }

    if (isKeyPressed(SDL_SCANCODE_RIGHTBRACKET, currState, prevState)) {
        p->flight = !p->flight;
    }

    // Vertical movement
    if (p->flight) {
        if (isKeyDown(SDL_SCANCODE_W, currState)) {
            deltaPos.y -= p->speed * dt;
        }
        if (isKeyDown(SDL_SCANCODE_S, currState)) {
            deltaPos.y += p->speed * dt;
        }
    }

    // Horizontal movement
    if (isKeyDown(SDL_SCANCODE_A, currState)) {
        deltaPos.x -= p->speed * dt;
        p->lastKey = SDL_SCANCODE_A;
    }

    if (isKeyDown(SDL_SCANCODE_D, currState)) {
        deltaPos.x += p->speed * dt;
        p->lastKey = SDL_SCANCODE_D;
    }

    // Dash
    if (isKeyPressed(SDL_SCANCODE_LSHIFT, currState, prevState)) {
        if (p->stamina >= 1.0f) {
            if (p->lastKey == SDL_SCANCODE_A) p->velocity.x = -dashSpeed;
            if (p->lastKey == SDL_SCANCODE_D || p->lastKey == SDL_SCANCODE_UNKNOWN) p->velocity.x = dashSpeed;
            p->stamina -= 1.0f;
            playSfx(*p->ctx, &p->audios, "dash");
        } else {
            playSfx(*p->ctx, &p->audios, "notEnoughStamina");
        }
    }

    // Jump & slam
    if (isKeyPressed(SDL_SCANCODE_SPACE, currState, prevState) && !prevState[SDL_SCANCODE_LCTRL]) {
        float temp = 1.35f;
        if (p->onWall) {
            if (p->lastKey == SDL_SCANCODE_A) {
                p->velocity.x = wallJumpStrenght * temp;
            } else if (p->lastKey == SDL_SCANCODE_D) {
                p->velocity.x = -wallJumpStrenght * temp;
            }
            p->velocity.y = -dashSpeed * temp;
        } else if (p->onGround) {
            p->velocity.y += p->jumpForce * dt; // Up is towards negatives in SDL
        }
        playSfx(*p->ctx, &p->audios, "jump");
    } else if ((isKeyPressed(SDL_SCANCODE_LCTRL, currState, prevState) && !p->onGround) && !p->isSlamming) {
        p->isSlamming = true;
        MIX_StopTrack((*p->ctx)->tracks[SFX_PLAYER_MOVE], 5);
    }

    if (p->isSlamming) {
        p->velocity.y -= (p->jumpForce * 2) * dt; // Up is towards negatives in SDL
    }

    memcpy(prevState, currState, SDL_SCANCODE_COUNT);
    return deltaPos;
}

objs collision_test_player(player_t *p, objs *tiles)
{
    objs collisions = {0};
    da_foreach (obj, tile, tiles) {
        if (SDL_HasRectIntersectionFloat(getBB(p), getBB(tile))) {
            da_append(&collisions, *tile);
        }
    }

    return collisions;
}

triggers_t collision_test_player_trigg(player_t *p, triggers_t *triggers_array)
{
    triggers_t collisions = {0};
    da_foreach (trigger_t, tile, triggers_array) {
        if ((tile->has_been_touched = SDL_HasRectIntersectionFloat(getBB(p), getBB(tile))) == true) {
            da_append(&collisions, *tile);
        }
    }

    return collisions;
}

void updatePlayer(player_t *p, objs *arr, float deltaTime, triggers_t *trigg_array, int *activeWave)
{
    float gravity = 28.0f;
    float dragCoef = 0.75f;
    float epsi = 1e-6;
    SDL_FRect *rect = getBB(p);
    V2f movement = inputUpdate(p, deltaTime);
    V2f frame_movement = {movement.x + p->velocity.x, movement.y + p->velocity.y};
    p->onGround = false;
    p->onWall = false;
    static bool wasOnGround = false;

    if (p->stamina < 3.0f) p->stamina = (p->stamina < 2.99f) ? p->stamina + 0.7 * deltaTime : 3.0f;

    if (p->velocity.x < epsi || p->velocity.x > epsi) p->velocity.x = p->velocity.x * dragCoef;

    // Play sfx when a bar of stamina is regenerated/recharged
    if ((p->stamina >= 0.97f && p->stamina <= 1.01f) || (p->stamina >= 1.97f && p->stamina <= 2.01f) ||
        (p->stamina >= 2.97f && p->stamina <= 2.99f)) {
        playSfx(*p->ctx, &p->audios, "staminaRegen");
    }

    rect->x += frame_movement.x;
    if (!p->noclip) {
        objs collisions = collision_test_player(p, arr);
        triggers_t trigg_collision = collision_test_player_trigg(p, trigg_array);
        da_foreach (obj, it, &collisions) {
            SDL_FRect *tile = it->boundingBox;
            p->onWall = true;

            if (frame_movement.x > 0) {
                rect->x = Left(tile) - rect->w - 0.01f; // Set the player's right edge to the tile's left edge
            }
            if (frame_movement.x < 0) {
                rect->x = Right(tile) + 0.01f; // Set the player's left edge to the tile's right edge
            }
        }
        da_foreach (trigger_t, it, &trigg_collision) { // trigger box collision check
            SDL_FRect *tile = it->boundingBox;
            if (it->waveId >= 0 && activeWave != NULL) {
                *activeWave = it->waveId;
                printf("TRIGGER hit (x=%.1f y=%.1f) -> wave %d\n", tile->x, tile->y, it->waveId);
            } else {
                printf("TRIGGER hit (x=%.1f y=%.1f) -> no wave id\n", tile->x, tile->y);
            }
        }
        free(collisions.items);
        free(trigg_collision.items);
    }

    rect->y += frame_movement.y;
    if (!p->noclip) {
        objs collisions = collision_test_player(p, arr);
        triggers_t trigg_collision = collision_test_player_trigg(p, trigg_array);
        da_foreach (obj, it, &collisions) {
            SDL_FRect *tile = it->boundingBox;
            if (frame_movement.y > 0) {
                rect->y = Top(tile) - rect->h - 0.01f; // Set the player's right edge to the tile's left edge
                p->onGround = true;
                if (p->onGround && !wasOnGround) {
                    if (p->isSlamming) playSfx(*p->ctx, &p->audios, "hardFall");
                    else
                        playSfx(*p->ctx, &p->audios, "softFall");
                }
                p->isSlamming = false;
            }
            if (frame_movement.y < 0) {
                rect->y = Bottom(tile) + 0.01f; // Set the player's left edge to the tile's right edge
            }
            p->velocity.y = 0;
        }
        da_foreach (trigger_t, it, &trigg_collision) { // trigger box collision check
            SDL_FRect *tile = it->boundingBox;
            if (it->waveId >= 0 && activeWave != NULL) {
                *activeWave = it->waveId;
                printf("TRIGGER hit (x=%.1f y=%.1f) -> wave %d\n", tile->x, tile->y, it->waveId);
            } else {
                printf("TRIGGER hit (x=%.1f y=%.1f) -> no wave id\n", tile->x, tile->y);
            }
        }
        free(collisions.items);
        free(trigg_collision.items);
    }
    wasOnGround = p->onGround;

    if (!p->flight) p->velocity.y = MIN(50.0f, p->velocity.y + (gravity * deltaTime));
    // p->velocity.y = p->velocity.y + (gravity * deltaTime);

    keepRectInbounds(p->boundingBox, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void renderPlayer(player_t *p)
{
    SDL_FlipMode flipped =
        (p->lastKey == SDL_SCANCODE_D || p->lastKey == SDL_SCANCODE_UNKNOWN) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderTextureRotated((*p->ctx)->renderer, p->tex, NULL, p->boundingBox, 0, NULL, flipped);
}

// créé 3 bar, utilisé pour afficher la stamina du joueur (dash disponibles)
bool createPlayerStatusBar(sdl_ctx_t *sdl_ctx, bar **b1, bar **b2, bar **b3, bar **hpB) {
    // (WINDOW_HEIGHT - 150) * sdl_ctx->screenRatio correspond au y de la zone de footer, donc 150 est la hauteur du footer

    if (!createBar(hpB,
                   (SDL_FRect){15 * sdl_ctx->screenRatio,
                               (((WINDOW_HEIGHT - 150) * sdl_ctx->screenRatio) + 15) * sdl_ctx->screenRatio, 450 * sdl_ctx->screenRatio,
                               54.5f * sdl_ctx->screenRatio},
                   (SDL_Color){20, 20, 20, 255}, (SDL_Color){178, 19, 19, 255}, (SDL_Color){255, 255, 255, 255}, 100.0f, 10.0f, 0, true))
        return false;

    if (!createBar(b1,
                   (SDL_FRect){(15 + (5)) * sdl_ctx->screenRatio,
                               (((WINDOW_HEIGHT - 150) * sdl_ctx->screenRatio)  + 82.5f ) * sdl_ctx->screenRatio, 145 * sdl_ctx->screenRatio,
                               37.5f * sdl_ctx->screenRatio},
                   (SDL_Color){80, 80, 255, 255}, (SDL_Color){0, 0, 205, 255}, (SDL_Color){255, 255, 255, 255}, 1.0f, 0, 5 * sdl_ctx->screenRatio, false))
        return false;

    if (!createBar(b2,
                   (SDL_FRect){(*b1)->BarBox->x + (*b1)->BarBox->w,
                               (*b1)->BarBox->y, (*b1)->BarBox->w,
                               (*b1)->BarBox->h},
                   (SDL_Color){207, 80, 255, 255}, (SDL_Color){127, 0, 205, 255}, (SDL_Color){255, 255, 255, 255}, 1.0f, 0, 5 * sdl_ctx->screenRatio, false))
        return false;

        if (!createBar(b3,
                       (SDL_FRect){(*b1)->BarBox->x + (*b1)->BarBox->w * 2,
                                   (*b1)->BarBox->y, (*b1)->BarBox->w,
                                   (*b1)->BarBox->h},
                       (SDL_Color){255, 143, 226, 255}, (SDL_Color){253, 63, 146, 255}, (SDL_Color){255, 255, 255, 255}, 1.0f, 0, 5 * sdl_ctx->screenRatio, false))
        return false;

    return true;
}

bool renderPlayerStatusBar(sdl_ctx_t *sdl_ctx, player_t *player, bar *b1, bar *b2, bar *b3, bar *hpB) {
    SDL_FRect dashBgBox = {hpB->BarBox->x, b1->BarBox->y - ((13.0f / 2.0f) * sdl_ctx->screenRatio),
                            ((b1->BarBox->w * 3) + 10.0f + b1->minCursorWidth) * sdl_ctx->screenRatio, hpB->BarBox->h - (3 * sdl_ctx->screenRatio)};

    barRender(sdl_ctx, hpB, player->hp, 50, 50, 50);

    // valeur de chacune des bars de dash
    // le min est pris, car dash1 peut être supérieur a 1, ce que je ne veux pas !
    float dash1 = fminf(player->stamina > 0 ? player->stamina : 0, 1.0f);
    float dash2 = fminf(player->stamina - 1.0f > 0 ? player->stamina - 1 : 0, 1.0f);
    float dash3 = fminf(player->stamina - 2.0f > 0 ? player->stamina - 2 : 0, 1.0f);

    renderFillRect(sdl_ctx->renderer, &dashBgBox, (SDL_Color){20, 20, 20, 255});

    barRender(sdl_ctx, b1, dash1, 50, 50, 50);
    barRender(sdl_ctx, b2, dash2, 50, 50, 50);
    barRender(sdl_ctx, b3, dash3, 50, 50, 50);
}

void destroyPlayerStatusBar(bar **b1, bar **b2, bar **b3, bar **hpB) {
    destroyBar(hpB);
    destroyBar(b1);
    destroyBar(b2);
    destroyBar(b3);
}

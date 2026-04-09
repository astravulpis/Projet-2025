#include "player_animation.h"
#include "common.h"
#include "sdl_helpers.h"
#include <math.h>

bool createPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation **pa, char *framesPath, float loopInterval)
{
    *pa = calloc(1, sizeof(player_animation));
    if ((*pa) == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for player_animation", __FILE__, __LINE__);
        return false;
    }

    // "assets/img/animation/V1Animation/idleAnimation/1.png"
    //                       ...                       2.png"
    //                       ...                       3.png"
    //                       ...                       4.png"
    //                       ...                       5.png"

    size_t mark = temp_save();
    for (int i = 0; i < 5; ++i) {
        temp_rewind(mark);
        char *path = temp_sprintf("%s%d.png", framesPath, i + 1);
        printf("path: %s\n", path);
        (*pa)->animationTab[i] = IMG_LoadTexture(sdl_ctx->renderer, path);
        if ((*pa)->animationTab[i] == NULL) {
            nob_log(WARNING, "Failed to load animation frame. See error: %s", SDL_GetError());
        }
    }
    (*pa)->currentAnimationIndex = 0;
    (*pa)->cumulateTime = -1;

    // loopInterval permet de controler la vitesse d'animation
    (*pa)->imgPerMs = loopInterval / ANIMATION_NB_FRAME;
    (*pa)->loopInterval = loopInterval;

    return true;
}

void destroyPlayerAnimation(player_animation **pa)
{
    if (*pa != NULL) {
        for (int i = 0; i < 5; ++i) {
            SDL_DestroyTexture((*pa)->animationTab[i]);
        }
    }
    free(*pa);
    (*pa) = NULL;
}

void resetPlayerAnimationState(player_animation *pa)
{ // set currentAnimationIndex to 0 and last time to -1
    if (pa != NULL) {
        pa->currentAnimationIndex = 0;
        pa->cumulateTime = -1;
    }
}

// void renderPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation *pa, SDL_FlipMode flipped, float deg)
void renderPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation *pa, SDL_FlipMode flipped, float deg, SDL_FRect *animationBox)
{
    if (pa != NULL) {
        if (pa->cumulateTime != -1) {
            float currentTick = SDL_GetTicks();

            if (currentTick - pa->cumulateTime >
                pa->loopInterval) { // si cumulateTime a été initialisé il y'a loopInterval il est remis au tick actuel
                pa->cumulateTime = currentTick;
                pa->currentAnimationIndex = 0;
            } else {
                pa->currentAnimationIndex =
                    fminf(roundf((currentTick - pa->cumulateTime) / pa->imgPerMs), ANIMATION_NB_FRAME - 1);
            }

            SDL_RenderTextureRotated(sdl_ctx->renderer, pa->animationTab[pa->currentAnimationIndex], NULL, animationBox, deg,
                                     NULL, flipped);
        } else {// normalement si on arrive ici, currentAnimationIndex est a 0 (car il y'a eu reset)
            pa->cumulateTime = SDL_GetTicks();
            SDL_RenderTextureRotated(sdl_ctx->renderer, pa->animationTab[pa->currentAnimationIndex], NULL, animationBox, deg,
                                     NULL, flipped);
        }
    }
}

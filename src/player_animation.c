/**
 * @file player_animation.c
 * @brief File to implement player animations
*
 * @author Rossignol François <francois_rossignol@outlook.fr>
 * @date 2026-04-08
 * @remark last Modified: 2026-04-09
 *
 * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 * Liam B. <liam.berge72@gmail.com>
 **/

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
        (*pa)->animationTab[i] = IMG_LoadTexture(sdl_ctx->renderer, path);
        if ((*pa)->animationTab[i] == NULL) {
            nob_log(WARNING, "Failed to load animation frame. See error: %s", SDL_GetError());
        }
    }
    (*pa)->currentAnimationIndex = 0;
    (*pa)->cumulateTime = -1;

    // loopInterval allows you to control the animation speed
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
                pa->loopInterval) { // If `cumulateTime` was initialized `loopInterval` ago, it is reset to the current tick
                pa->cumulateTime = currentTick;
                pa->currentAnimationIndex = 0;
            } else {
                pa->currentAnimationIndex =
                    fminf(roundf((currentTick - pa->cumulateTime) / pa->imgPerMs), ANIMATION_NB_FRAME - 1);
            }

            SDL_RenderTextureRotated(sdl_ctx->renderer, pa->animationTab[pa->currentAnimationIndex], NULL, animationBox, deg,
                                     NULL, flipped);
        } else {// Normally, if we get here, `currentAnimationIndex` is 0 (because it has been reset)
            pa->cumulateTime = SDL_GetTicks();
            SDL_RenderTextureRotated(sdl_ctx->renderer, pa->animationTab[pa->currentAnimationIndex], NULL, animationBox, deg,
                                     NULL, flipped);
        }
    }
}

#include "player_animation.h"
#include "common.h"
#include "sdl_helpers.h"
#include <math.h>

//bool createPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation **pa, SDL_FRect paRect, char *frame0Path, char *frame1Path, char *frame2Path, char *frame3Path, char *frame4Path)
bool createPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation **pa, char *frame0Path, char *frame1Path, char *frame2Path, char *frame3Path, char *frame4Path, float loopInterval)
{
    *pa = calloc(1, sizeof(player_animation));
    if ((*pa) == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for player_animation", __FILE__, __LINE__);
        return false;
    }

    (*pa)->animationTab[0] = IMG_LoadTexture(sdl_ctx->renderer, frame0Path);
    (*pa)->animationTab[1] = IMG_LoadTexture(sdl_ctx->renderer, frame1Path);
    (*pa)->animationTab[2] = IMG_LoadTexture(sdl_ctx->renderer, frame2Path);
    (*pa)->animationTab[3] = IMG_LoadTexture(sdl_ctx->renderer, frame3Path);
    (*pa)->animationTab[4] = IMG_LoadTexture(sdl_ctx->renderer, frame4Path);

    //(*pa)->animationBox = createRect_Ex(paRect);

    (*pa)->currentAnimationIndex = 0;
    (*pa)->cumulateTime  = -1;

    // loopInterval permet de controler la vitesse d'animation
    (*pa)->imgPerMs = loopInterval / ANIMATION_NB_FRAME;
    (*pa)->loopInterval = loopInterval;

    return true;
}

void destroyPlayerAnimation(player_animation **pa) {
    if (*pa != NULL) {

        //free((*pa)->animationBox);
        //(*pa)->animationBox = NULL;

        SDL_DestroyTexture((*pa)->animationTab[0]);
        SDL_DestroyTexture((*pa)->animationTab[1]);
        SDL_DestroyTexture((*pa)->animationTab[2]);
        SDL_DestroyTexture((*pa)->animationTab[3]);
        SDL_DestroyTexture((*pa)->animationTab[4]);

        free(*pa);
        (*pa) = NULL;
    }
}

void resetPlayerAnimationState(player_animation *pa) { // set currentAnimationIndex to 0 and last time to -1
    if (pa != NULL) {
        pa->currentAnimationIndex = 0;
        pa->cumulateTime  = -1;
    }
}

//void renderPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation *pa, SDL_FlipMode flipped, float deg)
void renderPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation *pa, SDL_FlipMode flipped, float deg, SDL_FRect *animationBox)
{
    if (pa != NULL) {
        if (pa->cumulateTime != -1) {
            float currentTick = SDL_GetTicks();

            if (currentTick - pa->cumulateTime > pa->loopInterval) { // si cumulateTime a été initialisé il y'a loopInterval il est remis au tick actuel
                pa->cumulateTime = currentTick;
                pa->currentAnimationIndex = 0;
            }
            else {
            pa->currentAnimationIndex = fminf(roundf((currentTick - pa->cumulateTime) / pa->imgPerMs), ANIMATION_NB_FRAME - 1);
            }

            SDL_RenderTextureRotated(sdl_ctx->renderer, pa->animationTab[pa->currentAnimationIndex], NULL, animationBox, deg, NULL, flipped);

        }
        else {// normalement si on arrive ici, currentAnimationIndex est a 0 (car il y'a eu reset)
            pa->cumulateTime = SDL_GetTicks();    
            SDL_RenderTextureRotated(sdl_ctx->renderer, pa->animationTab[pa->currentAnimationIndex], NULL, animationBox, deg, NULL, flipped);
        }
    }
}
#ifndef PLAYER_ANIMATION_H_
#define PLAYER_ANIMATION_H_

#include "common.h"

#define ANIMATION_NB_FRAME 5

typedef struct {
    //SDL_FRect *animationBox;
    SDL_Texture *animationTab[ANIMATION_NB_FRAME]; // chaque animation possède 5 images

    int currentAnimationIndex;
    float cumulateTime;
    float imgPerMs;
    float loopInterval; // en ms 1000ms = animationa 5 fps, 500ms = animation a 10fps
}player_animation;

//bool createPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation **pa, SDL_FRect paRect, char *frame0Path, char *frame1Path, char *frame2Path, char *frame3Path, char *frame4Path);
bool createPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation **pa, char *frame0Path, char *frame1Path, char *frame2Path, char *frame3Path, char *frame4Path, float loopInterval);
void destroyPlayerAnimation(player_animation **pa);
void resetPlayerAnimationState(player_animation *pa);
//void renderPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation *pa, SDL_FlipMode flipped, float deg);
void renderPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation *pa, SDL_FlipMode flipped, float deg, SDL_FRect *animationBox);

#endif // PLAYER_ANIMATION_H_
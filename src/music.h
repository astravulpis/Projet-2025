#ifndef MUSIC_H_
#define MUSIC_H_

#include "common.h"

/**
 * @fn Mix_Init(const char * path, sdl_ctx_t * ctx)
 * @param[in] path pointer pointer to the player itself
 * @param[in] ctx our own sdl context variable
 * @brief initialises the level music
 *
 * takes the path to the music obtained in the level parsing. this is then fed into SDL_Mixer to
 * start playing music for the level
 */
bool loadTrack(sdl_ctx_t *ctx, const char *path);

void playTrack(sdl_ctx_t *ctx);

/**
 * @fn sfx(const char * path)
 * @param[in] path pointer pointer to the player itself
 * @brief plays sound effects
 *
 * we feed this the path to the sound effect wanted in a case-by-case basis
 * with the sound effect being played without any need for tracks
 * this overlaps without issues with the level song
 */
bool sfx(sdl_ctx_t *sdl_ctx, const char *path);
#endif

#ifndef MUSIC_H_
#define MUSIC_H_

#include "common.h"

typedef struct {
    char *name;
    MIX_Audio *ptr;
} sfx;

typedef struct {
    sfx **items;
    size_t count;
    size_t capacity;
} sfxs;

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
 * @fn loadSfx(sdl_ctx_t *sdl_ctx, const char * path)
 * @param[in] sdl_ctx pointer to our sdl context
 * @param[in] audios pointer to a dynamic array of sfx samples loaded
 * @param[in] name name of the sfx sample
 * @param[in] path path to the music sample
 * @brief loads a sound effect and returns its pointer
 *
 * we feed this the path to the sound effect wanted in a case-by-case basis
 * this is just a wrapper function for MIX_LoadAudio
 * and the neat part is that MIX_Quit() handles the destroying part as each
 * loaded audio has its own reference count handled automatically
 */
void loadSfx(sdl_ctx_t *sdl_ctx, sfxs *audios, char *name, const char *path);

bool playSfx(sdl_ctx_t *ctx, sfxs *audios, const char *sfx_name);
bool __playSfx(sdl_ctx_t *sdl_ctx, MIX_Audio *audio);

void destroySfx(sfx **sfx);
void destroySfxs(sfxs *sfxs);

#endif

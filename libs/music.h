#ifndef MUSIC_H_
#define MUSIC_H_

#include "common.h"

/**
 * @struct sfx
 * @brief sound effect details
 */
typedef struct {
    char *name;
    MIX_Audio *ptr;
    size_t track_idx;
} sfx;

/**
 * @struct sfxs
 * @brief list of sound effects
 *
 * contains a list of individual sound effects
 */
typedef struct {
    sfx **items;
    size_t count;
    size_t capacity;
} sfxs;

/**
 * @fn loadTrack(sdl_ctx_t *ctx, int trackIdx, const char *path)
 * @param[in] path pointer pointer to the player itself
 * @param[in] ctx our own sdl context variable
 * @param[in] trackIdx track number to load the track
 * @brief loads a track into ctx
 */
bool loadTrack(sdl_ctx_t *ctx, int trackIdx, const char *path);

/**
 * @fn playTrack(sdl_ctx_t *ctx, int trackIdx)
 * @param[in] ctx our own sdl context variable
 * @param[in] trackIdx id of the track we want to play
 * @brief plays the track corresponding to the track ID
 */
void playTrack(sdl_ctx_t *ctx, int trackIdx);

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
void loadSfx(sdl_ctx_t *sdl_ctx, sfxs *audios, size_t track_name, char *name, const char *path);

/**
 * @fn playSfx(sdl_ctx_t *ctx, sfxs *audios, const char *sfx_name)
 * @param[in] ctx our own sdl context variable
 * @param[in] audios list of all the audios
 * @param[in] sfx_name name of the audio we are looking for
 * @brief goes through the list of sound effects looking for the one with the given name
 */
void playSfx(sdl_ctx_t *ctx, sfxs *audios, const char *sfx_name);

/**
 * @fn __playSfx(sdl_ctx_t *sdl_ctx, size_t track_idx, MIX_Audio *audio)
 * @param[in] ctx our own sdl context variable
 * @param[in] track_idx id of the track we want to play
 * @param[in] audio audio pointer to MIX
 * @brief internal function to seperate the setting track audio and actually playing it
 */
void __playSfx(sdl_ctx_t *sdl_ctx, size_t track_idx, MIX_Audio *audio);

/**
 * @fn destroySfx(sfx **sfx)
 * @param[in] sfx one sound effect
 * @brief destroys a sound effect
 */
void destroySfx(sfx **sfx);

/**
 * @fn destroySfxs(sfxs *sfxs)
 * @param[in] sfxs list of all the sound effects
 * @brief goes through the list of all sound effects to destory them all
 */
void destroySfxs(sfxs *sfxs);

#endif

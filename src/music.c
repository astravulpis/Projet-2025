/**
 * @file music.c
 * @brief File to implement music and sounds
 *
 * @author Reeves Guillaume <greeves2306@gmail.com>
 * @remark last modified: 2026-03-26
 * @date 2026-03-22
 *
 * Contributors:
 * Reeves Guillaume <greeves2306@gmail.com>
 **/

#include "music.h"
#include "common.h"

bool loadTrack(sdl_ctx_t *ctx, int trackIdx, const char *path)
{
    MIX_Audio *audio = NULL;

    /* load a sound file */
    audio = MIX_LoadAudio(ctx->mixer, path, false);
    if (!audio) {
        SDL_Log("Couldn't load %s: See error: %s", path, SDL_GetError());
        return false;
    }

    /* we need a track on the mixer to play the audio. Each track has audio assigned to it, and
       all playing tracks are mixed together for the final output. */

    MIX_SetTrackAudio(ctx->tracks[trackIdx], audio);

    /* we don't save `audio`; SDL_mixer will clean it up for us during MIX_Quit(). */
    return true;
}

void playTrack(sdl_ctx_t *ctx, int trackIdx)
{
    MIX_PlayTrack(ctx->tracks[trackIdx], 0);  /* no extra options this time, so a zero for the second argument. */
}

void loadSfx(sdl_ctx_t *sdl_ctx, sfxs *audios, size_t track_name, char *name, const char *path)
{
    MIX_Audio *audio = MIX_LoadAudio(sdl_ctx->mixer, path, false);

    if (!audio) {
        nob_log(ERROR, "%s:%d: See error: %s", __FILE__, __LINE__, SDL_GetError());
        return;
    }

    sfx *s = calloc(1, sizeof(sfx));
    if (!s) {
        nob_log(ERROR, "failed to allocate size from a sfx");
        return;
    }

    s->name = strdup(name);
    s->ptr = audio;
    s->track_idx = track_name;

    da_append(audios, s);
}

void playSfx(sdl_ctx_t *ctx, sfxs *audios, const char *sfx_name)
{
    da_foreach (sfx *, sfx, audios) {
        if (strcmp((*sfx)->name, sfx_name) == 0) {
            __playSfx(ctx, (*sfx)->track_idx, (*sfx)->ptr);
            break;
        }
    }
}

void __playSfx(sdl_ctx_t *sdl_ctx, size_t track_idx, MIX_Audio *audio)
{
    MIX_SetTrackAudio(sdl_ctx->tracks[track_idx], audio);
    playTrack(sdl_ctx, track_idx);
}

void destroySfx(sfx **sfx)
{
    if ((*sfx) != NULL) {
        free((*sfx)->name);
        (*sfx)->name = NULL;
    }

    free(*sfx);
    *sfx = NULL;
}

void destroySfxs(sfxs *sfxs)
{
    da_foreach (sfx *, s, sfxs) {
        destroySfx(s);
    }

    free(sfxs->items);
    sfxs->items = NULL;
    sfxs->count = 0;
}

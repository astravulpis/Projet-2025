/**
 * @file music.c
 * @brief File to implement music and sounds
 *
 * Author: Reeves Guillaume <greeves2306@gmail.com>
 * Last Modified: 2026-03-26
 * Date: 2026-03-22
 *
 * * Contributors:
 * Reeves Guillaume <greeves2306@gmail.com>
 **/

#include "music.h"
#include "common.h"
#include "sdl_ctx.h"

bool loadTrack(sdl_ctx_t *ctx, const char *path)
{
    MIX_Audio *audio = NULL;

    /* load a sound file */
    audio = MIX_LoadAudio(ctx->mixer, path, false);
    if (!audio) {
        SDL_Log("Couldn't load %s: %s", path, SDL_GetError());
        return false;
    }

    /* we need a track on the mixer to play the audio. Each track has audio assigned to it, and
       all playing tracks are mixed together for the final output. */

    MIX_SetTrackAudio(ctx->track, audio);

    /* start the audio playing! */
    playTrack(ctx);

    /* we don't save `audio`; SDL_mixer will clean it up for us during MIX_Quit(). */
    return true;
}

void playTrack(sdl_ctx_t *ctx)
{
    MIX_PlayTrack(ctx->track, 0);  /* no extra options this time, so a zero for the second argument. */
}

void(sdl_ctx_t *sdl_ctx, sfxs *audios, char *name, const char *path)
{
    MIX_Audio *audio = MIX_LoadAudio(sdl_ctx->mixer, path, false);

    if (!audio) {
        nob_log(ERROR, "See error: %s", SDL_GetError());
        return;
    }

    da_append(audios, ((sfx){name, audio}));
}

bool playSfx(sdl_ctx_t *ctx, sfxs *audios, const char *sfx_name);
{
    da_foreach (sfx *, sfx, audios) {
        if (strcmp(sfx, sfx_name) == 0) return __playSfx(ctx, sfx->ptr);
    }
}

bool __playSfx(sdl_ctx_t *sdl_ctx, MIX_Audio *audio)
{
    return MIX_
}

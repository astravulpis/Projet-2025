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
#include "SDL3_mixer/SDL_mixer.h"
#include "sdl_ctx.h"
#include "common.h"

static void freeMusicResources(MIX_Track * track, MIX_Audio * audio)
{
    if (track != NULL) {
        MIX_DestroyTrack(track);
        track = NULL;
    }

    if (audio != NULL) {
        MIX_FreeAudio(audio);
        audio = NULL;
    }
}

bool initMusic(MIX_Mixer * mixer)
{
    if (SDL_WasInit(SDL_INIT_AUDIO) == 0) {
        if (!SDL_Init(SDL_INIT_AUDIO)) {
            nob_log(ERROR, "%s:%d: Failed to initialize SDL audio. See: %s", __FILE__, __LINE__, SDL_GetError());
            return false;
        }
    }

    if (!MIX_Init()) {
        nob_log(ERROR, "%s:%d: Failed to initialize SDL_mixer. See: %s", __FILE__, __LINE__, SDL_GetError());
        return false;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (mixer == NULL) {
        nob_log(ERROR, "%s:%d: Failed to create SDL_mixer mixer device. See: %s", __FILE__, __LINE__, SDL_GetError());
        MIX_Quit();
        return false;
    }

    return true;
}

bool playMusic(const char *path, bool loop, MIX_Mixer * mixer, MIX_Audio * audio, MIX_Track * track)
{
    if (mixer == NULL) {
        nob_log(ERROR, "%s:%d: Audio mixer is not initialized.", __FILE__, __LINE__);
        return false;
    }

    freeMusicResources(track, audio);

    audio = MIX_LoadAudio(mixer, path, true);
    if (audio == NULL) {
        nob_log(ERROR, "%s:%d: Failed to load music file '%s'. See: %s", __FILE__, __LINE__, path, SDL_GetError());
        return false;
    }

    track = MIX_CreateTrack(mixer);
    if (track == NULL) {
        nob_log(ERROR, "%s:%d: Failed to create music track. See: %s", __FILE__, __LINE__, SDL_GetError());
        freeMusicResources(track, audio);
        return false;
    }

    if (!MIX_SetTrackAudio(track, audio)) {
        nob_log(ERROR, "%s:%d: Failed to assign music to track. See: %s", __FILE__, __LINE__, SDL_GetError());
        freeMusicResources(track, audio);
        return false;
    }

    SDL_PropertiesID options = 0;
    if (loop) {
        options = SDL_CreateProperties();
        if (options == 0) {
            nob_log(ERROR, "%s:%d: Failed to create SDL properties for music playback.", __FILE__, __LINE__);
            return false;
        }
        SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    }

    bool success = MIX_PlayTrack(track, options);
    if (options != 0) {
        SDL_DestroyProperties(options);
    }

    if (!success) {
        nob_log(ERROR, "%s:%d: Failed to play music track. See: %s", __FILE__, __LINE__, SDL_GetError());
        return false;
    }

    return true;
}

void stopMusic(MIX_Track * track)
{
    if (track != NULL) {
        MIX_StopTrack(track, 0);
    }
}

void pauseMusic(MIX_Track * track)
{
    if (track != NULL) {
        MIX_PauseTrack(track);
    }
}

void resumeMusic(MIX_Track * track)
{
    if (track != NULL) {
        MIX_ResumeTrack(track);
    }
}

void cleanupMusic(MIX_Mixer * mixer)
{

    if (mixer != NULL) {
        MIX_DestroyMixer(mixer);
        mixer = NULL;
    }

    MIX_Quit();
}



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
#include "sdl_ctx.h"
#include "common.h"

bool Mix_Init(const char * path, sdl_ctx_t * ctx)
{
    MIX_Mixer * mixer = NULL;
    MIX_Track * track = NULL;
    MIX_Audio * audio = NULL;


    if (!MIX_Init()) {
        nob_log(ERROR, "%s:%d: Failed to initialise MIX", __FILE__, __LINE__);
        return false;
    }

    /* Create a mixer on the default audio device. Don't care about the specific audio format. */
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer) {
        SDL_Log("Couldn't create mixer on default device: %s", SDL_GetError());
        return false;
    }

    /* load a sound file */
    audio = MIX_LoadAudio(mixer, path, false);
    if (!audio) {
        SDL_Log("Couldn't load %s: %s", path, SDL_GetError());
        return false;
    }

    /* we need a track on the mixer to play the audio. Each track has audio assigned to it, and
       all playing tracks are mixed together for the final output. */

    track = MIX_CreateTrack(mixer);
    if (!track) {
        SDL_Log("Couldn't create a mixer track: %s", SDL_GetError());
        return false;
    }
    MIX_SetTrackAudio(track, audio);

    ctx->track=track;

    /* start the audio playing! */
    MIX_PlayTrack(track, 0);  /* no extra options this time, so a zero for the second argument. */

    /* we don't save `audio`; SDL_mixer will clean it up for us during MIX_Quit(). */
    return true;
}

bool dash(){
    MIX_Mixer * mixer = NULL;
    MIX_Audio * audio = NULL;

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer) {
        SDL_Log("Couldn't create mixer on default device: %s", SDL_GetError());
        return false;
    }

    /* load a sound file */#
    const char * path = "./assets/audio/SFX/dash.mp3";
    audio = MIX_LoadAudio(mixer, path, false);
    if (!audio) {
        SDL_Log("Couldn't load %s: %s", path, SDL_GetError());
        return false;
    }
    MIX_PlayAudio(mixer, audio);
}

/*
void freeMusicResources(MIX_Track * track, MIX_Audio * audio)
{
    if (track != NULL) {
        MIX_DestroyTrack(track);
        track = NULL;
    }

    if (audio != NULL) {
        SDL_free(audio);
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
        freeMusicResources(track, audio);
        return false;
    }

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
*/


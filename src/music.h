#include "common.h"

#ifndef MUSIC_H_
#define MUSIC_H_

typedef enum {
    JUMP = 0,
    DASH = 1,
    SHOOT = 2,
    SOUND_COUNT
} sound_type_t;

extern const char* sound_paths[SOUND_COUNT];

/*
void freeMusicResources(MIX_Track * track, MIX_Audio * audio);
bool initMusic(MIX_Mixer * mixer);
bool playMusic(const char *path, bool loop, MIX_Mixer * mixer, MIX_Audio * audio, MIX_Track * track);
void stopMusic(MIX_Track * track);
void pauseMusic(MIX_Track * track);
void resumeMusic(MIX_Track * track);
void cleanupMusic(MIX_Mixer * mixer);
*/

bool Mix_Init(const char * path, sdl_ctx_t * ctx);
bool sfx(const char * path);
bool initSounds(sdl_ctx_t * sdl_ctx);
#endif
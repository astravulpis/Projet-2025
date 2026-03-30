#include "common.h"

#ifndef MUSIC_H_
#define MUSIC_H_

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
bool dash();
#endif
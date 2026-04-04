#ifndef GUNS_H_
#define GUNS_H_

#include "common.h"
#include "music.h"

#define GUN_COUNT 6

typedef struct {
    // bulletPattern pattern;
    const char *sfx_path;
    const char *name;
    SDL_Texture associatedImage;
    size_t lmbCD;
    size_t rmbCD;
    int idx;
} Gun;

typedef struct {
    Gun arsenal[GUN_COUNT];
    int selectedGun;
} Guns;

// SFX
const char *getGunSfx(Gun *gun);
void setGunSfx(Gun *gun, const char *path);
void loadGunSfx(Gun *gun);

// Image
const char *getGunImage(Gun *gun);
void setGunImage(Gun *gun, const char *path);

// Interactions
void shootGun(Gun *gun);
bool isLmbOnCd(Gun *gun);
bool isRmbOnCd(Gun *gun);

// Current gun
int getCurrentGun(Guns *guns);
void setCurrentGun(Guns *guns, int idx);

// Destroy functions
void destroyGun(Gun **gun);
void destroyGuns(Guns **guns);

#endif // GUNS_H_

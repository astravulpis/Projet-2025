#include "guns.h"
#include <string.h>

static MIX_Audio *gun_sfx[GUN_COUNT] = {0};

// SFX
const char *getGunSfx(Gun *gun)
{
    return gun->sfx_path;
}
void setGunSfx(Gun *gun, const char *path)
{
    gun->sfx_path = strdup(path);
}

void loadGunSfx(Gun *gun)
{
    if (gun_sfx[(assert(gun->idx < GUN_COUNT), gun->idx)] == NULL) {
        loadSfx(
    }
}

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
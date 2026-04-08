#ifndef GUNS_H_
#define GUNS_H_

#include "common.h"
#include "music.h"
#include "bullets.h"

#define GUN_COUNT 6

typedef struct {
    // bulletPattern pattern;
    const char *sfx_path;
    const char *img_path;
    const char *name;
    SDL_Texture associatedImage;
    size_t lmbCD;
    size_t rmbCD;
    int idx;
    int size;
    int dmg;
} Gun_t;

typedef struct {
    Gun_t * arsenal;
    int selectedGun;
} Guns_t;

/**
 * @fn initialiseGuns(sdl_ctx_t * ctx)
 * @param[in] ctx SDL context for loading images and audio
 * @brief Allocates and initializes all guns with their properties
 * @return Pointer to the allocated Guns_t structure, or NULL on error
 */
Guns_t *initialiseGuns(sdl_ctx_t * ctx);

// SFX
/**
 * @fn getGunSfx(Gun_t *gun)
 * @param[in] gun gun structure
 * @brief returns the gun's sound effect when shot
 */
const char *getGunSfx(Gun_t *gun);

/**
 * @fn setGunSfx(Gun_t *gun, const char *path)
 * @param[in] gun gun structure
 * @param[in] path path to the sound effect
 * @brief assigns the sound effect in path to the gun itself
 */
void setGunSfx(Gun_t *gun, const char *path);

/**
 * @fn loadGunSfx(Gun_t *gun, sdl_ctx_t * ctx)
 * @param[in] gun gun structure
 * @param[in] ctx our own sdl context variable
 * @brief loads the gun's sound effect using MIX_LoadAudio
 */
void loadGunSfx(Gun_t *gun, sdl_ctx_t * ctx);

// Image
/**
 * @fn getGunImage(Gun_t *gun)
 * @param[in] gun gun structure
 * @brief returns the gun's texture
 */
const char *getGunImage(Gun_t *gun);

/**
 * @fn setGunImage(Gun_t *gun, const char *path)
 * @param[in] gun gun structure
 * @param[in] path path to the texture
 * @brief assigns the gun its texture
 */
void setGunImage(Gun_t *gun, const char *path);

/**
 * @fn setGunImage(Gun_t *gun, const char *path
 * @param[in] gun gun structure
 * @param[in] path path to the texture
 * @brief assigns the gun its texture
 */
void loadGunImage(Gun_t *gun, sdl_ctx_t *ctx);

// Interactions
/**
 * @fn setGunImage(Gun_t *gun, const char *path
 * @param[in] gun gun structure
 * @param[in] path path to the texture
 * @brief assigns the gun its texture
 */
void shootGun(sdl_ctx_t *sdl_ctx, Gun_t *gun, bullets *bullet_arr, V2f position, V2f direction);
bool isLmbOnCd(Gun_t *gun);
bool isRmbOnCd(Gun_t *gun);

// Current gun
int getCurrentGun(Guns_t *guns);
Gun_t *getCurrentGunPtr(Guns_t *guns);
void setCurrentGun(Guns_t *guns, int idx);

// Initialize guns
void destroyGun(Gun_t **gun);
void destroyGuns(Guns_t **guns);

#endif // GUNS_H_
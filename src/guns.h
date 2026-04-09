#ifndef GUNS_H_
#define GUNS_H_

#include "bullets.h"
#include "common.h"
#include "music.h"

typedef enum { PIERCER, SHARPSHOOTER, SHOTGUN, MACHINEGUN, RAILCANNON, ROCKET, __gun_kind_count = 6 } gun_kind;

typedef struct {
    gun_kind kind;
    char *sfx_path;
    char *img_path;
    SDL_Texture associatedImage;
    float size;
    float dmg;
} Gun_t;

typedef struct {
    Gun_t *arsenal;
    int selectedGun;
} Guns_t;

/**
 * @fn initialiseGuns(sdl_ctx_t * ctx)
 * @param[in] ctx SDL context for loading images and audio
 * @brief Allocates and initializes all guns with their properties
 * @return Pointer to the allocated Guns_t structure, or NULL on error
 */
Guns_t *initialiseGuns(sdl_ctx_t *ctx);

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
void loadGunSfx(Gun_t *gun, sdl_ctx_t *ctx);

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
 * @fn loadGunImage(Gun_t *gun, sdl_ctx_t *ctx)
 * @param[in] gun gun structure
 * @param[in] ctx our sdl context variable
 * @brief loads the gun's image
 */
void loadGunImage(Gun_t *gun, sdl_ctx_t *ctx);

// Interactions
/**
 * @fn shootGun(sdl_ctx_t *sdl_ctx, Gun_t *gun, bullets *bullet_arr, V2f position, V2f direction)
 * @param[in] sdl_ctx sld context variable
 * @param[in] gun currently equiped gun
 * @param[in] bullet_arr dynamic array of all on screen bullets
 * @param[in] position starting position
 * @param[in] vel normalised bullet speed
 * @brief assigns the gun its texture
 */
void shootGun(sdl_ctx_t *sdl_ctx, Gun_t *gun, bullets *bullet_arr, V2f position, V2f vel);

// Current gun
/**
 * @fn getCurrentGun(Guns_t *guns)
 * @param[in] guns gun structure
 * @brief returns the ID of the currently equipped gun
 */
int getCurrentGun(Guns_t *guns);

/**
 * @fn setCurrentGun(Guns_t *guns, int idx)
 * @param[in] guns gun structure
 * @param[in] kind the kind of gun you'd like to use
 * @brief equips the gun with ID idx
 */
void setCurrentGun(Guns_t *guns, gun_kind kind);

// Initialize guns
/**
 * @fn destroyGun(Gun_t *gun)
 * @param[in] gun gun structure
 * @brief destroys a gun structure
 */
void destroyGun(Gun_t *gun);

/**
 * @fn destroyGuns(Guns_t **guns)
 * @param[in] guns guns structure
 * @brief goes through the list of all guns to destory them
 */
void destroyGuns(Guns_t **guns);

#endif // GUNS_H_

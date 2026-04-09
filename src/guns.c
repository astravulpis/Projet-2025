#include "guns.h"
#include "SDL3/SDL_render.h"
#include "common.h"
#include <math.h>
#include <string.h>

static MIX_Audio *gun_sfx[__gun_kind_count] = {0};
static SDL_Texture *gun_textures[__gun_kind_count] = {0};
static SDL_Texture *bullet_textures[__gun_kind_count] = {0};

void createGun(Guns_t *guns, gun_kind kind, float dmg, float size, char *sfxPath, char *gunImage, char *bulletTexture)
{
    guns->arsenal[kind].kind = kind;
    guns->arsenal[kind].dmg = dmg;
    guns->arsenal[kind].size = size;
    setGunSfx(&guns->arsenal[kind], sfxPath);
    setGunImage(&guns->arsenal[kind], gunImage);
    setBulletTexture(&guns->arsenal[kind], bulletTexture);
    // setGunSfx(&guns->arsenal[kind], "./assets/audio/SFX/piercer.wav");
    // setGunImage(&guns->arsenal[kind], "./assets/img/guns/pistol.png");
}

Guns_t *initialiseGuns(sdl_ctx_t *ctx)
{
    Guns_t *guns = malloc(sizeof(Guns_t));
    if (guns == NULL) {
        return NULL;
    }
    guns->arsenal = malloc(sizeof(Gun_t) * __gun_kind_count);
    if (guns->arsenal == NULL) {
        free(guns);
        return NULL;
    }
    // Basic Pistol
    createGun(guns, PIERCER, 5, 20, "./assets/audio/SFX/guns/piercer.wav", "./assets/img/guns/pistol.png",
              "./assets/img/weapons/piercer.png");

    // Machine Gun
    createGun(guns, MACHINEGUN, 3, 10, "./assets/audio/SFX/guns/piercer.wav", "./assets/img/guns/machinegun.png",
              "./assets/img/weapons/minigun.png");

    // Shotgun
    createGun(guns, SHOTGUN, 8, 8, "./assets/audio/SFX/guns/shotgun.wav", "./assets/img/guns/shotgun.png",
              "./assets/img/weapons/shotgun.png");

    // Railcannon
    createGun(guns, RAILCANNON, 25, 30, "./assets/audio/SFX/guns/railcannon.wav", "./assets/img/guns/sniper.png",
              "./assets/img/weapons/sniper.png");

    // sharpshooter (bounce is not implemented since lack of time at the moment)
    createGun(guns, SHARPSHOOTER, 6, 20, "./assets/audio/SFX/guns/railcannon.wav", "./assets/img/guns/bouncer.png",
              "./assets/img/weapons/sharpshooter.png");

    // Rocket Launcher
    createGun(guns, ROCKET, 15, 45, "./assets/audio/SFX/guns/rocket.wav", "./assets/img/guns/rocket.png",
              "./assets/img/weapons/rocketlauncher.png");

    // Load all textures and sounds
    for (int i = 0; i < __gun_kind_count; i++) {
        loadGunSfx(&guns->arsenal[i], ctx);
        loadGunImage(&guns->arsenal[i], ctx);
        loadBulletTexture(&guns->arsenal[i], ctx);
    }

    guns->selectedGun = 0; // Start with pistol

    return guns;
}

// SFX
const char *getGunSfx(Gun_t *gun)
{
    return gun->sfx_path;
}
void setGunSfx(Gun_t *gun, const char *path)
{
    gun->sfx_path = strdup(path);
}

void loadGunSfx(Gun_t *gun, sdl_ctx_t *ctx)
{
    if (gun_sfx[(assert(gun->kind < __gun_kind_count), gun->kind)] == NULL) {
        gun_sfx[gun->kind] = MIX_LoadAudio(ctx->mixer, gun->sfx_path, 1);
    }
}

// Image
const char *getGunImage(Gun_t *gun)
{
    return gun->img_path;
}

void setGunImage(Gun_t *gun, const char *path)
{
    gun->img_path = strdup(path);
}

void setBulletTexture(Gun_t *gun, const char *path)
{
    gun->bullet_path = strdup(path);
}

void loadGunImage(Gun_t *gun, sdl_ctx_t *ctx)
{
    if (gun_textures[(assert(gun->kind < __gun_kind_count), gun->kind)] == NULL) {
        gun_textures[gun->kind] = IMG_LoadTexture(ctx->renderer, gun->img_path);
    }
}

void loadBulletTexture(Gun_t *gun, sdl_ctx_t *ctx)
{
    if (bullet_textures[(assert(gun->kind < __gun_kind_count), gun->kind)] == NULL) {
        bullet_textures[gun->kind] = IMG_LoadTexture(ctx->renderer, gun->bullet_path);
    }
    gun->bullet_texture = bullet_textures[gun->kind];
}

// Interactions
void shootGun(sdl_ctx_t *sdl_ctx, Gun_t *gun, bullets *bullet_arr, V2f position, V2f vel)
{
    // Play sound effect
    if (gun_sfx[gun->kind] != NULL) {
        MIX_PlayAudio(sdl_ctx->mixer, gun_sfx[gun->kind]);
    }
    // avoid the bullets spawning in the ground (mostly a rocket launcher issue but also just looks nicer)
    V2f newPos = (V2f){position.x, position.y -= 10};

    // Create bullets based on gun type
    switch (gun->kind) {
    case PIERCER: // Pistol - Single bullet
        createBullet(bullet_arr, newPos, (V2f){vel.x / 2.f, vel.y / 2.f}, gun->size, gun->bullet_texture, gun->dmg);
        break;

    case SHOTGUN: // Shotgun - Spread of 5 bullets
        for (int i = -2; i <= 2; i++) {
            float angle = i * 0.1f; // Spread angle
            V2f spread_dir = {vel.x * cosf(angle) - vel.y * sinf(angle), vel.x * sinf(angle) + vel.y * cosf(angle)};
            createBullet(bullet_arr, newPos, (V2f){spread_dir.x / 3.f, spread_dir.y / 3.f}, gun->size, gun->bullet_texture,
                         gun->dmg);
        }
        break;

    case MACHINEGUN: // Machine Gun_t - Single fast bullet
        createBullet(bullet_arr, newPos, (V2f){vel.x / 1.5f, vel.y / 1.5f}, gun->size, gun->bullet_texture, gun->dmg);
        break;

    case RAILCANNON: // Sniper - Single powerful bullet
        createBullet(bullet_arr, newPos, (V2f){vel.x / 1.75f, vel.y / 1.75f}, gun->size, gun->bullet_texture, gun->dmg);
        break;

    case SHARPSHOOTER: // Bouncer - Single bullet with bounce effect (would need bullet system enhancement)
        createBullet(bullet_arr, newPos, (V2f){vel.x / 2.f, vel.y / 2.f}, gun->size, gun->bullet_texture, gun->dmg);
        break;

    case ROCKET: // Rocket Launcher - Single slow but powerful bullet
        createBullet(bullet_arr, newPos, (V2f){vel.x / 5.f, vel.y / 5.f}, gun->size, gun->bullet_texture, gun->dmg);
        break;
    default:
        UNREACHABLE("gun kind");
    }
}

// Current gun
int getCurrentGun(Guns_t *guns)
{
    return guns->selectedGun; // At worst it will always return 0
}

void setCurrentGun(Guns_t *guns, gun_kind kind)
{
    assert(guns != NULL);
    guns->selectedGun = kind; // This although does need the assert as assigning something to NULL or 0 will result in segfaults
}

// Destroy functions
void destroyGun(Gun_t *gun)
{
    free(gun->sfx_path);
    gun->sfx_path = NULL;

    free(gun->img_path);
    gun->img_path = NULL;
}

void destroyGuns(Guns_t **guns)
{
    if (*guns != NULL) {
        for (int i = 0; i < __gun_kind_count; ++i) {
            destroyGun(&(*guns)->arsenal[i]);
            SDL_DestroyTexture(gun_textures[i]);
            gun_textures[i] = NULL;
        }
        free((*guns)->arsenal);
        (*guns)->arsenal = NULL;
    }

    free(*guns);
    *guns = NULL;
}

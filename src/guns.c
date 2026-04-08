#include "guns.h"
#include <string.h>
#include <math.h>

static MIX_Audio *gun_sfx[GUN_COUNT] = {0};
static SDL_Texture *gun_textures[GUN_COUNT] = {0};

Guns_t *initialiseGuns(sdl_ctx_t * ctx)
{
    Guns_t *guns = malloc(sizeof(Guns_t));
    if (guns == NULL) {
        return NULL;
    }
    guns->arsenal = malloc(sizeof(Gun_t) * GUN_COUNT);
    if (guns->arsenal == NULL) {
        free(guns);
        return NULL;
    }
    // Basic Pistol
    guns->arsenal[0].name = "Pistol";
    guns->arsenal[0].dmg = 5;
    guns->arsenal[0].size = 20;
    guns->arsenal[0].idx = 0;
    setGunSfx(&guns->arsenal[0], "./assets/audio/SFX/piercer.wav");
    setGunImage(&guns->arsenal[0], "./assets/img/guns/pistol.png");

    // Shotgun
    guns->arsenal[1].name = "Shotgun";
    guns->arsenal[1].dmg = 8;
    guns->arsenal[1].size = 8; 
    guns->arsenal[1].idx = 1;
    setGunSfx(&guns->arsenal[1], "./assets/audio/SFX/shotgun.mp3");
    setGunImage(&guns->arsenal[1], "./assets/img/guns/shotgun.png");

    // Machine Gun_t (Weapon 3 - Rapid fire)
    guns->arsenal[2].name = "Machine Gun_t";
    guns->arsenal[2].dmg = 3;
    guns->arsenal[2].size = 10;
    guns->arsenal[2].idx = 2;
    setGunSfx(&guns->arsenal[2], "./assets/audio/SFX/piercer.wav");
    setGunImage(&guns->arsenal[2], "./assets/img/guns/machinegun.png");

    // Railcannon
    guns->arsenal[3].name = "Sniper Rifle";
    guns->arsenal[3].dmg = 25;
    guns->arsenal[3].size = 30;
    guns->arsenal[3].idx = 3;
    setGunSfx(&guns->arsenal[3], "./assets/audio/SFX/railcannon.mp3");
    setGunImage(&guns->arsenal[3], "./assets/img/guns/sniper.png");

    // sharpshooter (bounce is not implemented since lack of time at the moment)
    guns->arsenal[4].name = "Bouncer";
    guns->arsenal[4].dmg = 6;
    guns->arsenal[4].size = 20; // Special: bouncing effect
    guns->arsenal[4].idx = 4;
    setGunSfx(&guns->arsenal[4], "./assets/audio/SFX/piercer.wav");
    setGunImage(&guns->arsenal[4], "./assets/img/guns/bouncer.png");

    // Rocket Launcher 
    guns->arsenal[5].name = "Rocket Launcher";
    guns->arsenal[5].dmg = 15;
    guns->arsenal[5].size = 50;
    guns->arsenal[5].idx = 5;
    setGunSfx(&guns->arsenal[5], "./assets/audio/SFX/rocket.mp3");
    setGunImage(&guns->arsenal[5], "./assets/img/guns/rocket.png");

    // Load all textures and sounds
    for (int i = 0; i < GUN_COUNT; i++) {
        loadGunSfx(&guns->arsenal[i], ctx);
        loadGunImage(&guns->arsenal[i], ctx);
    }

    guns->selectedGun = 0; // Start with pistol

    printf("gun initialisation complete! \n");
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

void loadGunSfx(Gun_t *gun, sdl_ctx_t * ctx)
{
    if (gun_sfx[(assert(gun->idx < GUN_COUNT), gun->idx)] == NULL) {
        gun_sfx[gun->idx] = MIX_LoadAudio(ctx->mixer, gun->sfx_path, 1);
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

void loadGunImage(Gun_t *gun, sdl_ctx_t *ctx)
{
    if (gun_textures[(assert(gun->idx < GUN_COUNT), gun->idx)] == NULL) {
        gun_textures[gun->idx] = IMG_LoadTexture(ctx->renderer, gun->img_path);
        SDL_SetTextureScaleMode(gun_textures[gun->idx], SDL_SCALEMODE_NEAREST);
    }
}

// Interactions
void shootGun(sdl_ctx_t *sdl_ctx, Gun_t *gun, bullets *bullet_arr, V2f position, V2f vel)
{
    assert(gun != NULL);
    assert(bullet_arr != NULL);
    printf("am apparently shooting the gun \n");
    // Play sound effect
    if (gun_sfx[gun->idx] != NULL) {
        MIX_PlayAudio(sdl_ctx->mixer, gun_sfx[gun->idx]);
    }

    // Create bullets based on gun type
    switch (gun->idx) {
        case 0: // Pistol - Single bullet
            createBullet(bullet_arr, position, (V2f){vel.x/2, vel.y/2}, gun->size, (SDL_Color){0x00, 0x00, 0xFF, 0xFF});
            break;

        case 1: // Shotgun - Spread of 5 bullets
            for (int i = -2; i <= 2; i++) {
                float angle = i * 0.1f; // Spread angle
                V2f spread_dir = {
                    vel.x * cosf(angle) - vel.y * sinf(angle),
                    vel.x * sinf(angle) + vel.y * cosf(angle)
                };
                createBullet(bullet_arr, position, (V2f){spread_dir.x/3, spread_dir.y/3}, gun->size, (SDL_Color){0xFF, 0x95, 0x27, 0xFF});
            }
            break;

        case 2: // Machine Gun_t - Single fast bullet
            createBullet(bullet_arr, position, (V2f){vel.x/1.5, vel.y/1.5}, gun->size, (SDL_Color){0xDB, 0x36, 0x21, 0xFF});
            break;

        case 3: // Sniper - Single powerful bullet
            createBullet(bullet_arr, position, (V2f){vel.x/1.75, vel.y/1.75}, gun->size, (SDL_Color){0xDB, 0x21, 0xD2, 0xFF});
            break;

        case 4: // Bouncer - Single bullet with bounce effect (would need bullet system enhancement)
            createBullet(bullet_arr, position, (V2f){vel.x/2, vel.y/2}, gun->size, (SDL_Color){0xFF, 0x00, 0x00, 0xFF});
            break;

        case 5: // Rocket Launcher - Single slow but powerful bullet
            createBullet(bullet_arr, position, (V2f){vel.x/5, vel.y/5}, gun->size, (SDL_Color){0xFC, 0xFF, 0x00, 0xFF});
            break;
    }
}

// Current gun
int getCurrentGun(Guns_t *guns)
{
    assert(guns != NULL);
    return guns->selectedGun;
}

void setCurrentGun(Guns_t *guns, int idx)
{
    if (guns==NULL){
        printf("gun is somehow NULL i'm going to kill myself here and now :3 \n");
    }
    guns->selectedGun = idx;
    printf("gun is somehow not NULL and i have found a reason to live for \n");
    return;
}

// Destroy functions
void destroyGun(Gun_t **gun)
{
    if (gun == NULL || *gun == NULL) {
        return;
    }
    
    free((void *)(*gun)->sfx_path);
    (*gun)->sfx_path = NULL;
    
    free((void *)(*gun)->img_path);
    (*gun)->img_path = NULL;
    
    free((void *)(*gun)->name);
    (*gun)->name = NULL;
    
    free(*gun);
    *gun = NULL;
}

void destroyGuns(Guns_t **guns)
{
    if (guns == NULL || *guns == NULL) {
        return;
    }
    
    for (int i = 0; i < GUN_COUNT; ++i) {
        free((void *)(*guns)->arsenal[i].sfx_path);
        (*guns)->arsenal[i].sfx_path = NULL;
        
        free((void *)(*guns)->arsenal[i].img_path);
        (*guns)->arsenal[i].img_path = NULL;
        
        free((void *)(*guns)->arsenal[i].name);
        (*guns)->arsenal[i].name = NULL;
    }
    
    free(*guns);
    *guns = NULL;
}
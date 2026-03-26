/**
 * @file main.c
 * @brief Main translation unit to initialise and run the core loop of the game
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-02-02
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Rossignol François <francois_rossignol@outlook.fr>
 * Reeves Guillaume <greeves2306@gmail.com>
 **/

#include "../shared.h"
#include "SDL3/SDL_render.h"
#include "buttons.h"
#include "common.h"
#include "event.h"
#include "file_parsing.h"
#include "player.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "gui.h"
#include <stdlib.h>
#include <math.h>

#define rad2deg(deg) (((deg) / 180) * M_PI))
#define deg2rad(rad) (((rad) / M_PI) * 180))

/**
 * @file main.c
 * @brief File where every actions to run the game are being executed at.
 */
typedef struct {
    SDL_FRect *boundingBox;
    V2f velocity;
} bullet;

typedef struct {
    bullet *items;
    size_t count;
    size_t capacity;
} bullets;

bool createBullet(bullets *arr, V2f init_pos, V2f vel) {
    bullet projectile = {0};
    projectile.boundingBox = createRect(init_pos.x, init_pos.y, 20, 20);
    projectile.velocity.x = vel.x;
    projectile.velocity.y = vel.y;

    da_append(arr, projectile);

    return true;
}

void updateBulletState(bullets *arr, float deltaTime) {
    da_foreach(bullet, it, arr) {
        getBB(it)->x += it->velocity.x * deltaTime;
        getBB(it)->y += it->velocity.y * deltaTime;
    }
}

void renderBullets(sdl_ctx_t *ctx, bullets *arr) {
    da_foreach(bullet, it, arr) {
        renderFillRect(ctx->renderer, it->boundingBox, (SDL_Color){0xFF, 0x00, 0x00, 0xFF});
    }
}

void deleteBullets(bullets *arr) {
    da_foreach(bullet, it, arr)
    {
        free(it->boundingBox);
    }
    free(arr->items);
}

int main(int argc, char **argv)
{
    sdl_ctx_t *sdl_ctx = NULL;
    player_t *player = NULL;
    objs level = {0};

    if (!createCtx(&sdl_ctx)) return 1; // Error handling is done in the function
    if (!createPlayer(&player, (V2f){100, 120}, &sdl_ctx, "assets/img/ourple.png")) return 1;
    movePlayer(player, (V2f){200.0f, 200.0f});
    parseFlag(argc, argv, sdl_ctx, &level);

    V2f fpsTextPos = {10.0f, 10.0f};
    SDL_Texture *logoC = IMG_LoadTexture(sdl_ctx->renderer, "./assets/img/C.png");

    if (logoC == NULL) {
        nob_log(ERROR, "%s:%d: Failed to load image. See error: %s", __FILE__, __LINE__, SDL_GetError());
        destroyPlayer(&player);
        closeCtx(&sdl_ctx);
        return 1;
    }

    //initialisation du menu pause
    button *resumeButton = NULL;
    button *optionsButton = NULL;
    button *quitButton = NULL;

    pauseMenu(sdl_ctx, &resumeButton, &optionsButton, &quitButton);

    SDL_FRect footerBox = {0, (WINDOW_HEIGHT-150) * sdl_ctx->screenRatio, WINDOW_WIDTH * sdl_ctx->screenRatio, 150 * sdl_ctx->screenRatio};

    SDL_FPoint mouseCoord = {0, 0};
    int mouseInputFlag = 0;
    int prevMouseInput = 0;

    bullets bullet_arr = {0};

    Uint32 last = SDL_GetTicks();
    Uint32 frameStart = 0;
    int frameCounter = 0;
    int frameRate = 0;
    float deltaTime = 0;

    size_t mark = temp_save();
    SDL_SetRenderDrawBlendMode(sdl_ctx->renderer, SDL_BLENDMODE_BLEND);

    // Updates the event queue and internal input device state
    while (!sdl_ctx->quit) {
        temp_rewind(mark);
        SDL_SetRenderDrawColor(sdl_ctx->renderer, 0x00, 0x00, 0x00, 0xFF);

        Uint32 now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f; // seconds since last frame
        last = now;

        if (now - frameStart >= 1000) {
            frameRate = frameCounter;
            frameCounter = 0;
            frameStart = now;
        }

        while (SDL_PollEvent(&sdl_ctx->event)) {
            switch (sdl_ctx->event.type) {
            case SDL_EVENT_QUIT:
                sdl_ctx->quit = true;
                break;
            }
        }
        SDL_PumpEvents();

        mouseInputFlag = SDL_GetMouseState(&mouseCoord.x, &mouseCoord.y);
        basicKeyboardEvents(sdl_ctx);

        if (sdl_ctx->pause == false)//stop le joueur et ses input
            UpdatePlayer(player, &level, deltaTime);
        UpdatePlayer(player, &level, deltaTime);

        SDL_RenderClear(sdl_ctx->renderer);
        renderBackground(sdl_ctx);

        if (mouseInputFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT) && !(prevMouseInput & SDL_BUTTON_MASK(SDL_BUTTON_LEFT))) {
            V2f startingPos = (V2f){player->boundingBox->x + player->boundingBox->w / 2.0f, player->boundingBox->y + player->boundingBox->h / 2.0f};
            V2f deltaPos = (V2f){mouseCoord.x - startingPos.x, mouseCoord.y - startingPos.y - 15.0f};
            float magnitude = SDL_sqrt((deltaPos.x * deltaPos.x) + (deltaPos.y * deltaPos.y));
            V2f vel = (V2f){((deltaPos.x / magnitude) * 2500), ((deltaPos.y / magnitude) * 2500)};

            createBullet(&bullet_arr, startingPos, vel);
        }
        // else if (mouseInputFlag & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT) && !(prevMouseInput & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT))) {
        //     printf("Right Clicked\n");
        // }

        updateBulletState(&bullet_arr, deltaTime);
        renderBullets(sdl_ctx, &bullet_arr);
        prevMouseInput = mouseInputFlag;
        renderPlayer(player);

        // Render level textures
        da_foreach(obj, it, &level)
        {
            renderImage(sdl_ctx, it->texture, it->boundingBox);
        }
        // Render the level bounding boxes
        da_foreach(obj, it, &level)
        {
            if (SDL_HasRectIntersectionFloat(player->boundingBox, it->boundingBox)) {
                renderText_Ex(sdl_ctx, "true", WHITE, (V2f){150.0f, 10.0f});
            }
        }

        renderFillRect(sdl_ctx->renderer, &footerBox, (SDL_Color){45, 45, 45, 255});

        //temp text in the top left of the screen to monitor various game variables such as positions
        renderText_Ex(sdl_ctx, temp_sprintf("fps : %i", frameRate), WHITE, fpsTextPos);
        renderText_Ex(sdl_ctx, temp_sprintf("Player: {%.1f, %.1f}", getBB(player)->x, getBB(player)->y), WHITE, (V2f){10.0f, 80.0f});

        if(sdl_ctx->pause == true) {
            update_and_renderPauseMenu(sdl_ctx, &mouseCoord, mouseInputFlag, resumeButton, optionsButton, quitButton);
        }

        renderFillRect(sdl_ctx->renderer, &(SDL_FRect){0.0f, 820.0f, WINDOW_WIDTH, WINDOW_HEIGHT-820.0f}, (SDL_Color){0x00, 0x00, 0x00, 0x18});

        SDL_RenderPresent(sdl_ctx->renderer);
        frameCounter++;
    }
    //destroying all the SDL textures to avoid memory leaks
    da_foreach(obj, it, &level)
    {
        free(it->boundingBox);
        SDL_DestroyTexture(it->texture);
    }
    free(level.items);

    deleteBullets(&bullet_arr);
    destroyPauseMenu(&resumeButton, &optionsButton, &quitButton);
    destroyPlayer(&player);
    closeCtx(&sdl_ctx);
    return 0;
}

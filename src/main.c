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
#include "bullets.h"
#include "common.h"
#include "entity.h"
#include "event.h"
#include "file_parsing.h"
#include "gui.h"
#include "player.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "gui.h"
#include "music.h"
#include <SDL3_mixer/SDL_mixer.h>

#define rad2deg(deg) (((deg) / 180) * M_PI))
#define deg2rad(rad) (((rad) / M_PI) * 180))

/**
 * @file main.c
 * @brief File where every actions to run the game are being executed at.
 */

int main(int argc, char **argv)
{
    sdl_ctx_t *sdl_ctx = NULL;
    player_t *player = NULL;
    objs level = {0};

    if (!createCtx(&sdl_ctx)) return 1; // Error handling is done in the function
    if (!createPlayer(&player, (V2f){100, 120}, &sdl_ctx, "assets/img/V1.png")) return 1;
    movePlayer(player, (V2f){200.0f, 200.0f});
    parseFlag(argc, argv, sdl_ctx, &level);

    gui_menu *pauseMenu = createPauseMenu(sdl_ctx);

    SDL_FRect footerBox = {0, (WINDOW_HEIGHT - 150) * sdl_ctx->screenRatio, WINDOW_WIDTH * sdl_ctx->screenRatio,
                           150 * sdl_ctx->screenRatio};

    V2f mouseCoord = {0};
    int mouseInputFlag = 0;
    int prevMouseInput = 0;

    bullets bullet_arr = {0};
    entity_t *filth = NULL;
    if (!createEntity(&sdl_ctx, &filth, "./assets/img/filth.png", E_FILTH, (V2f){700.0f, 200.0f})) return 1;

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

        SDL_RenderClear(sdl_ctx->renderer);
        renderBackground(sdl_ctx);

        if (!sdl_ctx->paused &&
            (mouseInputFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT) && !(prevMouseInput & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)))) {
            V2f startingPos = (V2f){player->boundingBox->x + player->boundingBox->w / 2.0f,
                                    player->boundingBox->y + player->boundingBox->h / 2.0f};
            V2f deltaPos = (V2f){mouseCoord.x - startingPos.x, mouseCoord.y - startingPos.y - 15.0f};
            float magnitude = SDL_sqrt((deltaPos.x * deltaPos.x) + (deltaPos.y * deltaPos.y));
            V2f vel = (V2f){((deltaPos.x / magnitude) * 2500), ((deltaPos.y / magnitude) * 2500)};

            createBullet(&bullet_arr, startingPos, vel);
            sfx("./assets/audio/SFX/Shoot1.mp3");
        }
        // else if (mouseInputFlag & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT) && !(prevMouseInput & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)))
        // {
        //     printf("Right Clicked\n");
        // }

        updateBulletState(&bullet_arr, deltaTime);
        checkBulletLevelCollisions(&bullet_arr, &level); //collision check between the bullets and env + enemies
        renderBullets(sdl_ctx, &bullet_arr);
        prevMouseInput = mouseInputFlag;

        if (!sdl_ctx->paused) updatePlayer(player, &level, deltaTime);

        renderPlayer(player);

        if (!sdl_ctx->paused) updateEntity(filth, player, &bullet_arr, &level, deltaTime);
        renderEntity(filth);

        // Render level textures
        da_foreach (obj, it, &level) {
            renderImage(sdl_ctx, it->texture, it->boundingBox);
        }
        // Render the level bounding boxes
        da_foreach (obj, it, &level) {
            if (SDL_HasRectIntersectionFloat(player->boundingBox, it->boundingBox)) {
                renderText_Ex(sdl_ctx, "true", WHITE, (V2f){150.0f, 10.0f});
            }
        }

        renderText_Ex(sdl_ctx, temp_sprintf("fps : %d", frameRate), WHITE, (V2f){10.0f, 10.0f});
        renderText_Ex(sdl_ctx, temp_sprintf("bullets: %zu", bullet_arr.count), WHITE, (V2f){10.0f, 40.0f});
        renderText_Ex(sdl_ctx, temp_sprintf("Player: {%.1f, %.1f}", getBB(player)->x, getBB(player)->y), WHITE,
                      (V2f){10.0f, 80.0f});

        // Everything after the footer being rendered is rendered OVER it.
        renderFillRect(sdl_ctx->renderer, &footerBox, (SDL_Color){45, 45, 45, 255});

        // Update and render the menu at the very end
        if (sdl_ctx->paused == true) {
            updateMenu(sdl_ctx, mouseCoord, mouseInputFlag, pauseMenu, updatePauseMenu);
            renderMenu(sdl_ctx, pauseMenu);
            MIX_PauseTrack(sdl_ctx->track);
        }
        SDL_RenderPresent(sdl_ctx->renderer);
        frameCounter++;
    }
    // destroying all the SDL textures to avoid memory leaks
    da_foreach (obj, it, &level) {
        free(it->boundingBox);
        SDL_DestroyTexture(it->texture);
    }
    free(level.items);

    MIX_Quit();
    destroyEntity(&filth);
    deleteBullets(&bullet_arr);
    destroyMenu(&pauseMenu);
    destroyPlayer(&player);
    closeCtx(&sdl_ctx);
    return 0;
}

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
#include "common.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "event.h"
#include "file_parsing.h"
#include "level.h"
#include "player.h"
#include "bullets.h"
#include "gui.h"
#include "bars.h"
#include "sliders.h"
#include "checkboxes.h"
#include "music.h"

/**
 * @file main.c
 * @brief File where every actions to run the game are being executed at.
 */

int main(int argc, char **argv)
{
    V2f mouseCoord = {0};
    int mouseInputFlag = 0;
    int prevMouseInput = 0;
    size_t mark = temp_save();

    sdl_ctx_t *sdl_ctx = NULL;

    level_t *level = NULL;
    player_t *player = NULL;
    bullets bullet_arr = {0};

    if (!createCtx(&sdl_ctx)) return 1; // Error handling is done in the function
    if (!createPlayer(&player, (V2f){100, 120}, &sdl_ctx, "assets/img/V1.png")) return 1;
    parseFlag(argc, argv, sdl_ctx, &level);
    movePlayer(player, level->items[level->currentLoadedRoomID]->startPos);

    bar *hpBar = NULL;
    if (!createBar(
            &hpBar,
            (SDL_FRect){(WINDOW_WIDTH / 2.0f - 150.0f) * sdl_ctx->screenRatio, (WINDOW_HEIGHT - 95.0f) * sdl_ctx->screenRatio,
                        450 * sdl_ctx->screenRatio, 60.0f * sdl_ctx->screenRatio},
            (SDL_Color){20, 20, 20, 255}, (SDL_Color){178, 19, 19, 255}, (SDL_Color){255, 255, 255, 255}, 100.0f, 10.0f))
        return 1;

    gui_menu *pauseMenu = createPauseMenu(sdl_ctx);

    SDL_FRect footerBox = {0, (WINDOW_HEIGHT - 150) * sdl_ctx->screenRatio, WINDOW_WIDTH * sdl_ctx->screenRatio,
                           150 * sdl_ctx->screenRatio};

    slider *sTest = NULL;
    createSlider(sdl_ctx, &sTest,
                 (SDL_FRect){20 * sdl_ctx->screenRatio, 200 * sdl_ctx->screenRatio, 512 * sdl_ctx->screenRatio,
                             64 * sdl_ctx->screenRatio},
                 NULL, NULL, 10, 10.0f * sdl_ctx->screenRatio);

    checkbox *cTest = NULL;
    createCheckbox(sdl_ctx, &cTest,
                    (SDL_FRect){600 * sdl_ctx->screenRatio, 20 * sdl_ctx->screenRatio, 256 * sdl_ctx->screenRatio, 256 * sdl_ctx->screenRatio},
                    (SDL_FRect){620 * sdl_ctx->screenRatio, 40 * sdl_ctx->screenRatio, 216 * sdl_ctx->screenRatio, 216 * sdl_ctx->screenRatio},
                    NULL, NULL, NULL,
                    10.0f * sdl_ctx->screenRatio,
                    20.0f * sdl_ctx->screenRatio);

    Uint32 last = SDL_GetTicks();
    Uint32 frameStart = 0;
    int frameCounter = 0;
    int frameRate = 0;
    float deltaTime = 0;

    float healthBarTest = 0;
    bool addition = true;

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
        basicKeyboardEvents(sdl_ctx, level, player);

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
        updateBulletState(&bullet_arr, deltaTime);
        renderBullets(sdl_ctx, &bullet_arr);
        prevMouseInput = mouseInputFlag;

        if (!sdl_ctx->paused) {
            updatePlayer(player, &level, deltaTime);
            updateEntities(&e_bundle, player, &bullet_arr, &level, deltaTime);
        }

        renderPlayer(player);
        renderEntities(&e_bundle);

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

        // Render the currently loaded level
        renderRoom(sdl_ctx, level);

        // Everything after the footer being rendered is rendered OVER it.
        renderFillRect(sdl_ctx->renderer, &footerBox, (SDL_Color){45, 45, 45, 255});
        barRender(sdl_ctx, hpBar, healthBarTest, 50, 50, 50);

        // test de slider
        updateSliderStates(sTest, mouseCoord, mouseInputFlag, sdl_ctx);
        renderSlider(sdl_ctx, sTest);

        // test de checkbox
        updateCheckboxStates(cTest, mouseCoord, mouseInputFlag);
        renderCheckbox(sdl_ctx, cTest, "checkbox test");

        // Update and render the menu at the very end
        if (sdl_ctx->paused == true) {
            updateMenu(sdl_ctx, mouseCoord, mouseInputFlag, pauseMenu, updatePauseMenu);
            renderMenu(sdl_ctx, pauseMenu);
            MIX_PauseTrack(sdl_ctx->track);
        }

        SDL_RenderPresent(sdl_ctx->renderer);

        frameCounter++;
        if (healthBarTest < 0 && !addition) addition = true;
        if (healthBarTest > 100 && addition) addition = false;

        if (addition) healthBarTest += 10 * deltaTime;
        else
            healthBarTest -= 10 * deltaTime;
    }

    destroyBar(&hpBar);
    destroySliders(&sTest);
    destroyCheckbox(&cTest);
    destroyLevel(&level);
    deleteBullets(&bullet_arr);
    destroyMenu(&pauseMenu);
    destroyPlayer(&player);
    closeCtx(&sdl_ctx);
    MIX_Quit();
    return 0;
}

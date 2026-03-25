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
#include "buttons.h"
#include "common.h"
#include "event.h"
#include "file_parsing.h"
#include "player.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "gui.h"
#include <stdlib.h>

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
    if (!createPlayer(&player, (V2f){40, 80}, &sdl_ctx, "assets/img/ourple.png")) return 1;
    movePlayer(player, (V2f){200.0f, 200.0f});
    parseFlag(argc, argv, sdl_ctx, &level);

    // obj_create(&obj_arr, sdl_ctx, "./assets/img/white.png", 0.0f, 500.0f, 1000.f, 500.f);
    // obj_create(&obj_arr, sdl_ctx, "./assets/img/white.png", 200.0f, 300.0f, 140.0f, 400.0f);
    // obj_create(&obj_arr, sdl_ctx, "./assets/img/white.png", 400.0f, 0.0f, 100.0f, 300.0f);
    // obj_create(&obj_arr, sdl_ctx, "./assets/img/white.png", -10.0f, 0.0f, 15.f, 600.f);
    // obj_create(&obj_arr, sdl_ctx, "./assets/img/white.png", 0.0f, -10.0f, WINDOW_WIDTH, 15.0f);
    Uint32 last = SDL_GetTicks();
    float deltaT = 0;

    size_t pixelWidth = 0;
    TTF_MeasureString(sdl_ctx->font, temp_sprintf("x = %d.0, y = %d.0", WINDOW_WIDTH, WINDOW_HEIGHT), 0, 0, NULL, &pixelWidth);
    V2f fpsTextPos = {10.0f, 10.0f};
    V2f MouseTextPos = {10.0f, 48.0f};
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
    int mouseInputFlag;

    Uint32 frameStart = 0;
    int frameCounter = 0;
    int frameRate = 0;

    size_t mark = temp_save();

    // Updates the event queue and internal input device state
    while (!sdl_ctx->quit) {
        temp_rewind(mark);
        Uint32 now = SDL_GetTicks();
        deltaT = (now - last) / 1000.0f; // seconds since last frame
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

        basicKeyboardEvents(sdl_ctx);

        if (sdl_ctx->pause == false)//stop le joueur et ses input
            UpdatePlayer(player, &level, deltaT);

        SDL_RenderClear(sdl_ctx->renderer);
        renderBackground(sdl_ctx);

        mouseInputFlag = SDL_GetMouseState(&(mouseCoord.x), &(mouseCoord.y));//ne pas toucher a cette ligne !
        
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

        SDL_SetRenderDrawColor(sdl_ctx->renderer, 45, 45, 45, 255);
        SDL_RenderFillRect(sdl_ctx->renderer, &footerBox);
        SDL_SetRenderDrawColor(sdl_ctx->renderer, 0, 0, 0, 255);

        //temp text in the top left of the screen to monitor various game variables such as positions

        renderText_Ex(sdl_ctx, temp_sprintf("fps : %i", frameRate), WHITE, fpsTextPos);
        renderText_Ex(sdl_ctx, temp_sprintf("Mouse: {%.1f, %.1f}", mouseCoord.x, mouseCoord.y), WHITE, MouseTextPos);
        renderText_Ex(sdl_ctx, temp_sprintf("Dash: %i", player->dashAmount), WHITE, (V2f){10.0f, 110.0f});
        renderText_Ex(sdl_ctx, temp_sprintf("DashT: %.2f", player->dashTimer), WHITE, (V2f){10.0f, 140.0f});
        renderText_Ex(sdl_ctx, temp_sprintf("Player: {%.1f, %.1f}", getBB(player)->x, getBB(player)->y), WHITE,
                      (V2f){10.0f, 80.0f});
        renderText(sdl_ctx, temp_sprintf("DeltaT : %f", deltaT), WHITE, 10, 170);

        if(sdl_ctx->pause == true) {
            update_and_renderPauseMenu(sdl_ctx, &mouseCoord, mouseInputFlag, resumeButton, optionsButton, quitButton);
        }

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

    destroyPauseMenu(&resumeButton, &optionsButton, &quitButton);

    destroyPlayer(&player);
    closeCtx(&sdl_ctx);
    return 0;
}

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
#include <math.h>

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

    SDL_FRect *lineX = createRect(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, 1.0f, 1.0f);
    SDL_FRect *lineY = createRect(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, 1.0f, 1.0f);

    float mouse_X = 0;
    float mouse_Y = 0;
    SDL_FRect *boxBouton1 = createRect(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2, 300.0f, 75.0f);
    SDL_Color baseColor_btn1 = {0, 0, 255, 255};
    SDL_Color hoverColor_btn1 = {255, 10, 100, 255};
    SDL_Color clickColor_btn1 = {200, 10, 100, 255};
    char *textBtn1 = "btn 1";

    button *bouton1 = initButton(boxBouton1, textBtn1, &baseColor_btn1, &hoverColor_btn1, &clickColor_btn1);

    SDL_FPoint mouseCoord = {0, 0};
    int mouseInputFlag;
    // float mouse_X = 0;
    // float mouse_Y = 0;

    Uint32 frameStart = 0;
    int frameCounter = 0;
    int frameRate = 0;

    size_t mark = temp_save();

    // Updates the event queue and internal input device state
    while (!sdl_ctx->quit) {
        temp_rewind(mark);
        SDL_SetRenderDrawColor(sdl_ctx->renderer, 0x00, 0x00, 0x00, 0xFF);

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
        UpdatePlayer(player, &level, deltaT);

        SDL_RenderClear(sdl_ctx->renderer);
        renderBackground(sdl_ctx);

        updateButtonState(bouton1, mouseCoord, mouseInputFlag);
        buttonRender(sdl_ctx, bouton1);

        renderPlayer(player);

        // Render level textures
        da_foreach(obj, it, &level)
        {
            renderImage(sdl_ctx, it->texture, it->boundingBox);
        }

        da_foreach(obj, it, &level)
        {
            if (SDL_HasRectIntersectionFloat(player->boundingBox, it->boundingBox)) {
                renderText_Ex(sdl_ctx, "true", WHITE, (V2f){150.0f, 10.0f});
            }
        }

        SDL_GetMouseState(&mouse_X, &mouse_Y);
        renderText_Ex(sdl_ctx, temp_sprintf("fps : %i", frameRate), WHITE, fpsTextPos);
        renderText_Ex(sdl_ctx, temp_sprintf("Mouse: {%.1f, %.1f}", mouse_X, mouse_Y), WHITE, MouseTextPos);
        renderText_Ex(sdl_ctx, temp_sprintf("Dash: %i", player->dashAmount), WHITE, (V2f){10.0f, 110.0f});
        renderText_Ex(sdl_ctx, temp_sprintf("DashT: %.2f", player->dashTimer), WHITE, (V2f){10.0f, 140.0f});
        renderText_Ex(sdl_ctx, temp_sprintf("Player: {%.1f, %.1f}", getBB(player)->x, getBB(player)->y), WHITE,
                      (V2f){10.0f, 80.0f});

        SDL_SetRenderDrawColor(sdl_ctx->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        // X
        lineX->w = lineX->x + (mouse_X - WINDOW_WIDTH);

        // Y
        lineY->x = lineX->x + lineX->w;
        lineY->h = lineX->h + (mouse_Y - WINDOW_HEIGHT / 2.0f);

        // Hypotenus
        SDL_RenderLine(sdl_ctx->renderer, lineX->x, lineX->y, lineY->x, mouse_Y);

        renderText_Ex(sdl_ctx, temp_sprintf("height: %.2f", lineY->h), WHITE, (V2f){10.0f, 148.0f});
        SDL_RenderFillRect(sdl_ctx->renderer, lineX);
        SDL_RenderFillRect(sdl_ctx->renderer, lineY);

        SDL_RenderPresent(sdl_ctx->renderer);
        frameCounter++;
    }

    da_foreach(obj, it, &level)
    {
        free(it->boundingBox);
        SDL_DestroyTexture(it->texture);
    }
    free(level.items);
    free(lineX);
    free(lineY);

    destroyPlayer(&player);
    closeCtx(&sdl_ctx);
    return 0;
}

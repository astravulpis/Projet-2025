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
#include "event.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "player.h"
#include "buttons.h"

bool createImageRect();

int main()
{
    sdl_ctx_t *sdl_ctx = NULL;
    if (!createCtx(&sdl_ctx)) return 1; // Error handling is done in the function

    player_t *player = NULL;
    if (!createPlayer(&player, (V2f){100.0f, 100.0f}, &sdl_ctx, "assets/img/C.png")) return 1;

    Uint32 last = SDL_GetTicks();
    float deltaT = 0;

    size_t pixelWidth = 0;
    TTF_MeasureString(sdl_ctx->font, temp_sprintf("x = %d.0, y = %d.0", WINDOW_WIDTH, WINDOW_HEIGHT), 0, 0, NULL, &pixelWidth);
    V2f fpsTextPos = {10.0f, 10.0f};
    V2f MouseTextPos = {10.0f, 48.0f};
    SDL_FRect *boxDummy = createRect(400.0f, 400.0f, 50.0f, 50.0f);
    SDL_Texture *logoC = IMG_LoadTexture(sdl_ctx->renderer, "./assets/img/C.png");

    if (logoC == NULL) {
        nob_log(ERROR, "%s:%d: Failed to load image. See error: %s", __FILE__, __LINE__, SDL_GetError());
        destroyPlayer(&player);
        closeCtx(&sdl_ctx);
        return 1;
    }

    SDL_FRect *boxBouton1 = createRect(WINDOW_WIDTH/2-150, WINDOW_HEIGHT/2, 300.0f, 75.0f);
    SDL_Color baseColor_btn1 = {0, 0, 255, 255};
    SDL_Color hoverColor_btn1 = {255, 10, 100, 255};
    SDL_Color clickColor_btn1 = {200, 10, 100, 255};
    char *textBtn1 = "btn 1";

    button *bouton1 = initButton(boxBouton1, textBtn1, &baseColor_btn1, &hoverColor_btn1, &clickColor_btn1);

    SDL_FPoint mouseCoord = {0, 0};
    int mouseInputFlag;
    //float mouse_X = 0;
    //float mouse_Y = 0;

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
        UpdatePlayer(player, boxDummy, deltaT);
        
        //je l'ai bougé car il semble plus adapté ici
        mouseInputFlag = SDL_GetMouseState(&(mouseCoord.x), &(mouseCoord.y));

        SDL_RenderClear(sdl_ctx->renderer);
        renderBackground(sdl_ctx);

        updateButtonState(bouton1, mouseCoord, mouseInputFlag);
        buttonRender(sdl_ctx, bouton1);

        renderPlayer(player);
        renderImage(sdl_ctx, logoC, boxDummy);

        renderText_Ex(sdl_ctx, temp_sprintf("Mouse: {%.1f, %.1f}", mouseCoord.x, mouseCoord.y), WHITE, MouseTextPos);
        renderText_Ex(sdl_ctx, temp_sprintf("fps : %i", frameRate), WHITE, fpsTextPos);
        // renderText_Ex(sdl_ctx, temp_sprintf("timer: %.1f", player->stunnedTimer), WHITE, (V2f){10.0f, 148.0f});

        SDL_RenderPresent(sdl_ctx->renderer);
        frameCounter++;
    }

    free(boxDummy);
    destroyButton(&bouton1);

    destroyPlayer(&player);
    closeCtx(&sdl_ctx);
    return 0;
}

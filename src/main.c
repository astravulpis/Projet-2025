#include "../shared.h"
#include "common.h"
#include "event.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "player.h"
#include <stdlib.h>

#define GRAVITY 5.0f

/**
 * @file main.c
 * @brief File where every actions to run the game are being executed at.
 */
bool createImageRect();

int main()
{
    int level_selector;
    printf("give me the level you want to choose: ");   //choosing the level like this for now but in the future will be pulled from a file
    scanf("%d", &level_selector);
    
    int image_count = 0;
    SDL_FRect boxes[10];
    SDL_Texture *textures[10];
    
    
    sdl_ctx_t *sdl_ctx = NULL;
    if (!createCtx(&sdl_ctx)) return 1; // Error handling is done in the function
    player_t *player = NULL;
    if (!createPlayer(&player, (V2f){100.0f, 100.0f}, &sdl_ctx, "assets/img/C.png")) return 1;

    switch (level_selector) {//this is the level selector, very barebones for now but this can be scaled up very well
    case 1:
        textures[0] = IMG_LoadTexture(sdl_ctx->renderer, "./assets/img/C.png");
        boxes[0] = (SDL_FRect){100.0f, 200.0f, 50.0f, 50.0f};
        textures[1] = IMG_LoadTexture(sdl_ctx->renderer, "./assets/img/Texturelabs_Stone_170M.jpg");
        boxes[1] = (SDL_FRect){0.0f, WINDOW_HEIGHT-50, WINDOW_WIDTH, 50.0f};
        image_count = 2;
        loadBackgroundImage(sdl_ctx, "assets/img/Texturelabs_Atmosphere_201M.jpg");
        break;
    case 2:
        textures[0] = IMG_LoadTexture(sdl_ctx->renderer,"assets/img/C.png");
        boxes[0] = (SDL_FRect){100.0f, 200.0f, 50.0f, 50.0f};
        textures[1] = IMG_LoadTexture(sdl_ctx->renderer, "assets/img/V1.png");
        boxes[1] = (SDL_FRect){160.0f, 200.0f, 50.0f, 50.0f};
        textures[1] = IMG_LoadTexture(sdl_ctx->renderer, "./assets/img/Texturelabs_Stone_170M.jpg");
        boxes[1] = (SDL_FRect){0.0f, WINDOW_HEIGHT-50, WINDOW_WIDTH, 50.0f};
        image_count = 3;
        loadBackgroundImage(sdl_ctx, "assets/img/Texturelabs_Water_141M.jpg");
        break;
    default:
        printf("Invalid level selector\n");
        return 1;
    }
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

    float mouse_X = 0;
    float mouse_Y = 0;

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

        if (player->onGround == true) sdl_ctx->quit=true;

        basicKeyboardEvents(sdl_ctx);
        UpdatePlayer(player, boxes, image_count, deltaT);

        SDL_RenderClear(sdl_ctx->renderer);
        renderBackground(sdl_ctx);

        renderPlayer(player);

        // Render level textures
        for (int i = 0; i < image_count; i++) { //rendering all hitboxes on the textures
            renderImage(sdl_ctx, textures[i], &boxes[i]);
        }

        SDL_GetMouseState(&mouse_X, &mouse_Y);
        renderText_Ex(sdl_ctx, temp_sprintf("Mouse: {%.1f, %.1f}", mouse_X, mouse_Y), WHITE, MouseTextPos);
        renderText_Ex(sdl_ctx, temp_sprintf("fps : %i", frameRate), WHITE, fpsTextPos);
        // renderText_Ex(sdl_ctx, temp_sprintf("timer: %.1f", player->stunnedTimer), WHITE, (V2f){10.0f, 148.0f});

        SDL_RenderPresent(sdl_ctx->renderer);
        frameCounter++;
    }

    destroyPlayer(&player);
    closeCtx(&sdl_ctx);
    return 0;
}

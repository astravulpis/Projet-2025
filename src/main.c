#include "../shared.h"
#include "common.h"
#include "event.h"
#include "file_parsing.h"
#include "player.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
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

        basicKeyboardEvents(sdl_ctx);
        UpdatePlayer(player, &level, deltaT);

        SDL_RenderClear(sdl_ctx->renderer);
        renderBackground(sdl_ctx);

        renderPlayer(player);

        // Render level textures
        da_foreach(obj, it, &level) {
            renderImage(sdl_ctx, it->texture, it->boundingBox);
        }

        da_foreach(obj, it, &level) {
            if (SDL_HasRectIntersectionFloat(player->boundingBox, it->boundingBox)) {
                renderText_Ex(sdl_ctx, "true", WHITE, (V2f){150.0f, 10.0f});
            }
        }

        SDL_GetMouseState(&mouse_X, &mouse_Y);
        renderText_Ex(sdl_ctx, temp_sprintf("fps : %i", frameRate), WHITE, fpsTextPos);
        renderText_Ex(sdl_ctx, temp_sprintf("Mouse: {%.1f, %.1f}", mouse_X, mouse_Y), WHITE, MouseTextPos);
        renderText_Ex(sdl_ctx, temp_sprintf("Player: {%.1f, %.1f}", getBB(player)->x, getBB(player)->y), WHITE,
                      (V2f){10.0f, 80.0f});

        SDL_RenderPresent(sdl_ctx->renderer);
        frameCounter++;
    }

    da_foreach(obj, it, &level)
    {
        free(it->boundingBox);
        SDL_DestroyTexture(it->texture);
    }
    free(level.items);

    destroyPlayer(&player);
    closeCtx(&sdl_ctx);
    return 0;
}

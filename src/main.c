#include "../shared.h"
#include "common.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "player.h"

/**
 * @file main.c
 * @brief File where every actions to run the game are being executed at.
 */
bool createImageRect();

int main()
{
    sdl_ctx_t *sdl_ctx = NULL;
    if (!createCtx(&sdl_ctx)) return 1; // Error handling is done in the function

    Uint32 last = SDL_GetTicks();
    float deltaT = 0;

    size_t pixelWidth = 0;
    TTF_MeasureString(sdl_ctx->font, temp_sprintf("x = %d.0, y = %d.0", WINDOW_WIDTH, WINDOW_HEIGHT), 0, 0, NULL, &pixelWidth);
    V2f fpsTextPos = {10.0f, 10.0f};
    V2f MouseTextPos = {10.0f, 48.0f};
    SDL_FRect *boxDummy = createRect(400.0f, 400.0f, 50.0f, 50.0f);
    SDL_FRect *boxC = createRect(350.0f, 200.0f, 100.0f, 100.0f);

    SDL_Texture *C_Logo = IMG_LoadTexture(sdl_ctx->renderer, "assets/img/C.png");
    if (!C_Logo) {
        nob_log(ERROR, "%s:%d: Failed to load C logo image", __FILE__, __LINE__);
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

        UpdatePlayer(sdl_ctx, boxC, boxDummy, deltaT);

        SDL_RenderClear(sdl_ctx->renderer);

        renderBackground(sdl_ctx);

        renderImage(sdl_ctx, C_Logo, boxC);
        renderImage(sdl_ctx, C_Logo, boxDummy);

        renderText_Ex(sdl_ctx, temp_sprintf("fps : %i", frameRate), WHITE, fpsTextPos);

        SDL_GetMouseState(&mouse_X, &mouse_Y);
        renderText_Ex(sdl_ctx, temp_sprintf("Mouse: {%.1f, %.1f}", mouse_X, mouse_Y), WHITE, MouseTextPos);

        SDL_RenderPresent(sdl_ctx->renderer);
        frameCounter++;
    }

    SDL_DestroyTexture(C_Logo);
    free(boxC);
    free(boxDummy);

    closeCtx(&sdl_ctx);
    return 0;
}

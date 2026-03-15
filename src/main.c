#include "../shared.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "common.h"
#include "event.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include <math.h>

/**
 * @file main.c
 * @brief File where every actions to run the game are being executed at.
 */
bool createImageRect();

int main()
{
    float speed = 500.0f;
    float dx, dy;

    sdl_ctx_t *sdl_ctx = NULL;
    if (!createCtx(&sdl_ctx)) return 1; // Error handling is done in the function

    Uint32 last = SDL_GetTicks();
    float deltaT = 0;

    size_t pixelWidth = 0;
    TTF_MeasureString(sdl_ctx->font, temp_sprintf("x = %d.0, y = %d.0", WINDOW_WIDTH, WINDOW_HEIGHT), 0, 0, NULL, &pixelWidth);
    V2f boxFps = {10.0f, 10.0f};
    V2f boxMouse = {10.0f, 48.0f};
    SDL_FRect *boxDummy = createRect(400.0f, 400.0f, 50.0f, 50.0f);
    SDL_FRect *boxC = createRect(350.0f, 200.0f, 100.0f, 100.0f);

    SDL_Texture *C_Logo = IMG_LoadTexture(sdl_ctx->renderer, "assets/img/C.bmp");
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

        dx = 0.0f;
        dy = 0.0f;

        // calls basic_movement with the adress of the image we are moving, it's X and Y coordinates
        basicMovementEvents(sdl_ctx, &dx, &dy);

        if (dx != 0 && dy != 0) {
            boxC->x += dx * speed * deltaT / sqrt(2);
            boxC->y += dy * speed * deltaT / sqrt(2);
        } else {
            boxC->x += dx * speed * deltaT;
            boxC->y += dy * speed * deltaT;
        }

        // resolveOverlap(boxC, boxDummy);
        keepPlayerInbound(boxC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        SDL_RenderClear(sdl_ctx->renderer);

        renderBackground(sdl_ctx);

        renderImage(sdl_ctx, C_Logo, boxC);
        renderImage(sdl_ctx, C_Logo, boxDummy);

        renderText_Ex(sdl_ctx, temp_sprintf("fps : %i", frameRate), WHITE, boxFps);

        SDL_GetMouseState(&mouse_X, &mouse_Y);
        renderText_Ex(sdl_ctx, temp_sprintf("Mouse: {%.1f, %.1f}", mouse_X, mouse_Y), WHITE, boxMouse);

        SDL_RenderPresent(sdl_ctx->renderer);
        frameCounter++;
    }

    SDL_DestroyTexture(C_Logo);
    free(boxC);
    free(boxDummy);

    closeCtx(&sdl_ctx);
    return 0;
}

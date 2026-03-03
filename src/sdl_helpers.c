#include "sdl_helpers.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "common.h"
#include <stdlib.h>

sdl_ctx_t *init_all(void)
{
    sdl_ctx_t *ctx = NULL;
    ctx = malloc(sizeof(sdl_ctx_t));
    if (!ctx) return NULL;
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        return NULL;
    }
    SDL_CreateWindowAndRenderer("ULTRAC00L", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &(ctx->window), &(ctx->renderer));
    if (ctx->window == NULL) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        close_SDL(ctx);
        return NULL;
    }
    ctx->vsyncActivation=true;
    //Activation du Vsync pour avoir un contrôle du framerate et éviter une surcharge du pc
    if (SDL_SetRenderVSync(ctx->renderer, 1) == false) {
        SDL_Log( "Impossible d'initialiser VSync, erreur : %s\n", SDL_GetError() );
        close_SDL(ctx);
        return NULL;
    }

    ctx->bgSurface = SDL_GetWindowSurface(ctx->window);
    return ctx;
}

void close_SDL(sdl_ctx_t *sdl_ctx)
{
    if (sdl_ctx) { // If it isn't NULL
        SDL_DestroyWindow(sdl_ctx->window);
        SDL_DestroyRenderer(sdl_ctx->renderer);
        SDL_DestroySurface(sdl_ctx->bgSurface);
        SDL_DestroyTexture(sdl_ctx->bgTexture);

        sdl_ctx->window = NULL;
        sdl_ctx->renderer = NULL;
        sdl_ctx->bgSurface = NULL;
        sdl_ctx->bgTexture = NULL;
    }

    // Freeing a NULL is fine
    free(sdl_ctx);

    SDL_Quit();
}

void initBackgroundColor(sdl_ctx_t *sdl_ctx, int r, int g, int b)
{
    sdl_ctx->bgSurface = SDL_GetWindowSurface(sdl_ctx->window);
    SDL_FillSurfaceRect(sdl_ctx->bgSurface, NULL, SDL_MapSurfaceRGB(sdl_ctx->bgSurface, r, g, b));
    sdl_ctx->bgTexture = SDL_CreateTextureFromSurface(sdl_ctx->renderer, sdl_ctx->bgSurface);
}

void updateBackgroundColor(sdl_ctx_t *sdl_ctx, int r, int g, int b)
{
    SDL_DestroySurface(sdl_ctx->bgSurface);
    SDL_DestroyTexture(sdl_ctx->bgTexture);

    SDL_FillSurfaceRect(sdl_ctx->bgSurface, NULL, SDL_MapSurfaceRGB(sdl_ctx->bgSurface, r, g, b));

    sdl_ctx->bgTexture = SDL_CreateTextureFromSurface(sdl_ctx->renderer, sdl_ctx->bgSurface);
}

void renderBackground(sdl_ctx_t *sdl_ctx)
{
    if (sdl_ctx->bgTexture != NULL) SDL_RenderTexture(sdl_ctx->renderer, sdl_ctx->bgTexture, NULL,  NULL);
}


/**
 * @file load_sdl3.c
 * @brief First unit test to make sure SDL3 was correctly included into the project
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-30
 * Date: 2026-02-30
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 **/

#include "../shared.h"
#include "SDL3/SDL_render.h"
#include "common.h"
#include "file_parsing.h"
#include "level.h"

static bool createCustomCtx(sdl_ctx_t **ctx);
static bool initCustomCtx(sdl_ctx_t *sdl_ctx);
static void closeCustomCtx(sdl_ctx_t **sdl_ctx);

static bool createCustomCtx(sdl_ctx_t **ctx)
{
    (*ctx) = calloc(1, sizeof(sdl_ctx_t));
    if ((*ctx) == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for sdl_ctx_t. Critical error!", __FILE__, __LINE__);
        return false;
    }

    if (!initCustomCtx((*ctx))) return false;

    return true;
}

static bool initCustomCtx(sdl_ctx_t *sdl_ctx)
{
    bool result = true; // true by default to allow fall-through pass the `defer` label

    if (!SDL_Init(SDL_INIT_EVENTS)) {
        nob_log(ERROR, "%s:%d: SDL failed to initialize. See: %s", __FILE__, __LINE__, SDL_GetError());
        return_defer(false);
    }

    SDL_CreateWindowAndRenderer("ULTRAC00L", WINDOW_WIDTH, WINDOW_HEIGHT, 0x0, &(sdl_ctx->window), &(sdl_ctx->renderer));
    if (!sdl_ctx->window) {
        nob_log(ERROR, "%s:%d: SDL failed to create window and renderer. See: %s", __FILE__, __LINE__, SDL_GetError());
        return_defer(false);
    }

defer:
    if (result == false) {
        closeCustomCtx(&sdl_ctx);
    }

    return result;
}

static void closeCustomCtx(sdl_ctx_t **sdl_ctx)
{
    if ((*sdl_ctx) != NULL) {

        if ((*sdl_ctx)->renderer != NULL) {
            SDL_DestroyRenderer((*sdl_ctx)->renderer);
            (*sdl_ctx)->renderer = NULL;
        }

        if ((*sdl_ctx)->window != NULL) {
            SDL_DestroyWindow((*sdl_ctx)->window);
            (*sdl_ctx)->window = NULL;
        }
    }

    // Freeing a NULL is fine
    free(*sdl_ctx);
    sdl_ctx = NULL;
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    level_t *level = NULL;
    sdl_ctx_t *ctx = NULL;
    int result = 0;
    if (!createCustomCtx(&ctx)) return_defer(1);
    if (!parseFlag(argc, argv, &ctx, &level)) return_defer(1);
    destroyLevel(&level);

    printf("everything went well\n");
defer:
    closeCustomCtx(&ctx);
    return result;
}

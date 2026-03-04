#include "sdl_helpers.h"
#include "SDL3/SDL_rect.h"
#include "common.h"
#include <stdlib.h>

sdl_ctx_t *init_all(void)
{
    sdl_ctx_t *ctx = NULL;
    SDL_WindowFlags windowFlags = 0x0; // Pour ajouter des flags, utiliser le `OR` binaire, soit `|`. e.g: SDL_[FLAG1] | SDL_[FLAG2] | SDL_[FLAG3]

    ctx = malloc(sizeof(sdl_ctx_t));
    if (!ctx) return NULL;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        return NULL;
    }

    SDL_CreateWindowAndRenderer("ULTRAC00L", WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags, &(ctx->window), &(ctx->renderer));
    if (!ctx->window) {
        nob_log(ERROR, "SDL failed to create window and renderer. See: %s", SDL_GetError());
        close_SDL(ctx);
        return NULL;
    }

    if (!enableVsync(ctx)) {
        close_SDL(ctx);
        return NULL;
    }
    ctx->quit = false;

    return ctx;
}

void close_SDL(sdl_ctx_t *sdl_ctx)
{
    if (sdl_ctx) { // If it isn't NULL
        SDL_DestroyWindow(sdl_ctx->window);
        SDL_DestroyRenderer(sdl_ctx->renderer);

        sdl_ctx->window = NULL;
        sdl_ctx->renderer = NULL;
        clearContextSurface(sdl_ctx);
    }

    // Freeing a NULL is fine
    free(sdl_ctx);

    SDL_Quit();
}

SDL_FRect *createRect(float x, float y, float width, float height)
{
    SDL_FRect *res = NULL;
    if ((res = malloc(sizeof(SDL_FRect))) == NULL) {
        nob_log(ERROR, "failed to create a rect. Please buy more ram.");
        return NULL;
    }

    res->x = x;
    res->y = y;
    res->w = width;
    res->h = height;

    return res;
}

bool enableVsync(sdl_ctx_t *sdl_ctx)
{
    bool res = true;
    sdl_ctx->vsyncActivation = true;
    //Activation du Vsync pour avoir un contrôle du framerate et éviter une surcharge du pc
    if (!SDL_SetRenderVSync(sdl_ctx->renderer, 1)) {
        nob_log(ERROR, "Impossible d'activer la VSync. Voir: %s", SDL_GetError());
        res = false;
    }

    return res;
}

bool disableVsync(sdl_ctx_t *sdl_ctx)
{
    bool res = true;
    sdl_ctx->vsyncActivation = false;
    //Activation du Vsync pour avoir un contrôle du framerate et éviter une surcharge du pc
    if (!SDL_SetRenderVSync(sdl_ctx->renderer, 0)) {
        nob_log(ERROR, "Impossible de desactiver la VSync, Voir: %s", SDL_GetError());
        res = false;
    }

    return res;
}

void clearContextSurface(sdl_ctx_t *sdl_ctx)
{
    SDL_DestroyTexture(sdl_ctx->bgTexture);
    sdl_ctx->bgTexture = NULL;
}

void renduImage(sdl_ctx_t *sdl_ctx, SDL_Texture *textureImg, SDL_FRect *rect)
{
    if (textureImg == NULL)
        return; // Au cas ou la texture ne correponde a rien

    SDL_RenderTexture(sdl_ctx->renderer, textureImg, NULL, rect);
}

SDL_Texture *chargerImage(sdl_ctx_t *sdl_ctx, char *chemin)
{
    //chargement de l'image dans une surface, si cela échoue on retourne directement NULL
    //format pris en charge BMP, PNG,

    char extension[10] = {0}; // Initialisation du tableau a zero
    sscanf(chemin, "%*[^.]%s", extension);
    nob_log(INFO, "Extension : %s", extension);

    SDL_Texture *textureImg = NULL;

    //Appel de la fonction adapté a l'extension de l'image
    if (strcmp(".bmp", extension) == 0){
        textureImg = SDL_CreateTextureFromSurface(sdl_ctx->renderer, SDL_LoadBMP(chemin));
    } else if (strcmp(".png", extension) == 0){
        textureImg = SDL_CreateTextureFromSurface(sdl_ctx->renderer, SDL_LoadPNG(chemin));
    }

    return textureImg;
}


void loadBackgroundImage(sdl_ctx_t *sdl_ctx, char *chemin)
{
    sdl_ctx->bgTexture = chargerImage(sdl_ctx, chemin);
}

void renderBackground(sdl_ctx_t *sdl_ctx)
{
    assert(sdl_ctx->bgTexture != NULL);
    renduImage(sdl_ctx, sdl_ctx->bgTexture, NULL);
}

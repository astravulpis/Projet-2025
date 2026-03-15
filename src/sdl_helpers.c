#include "sdl_helpers.h"
#include "common.h"
#include <stdlib.h>

SDL_FRect *createRect(float x, float y, float width, float height)
{
    SDL_FRect *res = NULL;
    if ((res = malloc(sizeof(SDL_FRect))) == NULL) {
        nob_log(ERROR, "%s:%d: failed to create a rect. Please buy more ram", __FILE__, __LINE__);
        return NULL;
    }

    if (width <= 0) width = 1;
    if (height <= 0) height = 1;

    res->x = x;
    res->y = y;
    res->w = width;
    res->h = height;

    return res;
}

void renderImage(sdl_ctx_t *sdl_ctx, SDL_Texture *textureImg, SDL_FRect *rect)
{
    assert(textureImg != NULL);
    SDL_RenderTexture(sdl_ctx->renderer, textureImg, NULL, rect);
}

bool renderText(sdl_ctx_t *sdl_ctx, char *text, TTF_Font *font, SDL_Color fontColor, SDL_FRect *renderingRect)
{
    SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, 0, fontColor, 0);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(sdl_ctx->renderer, textSurface);

    if (textTexture == NULL) {
        nob_log(ERROR, "%s:%d: Failed to create texture from the text surface", __FILE__, __LINE__);
        return false;
    }

    SDL_RenderTexture(sdl_ctx->renderer, textTexture, NULL, renderingRect);

    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);
    return true;
}

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

SDL_FRect *createRect_Ex(SDL_FRect rect)
{
    SDL_FRect *res = NULL;
    if ((res = malloc(sizeof(SDL_FRect))) == NULL) {
        nob_log(ERROR, "%s:%d: failed to create a rect. Please buy more ram", __FILE__, __LINE__);
        return NULL;
    }

    if (rect.w <= 0) {
        res->w = 1;
    } else {
        res->w = rect.w;
    }

    if (rect.h <= 0) {
        res->h = 1;
    } else {
        res->h = rect.h;
    }

    res->x = rect.x;
    res->y = rect.y;

    return res;
}
void renderImage(sdl_ctx_t *sdl_ctx, SDL_Texture *textureImg, SDL_FRect *rect)
{
    assert(textureImg != NULL);
    SDL_RenderTexture(sdl_ctx->renderer, textureImg, NULL, rect);
}

bool renderText(sdl_ctx_t *sdl_ctx, const char *text, SDL_Color color, float x_pos, float y_pos)
{
    return renderText_Ex(sdl_ctx, text, color, (V2f){x_pos, y_pos});
}

bool renderText_Ex(sdl_ctx_t *sdl_ctx, const char *text, SDL_Color color, V2f position)
{
    bool result = true;
    SDL_Surface *temp_surf = TTF_RenderText_Blended_Wrapped(sdl_ctx->font, text, 0, color, 0);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(sdl_ctx->renderer, temp_surf);

    if (tex == NULL) {
        nob_log(ERROR, "%s:%d: Failed to create texture from the text surface", __FILE__, __LINE__);
        return_defer(false);
    }

    if (!SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST)) {
        nob_log(ERROR, "%s:%d: Failed to set scale mode for text texture", __FILE__, __LINE__);
        return_defer(false);
    }

    SDL_RenderTexture(sdl_ctx->renderer, tex, NULL, &(SDL_FRect){position.x, position.y, temp_surf->w, temp_surf->h});

defer:
    SDL_DestroySurface(temp_surf);
    temp_surf = NULL;
    SDL_DestroyTexture(tex);
    tex = NULL;
    return result;
}

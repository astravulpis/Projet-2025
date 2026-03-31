/**
 * @file sdl_helpers.c
 * @brief Implementation of the different SDL helper functions
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-03-03
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#include "sdl_helpers.h"
#include "common.h"
#include <stdlib.h>
#include "bullets.h"


SDL_FRect *createRect(float x, float y, float width, float height)
{
    return createRect_Ex((SDL_FRect){x, y, width, height});
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
    if (textureImg != NULL) {
        SDL_RenderTexture(sdl_ctx->renderer, textureImg, NULL, rect);
    }
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

void renderFillRect(SDL_Renderer *renderer, SDL_FRect *rect, SDL_Color color)
{
    SDL_Color prev = {0};
    SDL_GetRenderDrawColor(renderer, &prev.r, &prev.g, &prev.b, &prev.a);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, rect);
    SDL_SetRenderDrawColor(renderer, prev.r, prev.g, prev.b, prev.a);
}

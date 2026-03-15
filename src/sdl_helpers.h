#ifndef SDL_HELPER_H_
#define SDL_HELPER_H_

#include "common.h"

/**
 * @fn renduImage(sdl_ctx_t *sdl_ctx, SDL_Texture *textureImg, float x, float y, float width, float height)
 * @brief Fait le rendu d'une SDL_Texture au coordonnées x, y.
 */
void renderImage(sdl_ctx_t *sdl_ctx, SDL_Texture *textureImg, SDL_FRect *rect);

/**
 * @fn createRect(float x, float y, float width, float height)
 * @return rect pointer sur un SDL_Frect allouer en memoire
 * @brief Alloue de la memoire pour un SDL_FRect en associant sa position et sa taille
 */
SDL_FRect *createRect(float x, float y, float width, float height);

bool renderText(sdl_ctx_t *sdl_ctx, char *text, TTF_Font *font, SDL_Color fontColor, SDL_FRect *renderingRect);

#endif // SDL_HELPER_H_

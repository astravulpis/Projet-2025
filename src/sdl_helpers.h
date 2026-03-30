/**
 * @file sdl_helpers.h
 * @brief This file declares all of the helper functions related to SDL, but written by our hands
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-03-15
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#ifndef SDL_HELPER_H_
#define SDL_HELPER_H_

#include "common.h"

/**
 * @fn renderImage(sdl_ctx_t *sdl_ctx, SDL_Texture *textureImg, SDL_FRect *rect)
 * @param[in] sdl_ctx Our sdl context
 * @param[in] textureImg The texture that gets rendered
 * @param[in] rect The rectangle that houses the texture
 * @brief Renders a texture on a rect
 */
void renderImage(sdl_ctx_t *sdl_ctx, SDL_Texture *textureImg, SDL_FRect *rect);

/**
 * @fn createRect(float x, float y, float width, float height)
 * @param[in] x Position x
 * @param[in] y Position y
 * @param[in] width Width of the rect
 * @param[in] height Height of the rect
 * @return rect A pointer to a SDL_FRect allocated on the heap
 * @brief allocate memory for a SDL_FRect of size width*height at position {x, y}
 */
SDL_FRect *createRect(float x, float y, float width, float height);

/**
 * @fn createRect_Ex(SDL_FRect rect)
 * @param[in] rect A temporary/example rect to copy over into heap
 * @return rect A pointer to a SDL_FRect allocated on the heap
 * @brief allocate memory for a SDL_FRect
 */
SDL_FRect *createRect_Ex(SDL_FRect rect);

/**
 * @fn renderText(sdl_ctx_t *sdl_ctx, const char *text, SDL_Color color, float x_pos, float y_pos)
 * @brief Render text on the screen
 * @param[in] sdl_ctx Our sdl context
 * @param[in] text The text to be rendered
 * @param[in] color The color of the text rendered
 * @param[in] x_pos The x-offset from the left of the window
 * @param[in] y_pos The y-offset from the top of the window
 */
bool renderText(sdl_ctx_t *sdl_ctx, const char *text, SDL_Color color, float x_pos, float y_pos);

/**
 * @fn renderText_Ex(sdl_ctx_t *sdl_ctx, const char *text, SDL_Color color, V2f position);
 * @brief Render text on the screen
 * @param[in] sdl_ctx Our sdl context
 * @param[in] text The text to be rendered
 * @param[in] color The color of the text rendered
 * @param[in] position The position of where the text should be at
 */
bool renderText_Ex(sdl_ctx_t *sdl_ctx, const char *text, SDL_Color color, V2f position);

/**
 * @fn renderFillRect(SDL_Renderer *renderer, SDL_FRect *rect, SDL_Color color)
 * @brief render a rectangle with the color specified
 */
void renderFillRect(SDL_Renderer *renderer, SDL_FRect *rect, SDL_Color color);

/**
 * @fn keep_player_inbound(SDL_FRect *r, float minX, float minY, float maxX, float maxY)
 * @param[in] r The SDL rect given to check
 * @param[in] minX Minimum X bound
 * @param[in] minY Minimum Y bound
 * @param[in] maxX Maximum X bound
 * @param[in] maxY Maximum Y bound
 * @brief keep rectangle in the given bounds
 *
 * Takes the location of the rect we are moving then compares it to the bounds given.
 */
void keepRectInbounds(SDL_FRect *r, float minX, float minY, float maxX, float maxY);

#endif // SDL_HELPER_H_

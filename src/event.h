#ifndef EVENT_H_
#define EVENT_H_

#include "common.h"

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
void keepPlayerInbound(SDL_FRect *r, float minX, float minY, float maxX, float maxY);

/**
 * @fn basic_movements(struct sdl_context_s *ctx)
 * @brief Listens to keyboard inputs (specifically A and D)
 * @param[in] ctx SDL context
 * @param[out] direction Returns the character pressed
 */
void basicMovementEvents(struct sdl_context_s *ctx, float *dx, float *dy);

/**
 * @fn basic_keyboard_events(struct sdl_context_s *ctx)
 * @brief Listens to keyboard inputs for mapped keys
 * @param[in] ctx SDL context
 */
void basicKeyboardEvents(sdl_ctx_t *sdl_ctx);

// void resolveOverlap(SDL_FRect *a, SDL_FRect *b);

#endif // EVENT_H_

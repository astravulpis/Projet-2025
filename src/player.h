#ifndef PLAYER_H_
#define PLAYER_H_

#include "common.h"

void UpdatePlayer(sdl_ctx_t *sdl_ctx, SDL_FRect *player, SDL_FRect *object, float deltaTime);

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

#endif // PLAYER_H_


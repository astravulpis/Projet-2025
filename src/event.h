/**
 * @file event.h
 * @brief Here lies event handlers
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-03-03
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 **/

#ifndef EVENT_H_
#define EVENT_H_

#include "common.h"
#include "level.h"

/**
 * @fn basic_keyboard_events(struct sdl_context_s *ctx)
 * @brief Listens to keyboard inputs for mapped keys
 * @param[in] ctx SDL context
 */
void basicKeyboardEvents(sdl_ctx_t *sdl_ctx, level_t *level, player_t *player);

// void resolveOverlap(SDL_FRect *a, SDL_FRect *b);

#endif // EVENT_H_

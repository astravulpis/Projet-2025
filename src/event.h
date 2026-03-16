#ifndef EVENT_H_
#define EVENT_H_

#include "common.h"

/**
 * @fn basic_keyboard_events(struct sdl_context_s *ctx)
 * @brief Listens to keyboard inputs for mapped keys
 * @param[in] ctx SDL context
 */
void basicKeyboardEvents(sdl_ctx_t *sdl_ctx);

// void resolveOverlap(SDL_FRect *a, SDL_FRect *b);

#endif // EVENT_H_

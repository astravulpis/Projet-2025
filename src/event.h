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
#include "guns.h"
#include "level.h"
#include "player.h"
#include "sdl_helpers.h"

/**
 * @fn basic_keyboard_events(struct sdl_context_s *ctx)
 * @param[in] sdl_ctx SDL context
 * @param[in] level level structure
 * @param[in] player player structure
 * @param[in] guns list of guns to be able to equip them
 * @brief Listens to keyboard inputs for mapped keys
 */
void basicKeyboardEvents(sdl_ctx_t *sdl_ctx, level_t *level, player_t *player, Guns_t *guns);

#endif // EVENT_H_

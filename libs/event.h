/**
 * @file event.h
 * @brief Here lies event handlers
 *
 * @author Liam B. <liam.berge72@gmail.com>
 * @date 2026-03-03
 * @remark last modified: 2026-04-09
 *
 * Contributors:
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
 * @brief Listens to keyboard inputs for mapped keys to execute the corresponding action
 *
 * gets the keyboard input with \ref SDL_GetKeyboardState, then goes through the mapped inputs.
 * Inputs being the escape key for the menu which also pauses the music with \ref MIX_PauseAllTracks,
 * however resumes the music if the game was already paused with \ref MIX_ResumeAllTracks.
 * It also changes the current menu based on what is clicked by updating the \ref ctx->currMenu.
 * Finally it checks for the number row keys to change the currently equipped gun by calling \ref setCurrentGun.
 */
void basicKeyboardEvents(sdl_ctx_t *sdl_ctx, Guns_t *guns);
// void basicKeyboardEvents(sdl_ctx_t *sdl_ctx, level_t *level, player_t *player, Guns_t *guns);

#endif // EVENT_H_

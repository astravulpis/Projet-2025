/**
 * @file gui.h
 * @brief Declarations for the Graphical User Interface (GUI)
 *
 * Author: Rossignol François <francois_rossignol@outlook.fr>
 * Last Modified: 2026-03-26
 * Date: 2026-03-25
 *
 * * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#ifndef GUI_H_
#define GUI_H_

#include "common.h"
#include "buttons.h"

void pauseMenu (sdl_ctx_t *sdl_ctx, button **resume, button **options, button **quit);
void destroyPauseMenu (button **resume, button **options, button **quit);
void update_and_renderPauseMenu(sdl_ctx_t *sdl_ctx, SDL_FPoint *mouseCoord, int mouseInputFlag, button *resume, button *options, button *quit);

#endif // GUI_H_

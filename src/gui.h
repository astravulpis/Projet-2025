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
 * Liam B. <liam.berge72@gmail.com>
 **/

#ifndef GUI_H_
#define GUI_H_

#include "buttons.h"
#include "common.h"

typedef struct {
    button **items;
    size_t count;
    size_t capacity;

    // Attributs specific to the GUI menu
    SDL_Color bgColor;
} gui_menu;

gui_menu *createMenu(SDL_Color bgColor, size_t count, ...);
void updateMenu(sdl_ctx_t *sdl_ctx, V2f mouseCoord, int mouseInputFlag, gui_menu *menu, void (*updateFunc)(sdl_ctx_t *, gui_menu *));
void renderMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu);
void destroyMenu(gui_menu **menu);

// Pause menu related
gui_menu *createPauseMenu(sdl_ctx_t *sdl_ctx);
void updatePauseMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu);

#endif // GUI_H_

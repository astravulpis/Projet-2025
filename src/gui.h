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
#include "sliders.h"

typedef struct {
    button **items;
    size_t count;
    size_t capacity;
} buttons;

typedef struct {
    slider **items;
    size_t count;
    size_t capacity;
} sliders;

typedef struct {
    buttons btns;
    sliders sliders;

    // Attributs specific to the GUI menu
    SDL_Color bgColor;
} gui_menu;

typedef void (*helper_function)(sdl_ctx_t *, gui_menu *);

gui_menu *createMenu(SDL_Color bgColor);
void updateMenu(sdl_ctx_t *sdl_ctx, V2f mouseCoord, int mouseInputFlag, gui_menu *menu, helper_function updateFunc);
void renderMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu);
void destroyMenu(gui_menu **menu);
void addButtonToMenu(gui_menu *menu, button *btn);
void addSliderToMenu(gui_menu *menu, slider *slider);

// Pause menu related
gui_menu *createPauseMenu(sdl_ctx_t *sdl_ctx);
void updatePauseMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu);

// Options menu related
gui_menu *createOptionsMenu(sdl_ctx_t *sdl_ctx);
void updateOptionsMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu);

// Home menu related
gui_menu *createHomeMenu(sdl_ctx_t *sdl_ctx);
void updateHomeMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu);

// Level selection related
gui_menu *createLevelMenu(sdl_ctx_t *sdl_ctx);
void updateLevelMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu);

#endif // GUI_H_

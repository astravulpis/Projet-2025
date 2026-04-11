/**
 * @file gui.h
 * @brief Declarations for the Graphical User Interface (GUI)
 *
 * @author Rossignol François <francois_rossignol@outlook.fr>
 * @date 2026-03-25
 * @remark last modified: 2026-04-9
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

typedef struct {
    size_t *loadedLevelIdx;
    // other data pointers
    // here ...
} helperFuncOpts;

typedef void (*helperFunc)(sdl_ctx_t *, gui_menu *, helperFuncOpts opts);

// Pause menu related
gui_menu *createPauseMenu(sdl_ctx_t *sdl_ctx);
void updatePauseMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, helperFuncOpts opts);

// Options menu related
gui_menu *createOptionsMenu(sdl_ctx_t *sdl_ctx);
void updateOptionsMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, helperFuncOpts opts);

// Home menu related
gui_menu *createHomeMenu(sdl_ctx_t *sdl_ctx);
void updateHomeMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, helperFuncOpts opts);

// Level selection related
gui_menu *createLevelMenu(sdl_ctx_t *sdl_ctx);
void updateLevelMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, helperFuncOpts opts);

gui_menu *createMenu(SDL_Color bgColor);
void __updateMenu(sdl_ctx_t *sdl_ctx, V2f mouseCoord, int mouseInputFlag, gui_menu *menu, helperFunc updateFunc,
                  helperFuncOpts opts);
#define updateMenu(sdl_ctx, mouseCoord, mouseInputFlag, menu, updateFunc, ...)                                   \
    __updateMenu((sdl_ctx), (mouseCoord), (mouseInputFlag), (menu), (updateFunc), (helperFuncOpts){__VA_ARGS__})
void renderMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu);
void destroyMenu(gui_menu **menu);
void addButtonToMenu(gui_menu *menu, button *btn);
void addSliderToMenu(gui_menu *menu, slider *slider);

#endif // GUI_H_

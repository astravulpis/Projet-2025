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

/**
 * @struct buttons
 * @brief a \ref button array
 *
 * a \ref button array with len attribute \ref count and limit \ref capacity
 */
typedef struct {
    button **items; //!< the button array
    size_t count; //!< the button counter
    size_t capacity; //!< the array maximum length
} buttons;

/**
 * @struct sliders
 * @brief a \ref slider array
 *
 * a \ref slider array with len attribute \ref count and limit \ref capacity
 */
typedef struct {
    slider **items; //!< the slider array
    size_t count; //!< the slider counter
    size_t capacity; //!< the array maximum length
} sliders;

/**
 * @struct gui_menu
 * @brief a struct that contain \ref buttons array and \ref sliders array with bgColor attribute.
 */
typedef struct {
    buttons btns; //!< \ref buttons array
    sliders sliders; //!< \ref sliders array

    // attribute specific to the GUI menu
    SDL_Color bgColor;
} gui_menu;

/**
 * @struct helperFuncOpts
 * @brief a struct that contain data given for \ref helperFunc
 */
typedef struct {
    size_t *loadedLevelIdx;
    bool *isBGMPlaying;
    // other data pointers
    // here ...
} helperFuncOpts;

typedef void (*helperFunc)(sdl_ctx_t *, gui_menu *, helperFuncOpts opts);

// Pause menu related
/**
 * @fn createPauseMenu(sdl_ctx_t *sdl_ctx)
 * @param sdl_ctx is used to initialize the images for \ref button 's in the \ref buttons of the \ref gui_menu
 * @brief initialize all the \ref button of the \ref gui_menu pauseMenu
 * @return returns a pointer to gui_menu or 3 \ref button has been added to the \ref buttons attribute
 */
gui_menu *createPauseMenu(sdl_ctx_t *sdl_ctx);

/**
 * @fn updatePauseMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, helperFuncOpts opts)
 * @param sdl_ctx is used to control \ref currMenu (see \ref menu_kind).
 * @param menu the menu that we to update; here the pause menu.
 * @param helperFuncOpts addtional data, not used in this menu
 * @brief update all the \ref button of the \ref gui_menu \ref menu
 */
void updatePauseMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, helperFuncOpts opts);

// Options menu related*
/**
 * @fn createOptionsMenu(sdl_ctx_t *sdl_ctx);
 * @param sdl_ctx is used to initialize the images for \ref button 's in the \ref buttons of the \ref gui_menu and have acces to \ref screenRatio
 * @brief initialize all the \ref button of the \ref gui_menu optionMenu
 * @return returns a pointer to gui_menu of 1 \ref button has been added to the \ref buttons attribute, and 3 \ref slider in \ref sliders
 */
gui_menu *createOptionsMenu(sdl_ctx_t *sdl_ctx);

/**
 * @fn updateOptionsMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, helperFuncOpts opts)
 * @param sdl_ctx is used to control \ref currMenu (see \ref menu_kind).
 * @param menu the menu that we to update; here the option menu.
 * @param helperFuncOpts addtional data, not used in this menu
 * @brief update all the \ref button and \ref slider of the \ref gui_menu \ref menu
 */
void updateOptionsMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, helperFuncOpts opts);

// Home menu related
/**
 * @fn createHomeMenu(sdl_ctx_t *sdl_ctx)
 * @param sdl_ctx is used to initialize the images for \ref button 's in the \ref buttons of the \ref gui_menu
 * @brief initialize all the \ref button of the \ref gui_menu home
 * @return returns a pointer to gui_menu of 4 \ref button has been added to the \ref buttons attribute
 */
gui_menu *createHomeMenu(sdl_ctx_t *sdl_ctx);

/**
 * @fn updateHomeMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, helperFuncOpts opts)
 * @param sdl_ctx is used to control \ref currMenu (see \ref menu_kind).
 * @param menu the menu that we to update; here the home menu.
 * @param helperFuncOpts addtional data, not used in this menu
 * @brief update all the \ref button of the \ref gui_menu \ref menu
 */
void updateHomeMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, helperFuncOpts opts);

// Level selection related
/**
 * @fn crteaeLevelMenu(sdl_ctx_t *sdl_ctx)
 * @param sdl_ctx is used to initialize the images for \ref button 's in the \ref buttons of the \ref gui_menu
 * @brief initialize all the \ref button of the \ref gui_menu levels selection menu
 * @return returns a pointer to gui_menu of 6 (5 levels, 1 back to previous menu) \ref button has been added to the \ref buttons attribute
 */
gui_menu *createLevelMenu(sdl_ctx_t *sdl_ctx);

/**
 * @fn updateLevelMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, helperFuncOpts opts)
 * @param sdl_ctx is used to control \ref currMenu (see \ref menu_kind).
 * @param menu the menu that we to update; here the level selection menu.
 * @param helperFuncOpts addtional data, here \ref loadedLevelIdx is changed in function of wich button is pressed, and a new level will be loaded and printed
 * @brief update all the \ref button of the \ref gui_menu \ref menu
 */
void updateLevelMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, helperFuncOpts opts);

gui_menu *createMenu(SDL_Color bgColor);
void __updateMenu(sdl_ctx_t *sdl_ctx, mouseDevice *mouse, gui_menu *menu, helperFunc updateFunc, helperFuncOpts opts);
#define updateMenu(sdl_ctx, mouse, menu, updateFunc, ...)                                   \
    __updateMenu((sdl_ctx), (mouse), (menu), (updateFunc), (helperFuncOpts){__VA_ARGS__})
void renderMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu);
void destroyMenu(gui_menu **menu);
void addButtonToMenu(gui_menu *menu, button *btn);
void addSliderToMenu(gui_menu *menu, slider *slider);

#endif // GUI_H_

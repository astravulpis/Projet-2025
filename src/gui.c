/**
 * @file gui.c
 * @brief Implementation of the GUI
 *
 * Author: Rossignol François <francois_rossignol@outlook.fr>
 * Last Modified: 2026-03-26
 * Date: 2026-03-25
 *
 * * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 * Liam B. <liam.berge72@gmail.com>
 **/

#include "gui.h"
#include "buttons.h"
#include "common.h"
#include "sdl_helpers.h"
#include <stdarg.h>
#include "music.h"

gui_menu *createMenu(SDL_Color bgColor, size_t count, ...)
{
    gui_menu *menu = calloc(1, sizeof(gui_menu));
    if (menu == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for a `gui_menu`", __FILE__, __LINE__);
        return NULL;
    }

    va_list ap;
    va_start(ap, count);
    for (size_t i = 0; i < count; ++i) {
        da_append(menu, (va_arg(ap, void *)));
    }
    va_end(ap);

    menu->bgColor = bgColor;

    return menu;
}

gui_menu *createPauseMenu(sdl_ctx_t *sdl_ctx)
{
    SDL_FRect boxResume = (SDL_FRect){(WINDOW_WIDTH / 2.0f - 150.0f) * sdl_ctx->screenRatio,
                                      (WINDOW_HEIGHT / 2.0f - 37.5f - 90.0f) * sdl_ctx->screenRatio,
                                      300.0f * sdl_ctx->screenRatio, 75.0f * sdl_ctx->screenRatio};

    SDL_FRect boxOptions = (SDL_FRect){(WINDOW_WIDTH / 2.0f - 150.0f) * sdl_ctx->screenRatio,
                                       (WINDOW_HEIGHT / 2.0f - 37.5f) * sdl_ctx->screenRatio, 300.0f * sdl_ctx->screenRatio,
                                       75.0f * sdl_ctx->screenRatio};

    SDL_FRect boxQuit = (SDL_FRect){(WINDOW_WIDTH / 2.0f - 150.0f) * sdl_ctx->screenRatio,
                                    (WINDOW_HEIGHT / 2.0f - 37.5f + 90.0f) * sdl_ctx->screenRatio,
                                    300.0f * sdl_ctx->screenRatio, 75.0f * sdl_ctx->screenRatio};

    button *resumeButton = NULL;
    createButton(sdl_ctx, &resumeButton, "resume", boxResume, "./assets/img/buttons/BTest.png", "./assets/img/buttons/HTest.png", "./assets/img/buttons/CTest.png");

    button *optionsButton = NULL;
    createButton(sdl_ctx, &optionsButton, "options", boxOptions, "./assets/img/buttons/BTest.png", "./assets/img/buttons/HTest.png", "./assets/img/buttons/CTest.png");

    button *quitButton = NULL;
    createButton(sdl_ctx, &quitButton, "quit", boxQuit, "./assets/img/buttons/BTest.png", "./assets/img/buttons/HTest.png", "./assets/img/buttons/CTest.png");

    gui_menu *menu = createMenu((SDL_Color){60, 60, 60, 120}, 3, resumeButton, optionsButton, quitButton);

    return menu;
}

void updatePauseMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu)
{

    if (menu->items[0]->isLeftClicked == true){
        sdl_ctx->paused = false;
        MIX_ResumeTrack(sdl_ctx->track);
    } 
    if (menu->items[2]->isLeftClicked == true) sdl_ctx->quit = true;
}

void updateMenu(sdl_ctx_t *sdl_ctx, V2f mouseCoord, int mouseInputFlag, gui_menu *menu,
                void (*updateFunc)(sdl_ctx_t *, gui_menu *))
{
    da_foreach (button *, button, menu) {
        updateButtonState(*button, mouseCoord, mouseInputFlag);
    }
    updateFunc(sdl_ctx, menu);
}

void renderMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu)
{
    // Fill the background with a specific color
    renderFillRect(sdl_ctx->renderer, &(SDL_FRect){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, menu->bgColor);

    // Render each button of the menu
    da_foreach (button *, button, menu) {
        buttonRender(sdl_ctx, *button);
    }
}

void destroyMenu(gui_menu **menu)
{
    da_foreach (button *, button, (*menu)) {
        destroyButton(button);
    }
    free((*menu)->items);
    free(*menu);
    *menu = NULL;
}
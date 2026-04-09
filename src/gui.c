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
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "sliders.h"
#include "level.h"

gui_menu *createMenu(SDL_Color bgColor)
{
    gui_menu *menu = calloc(1, sizeof(gui_menu));
    if (menu == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for a `gui_menu`", __FILE__, __LINE__);
        return NULL;
    }

    memset(&menu->btns, 0, sizeof(buttons));
    memset(&menu->sliders, 0, sizeof(sliders));

    menu->bgColor = bgColor;

    return menu;
}

void addButtonToMenu(gui_menu *menu, button *btn)
{
    da_append(&menu->btns, btn);
}

void addSliderToMenu(gui_menu *menu, slider *slider)
{
    da_append(&menu->sliders, slider);
}

// ******************* PAUSE MENU ***********************
gui_menu *createPauseMenu(sdl_ctx_t *sdl_ctx)
{
    SDL_FRect boxResume = (SDL_FRect){(WINDOW_WIDTH / 2.0f - 192.0f), (WINDOW_HEIGHT / 2.0f - 48.0f - 120.0f), 384.0f, 96.0f};
    boxToScale(&boxResume, sdl_ctx->screenRatio);

    SDL_FRect boxOptions = (SDL_FRect){(WINDOW_WIDTH / 2.0f - 192.0f), (WINDOW_HEIGHT / 2.0f - 48.0f), 384.0f, 96.0f};
    boxToScale(&boxOptions, sdl_ctx->screenRatio);

    SDL_FRect boxQuit = (SDL_FRect){(WINDOW_WIDTH / 2.0f - 192.0f), (WINDOW_HEIGHT / 2.0f - 48.0f + 120.0f), 384.0f, 96.0f};
    boxToScale(&boxQuit, sdl_ctx->screenRatio);

    button *resumeButton = NULL;
    createButton(sdl_ctx, &resumeButton, "RESUME", boxResume, "./assets/img/buttons/base128.png",
                 "./assets/img/buttons/hover128.png", "./assets/img/buttons/click128.png");

    button *optionsButton = NULL;
    createButton(sdl_ctx, &optionsButton, "OPTIONS", boxOptions, "./assets/img/buttons/base128.png",
                 "./assets/img/buttons/hover128.png", "./assets/img/buttons/click128.png");

    button *quitButton = NULL;
    createButton(sdl_ctx, &quitButton, "QUIT", boxQuit, "./assets/img/buttons/base128.png", "./assets/img/buttons/hover128.png",
                 "./assets/img/buttons/click128.png");

    gui_menu *menu = createMenu((SDL_Color){60, 60, 60, 120});
    addButtonToMenu(menu, resumeButton);
    addButtonToMenu(menu, optionsButton);
    addButtonToMenu(menu, quitButton);

    return menu;
}

void updatePauseMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, ...)
{

    // Resume button
    if (menu->btns.items[0]->isLeftClicked == true) {
        sdl_ctx->currMenu = NONE_MENU;
        MIX_ResumeAllTracks(sdl_ctx->mixer);
    }
    // Options button
    if (menu->btns.items[1]->isLeftClicked == true) {
        sdl_ctx->currMenu = OPTIONS_MENU;
    }

    // Quit button
    if (menu->btns.items[2]->isLeftClicked == true) sdl_ctx->quit = true;
}

// ******************* OPTIONS MENU ***********************
gui_menu *createOptionsMenu(sdl_ctx_t *sdl_ctx)
{
    SDL_FRect goBackBox = (SDL_FRect){100.f, WINDOW_HEIGHT - 350.f, 384.f, 96.f};
    boxToScale(&goBackBox, sdl_ctx->screenRatio);

    SDL_FRect masterBox = (SDL_FRect){300, 200, 512, 64};
    boxToScale(&masterBox, sdl_ctx->screenRatio);

    SDL_FRect musicBox = (SDL_FRect){300, 278, 512, 64};
    boxToScale(&musicBox, sdl_ctx->screenRatio);

    SDL_FRect sfxBox = (SDL_FRect){300, 352, 512, 64};
    boxToScale(&sfxBox, sdl_ctx->screenRatio);

    button *goBackButton = NULL;
    createButton(sdl_ctx, &goBackButton, "RESUME", goBackBox, "./assets/img/buttons/base128.png",
                 "./assets/img/buttons/hover128.png", "./assets/img/buttons/click128.png");

    slider *masterVolume = NULL;
    createSlider(&masterVolume, masterBox, 100, 10.0f * sdl_ctx->screenRatio, sdl_ctx->opts.masterVolume, "Master Volume");

    slider *musicVolume = NULL;
    createSlider(&musicVolume, musicBox, 100, 10.0f * sdl_ctx->screenRatio, sdl_ctx->opts.musicVolume, "Music Volume");

    slider *sfxVolume = NULL;
    createSlider(&sfxVolume, sfxBox, 100, 10.0f * sdl_ctx->screenRatio, sdl_ctx->opts.sfxVolume, "Sound Effect Volume");

    gui_menu *menu = createMenu((SDL_Color){60, 60, 60, 120});
    addButtonToMenu(menu, goBackButton);
    addSliderToMenu(menu, masterVolume);
    addSliderToMenu(menu, musicVolume);
    addSliderToMenu(menu, sfxVolume);

    return menu;
}

void updateOptionsMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, ...)
{
    static float sfx = 0;

    // Go back button
    if (menu->btns.items[0]->isLeftClicked == true) {
        sdl_ctx->currMenu = PAUSE_MENU;
    }

    sdl_ctx->opts.masterVolume = menu->sliders.items[0]->currentValue;
    setMasterTrackGain(sdl_ctx);

    sdl_ctx->opts.musicVolume = menu->sliders.items[1]->currentValue;
    setMusicTrackGain(sdl_ctx);

    // Sfx
    sdl_ctx->opts.sfxVolume = menu->sliders.items[2]->currentValue;
    if (sdl_ctx->opts.sfxVolume - sfx != 0) // Avoids doing a for-loop on the different tracks when not needed
        setSfxTrackGain(sdl_ctx);
    sfx = sdl_ctx->opts.sfxVolume;
}

// ******************* HOME MENU ***********************
gui_menu *createHomeMenu(sdl_ctx_t *sdl_ctx)
{
    SDL_FRect boxPlay = (SDL_FRect){(WINDOW_WIDTH / 2.0f - 192.0f), (WINDOW_HEIGHT / 2.0f - 48.0f - 120.0f), 384.0f, 96.0f};
    boxToScale(&boxPlay, sdl_ctx->screenRatio);

    SDL_FRect boxLevelSelect = (SDL_FRect){(WINDOW_WIDTH / 2.0f - 192.0f), (WINDOW_HEIGHT / 2.0f - 48.0f), 384.0f, 96.0f};
    boxToScale(&boxLevelSelect, sdl_ctx->screenRatio);

    SDL_FRect boxQuit = (SDL_FRect){(WINDOW_WIDTH / 2.0f - 192.0f), (WINDOW_HEIGHT / 2.0f - 48.0f + 120.0f), 384.0f, 96.0f};
    boxToScale(&boxQuit, sdl_ctx->screenRatio);

    button *playButton = NULL;
    createButton(sdl_ctx, &playButton, "PLAY", boxPlay, "./assets/img/buttons/base128.png",
                 "./assets/img/buttons/hover128.png", "./assets/img/buttons/click128.png");

    button *levelButton = NULL;
    createButton(sdl_ctx, &levelButton, "LEVEL SELECTION", boxLevelSelect, "./assets/img/buttons/base128.png",
                 "./assets/img/buttons/hover128.png", "./assets/img/buttons/click128.png");

    button *quitButton = NULL;
    createButton(sdl_ctx, &quitButton, "QUIT", boxQuit, "./assets/img/buttons/base128.png", "./assets/img/buttons/hover128.png",
                 "./assets/img/buttons/click128.png");

    gui_menu *menu = createMenu((SDL_Color){60, 60, 60, 120});
    addButtonToMenu(menu, playButton);
    addButtonToMenu(menu, levelButton);
    addButtonToMenu(menu, quitButton);

    return menu;
}

void updateHomeMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, ...)
{

    // Play button
    if (menu->btns.items[0]->isLeftClicked == true) {
        sdl_ctx->currMenu = NONE_MENU;
        MIX_ResumeAllTracks(sdl_ctx->mixer);
    }
    // Level Selection button
    if (menu->btns.items[1]->isLeftClicked == true) {
        sdl_ctx->currMenu = LEVEL_SELECTION_MENU;
    }

    // Quit button
    if (menu->btns.items[2]->isLeftClicked == true) sdl_ctx->quit = true;
}

// ******************* LEVEL SELECTION MENU ***********************
gui_menu *createLevelMenu(sdl_ctx_t *sdl_ctx)
{
    SDL_FRect boxLevelA = (SDL_FRect){92, 256, 256.0f, 64.0f};
    boxToScale(&boxLevelA, sdl_ctx->screenRatio); // pas utilisé pour les autres boutons de niveau car ils vont être crée a partir de cette box (ratio déja fait)

    SDL_FRect boxLevelB = (SDL_FRect){boxLevelA.x, boxLevelA.y + boxLevelA.h + (15 * sdl_ctx->screenRatio), boxLevelA.w, boxLevelA.h};

    SDL_FRect boxLevelC = (SDL_FRect){boxLevelA.x, boxLevelB.y + boxLevelA.h + (15 * sdl_ctx->screenRatio), boxLevelA.w, boxLevelA.h};

    SDL_FRect boxLevelD = (SDL_FRect){boxLevelA.x, boxLevelC.y + boxLevelA.h + (15 * sdl_ctx->screenRatio), boxLevelA.w, boxLevelA.h};

    SDL_FRect boxLevelE = (SDL_FRect){boxLevelA.x, boxLevelD.y + boxLevelA.h + (15 * sdl_ctx->screenRatio), boxLevelA.w, boxLevelA.h};

    SDL_FRect boxBackToHomeMenu = (SDL_FRect){30, 64, 384.0f, 96.0f};
    boxToScale(&boxBackToHomeMenu, sdl_ctx->screenRatio);

    button *levelAButton = NULL;
    createButton(sdl_ctx, &levelAButton, "A", boxLevelA, "./assets/img/buttons/base128.png",
                 "./assets/img/buttons/hover128.png", "./assets/img/buttons/click128.png");

    button *levelBButton = NULL;
    createButton(sdl_ctx, &levelBButton, "B", boxLevelB, "./assets/img/buttons/base128.png",
                 "./assets/img/buttons/hover128.png", "./assets/img/buttons/click128.png");

    button *levelCButton = NULL;
    createButton(sdl_ctx, &levelCButton, "C", boxLevelC, "./assets/img/buttons/base128.png",
                 "./assets/img/buttons/hover128.png", "./assets/img/buttons/click128.png");

    button *levelDButton = NULL;
    createButton(sdl_ctx, &levelDButton, "D", boxLevelD, "./assets/img/buttons/base128.png",
                 "./assets/img/buttons/hover128.png", "./assets/img/buttons/click128.png");

    button *levelEButton = NULL;
    createButton(sdl_ctx, &levelEButton, "E", boxLevelE, "./assets/img/buttons/base128.png",
                 "./assets/img/buttons/hover128.png", "./assets/img/buttons/click128.png");

    button *backButton = NULL;
    createButton(sdl_ctx, &backButton, "BACK TO HOME MENU", boxBackToHomeMenu, "./assets/img/buttons/base128.png", "./assets/img/buttons/hover128.png",
                 "./assets/img/buttons/click128.png");

    gui_menu *menu = createMenu((SDL_Color){60, 60, 60, 120});
    addButtonToMenu(menu, levelAButton);
    addButtonToMenu(menu, levelBButton);
    addButtonToMenu(menu, levelCButton);
    addButtonToMenu(menu, levelDButton);
    addButtonToMenu(menu, levelEButton);

    addButtonToMenu(menu, backButton);

    return menu;
}

void updateLevelMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu, size_t *loadedLevelIdx)
{

    // Level button's
    // Level A
    if (menu->btns.items[0]->isLeftClicked == true) {
        sdl_ctx->currMenu = NONE_MENU;
        MIX_ResumeAllTracks(sdl_ctx->mixer);
        *loadedLevelIdx = 0;
    }
    // Level B
    if (menu->btns.items[1]->isLeftClicked == true) {
        sdl_ctx->currMenu = NONE_MENU;
        MIX_ResumeAllTracks(sdl_ctx->mixer);
        *loadedLevelIdx = 1;
    }
    // Level C
    if (menu->btns.items[2]->isLeftClicked == true) {
        sdl_ctx->currMenu = NONE_MENU;
        MIX_ResumeAllTracks(sdl_ctx->mixer);
        *loadedLevelIdx = 2;
    }
    // Level D
    if (menu->btns.items[3]->isLeftClicked == true) {
        sdl_ctx->currMenu = NONE_MENU;
        MIX_ResumeAllTracks(sdl_ctx->mixer);
        *loadedLevelIdx = 3;
    }
    // Level E
    if (menu->btns.items[4]->isLeftClicked == true) {
        sdl_ctx->currMenu = NONE_MENU;
        MIX_ResumeAllTracks(sdl_ctx->mixer);
        *loadedLevelIdx = 4;
    }

    // Back to Home menu button
    if (menu->btns.items[5]->isLeftClicked == true){
        sdl_ctx->currMenu = START_MENU;
    }
}

void updateMenu(sdl_ctx_t *sdl_ctx, V2f mouseCoord, int mouseInputFlag, gui_menu *menu, helper_function updateFunc, size_t *loadedLevelIdx)
{
    da_foreach (button *, button, &menu->btns) {
        updateButtonState(*button, mouseCoord, mouseInputFlag);
    }
    da_foreach (slider *, slider, &menu->sliders) {
        updateSliderStates(*slider, mouseCoord, mouseInputFlag, NULL);
    }

    updateFunc(sdl_ctx, menu, loadedLevelIdx);
}

void renderMenu(sdl_ctx_t *sdl_ctx, gui_menu *menu)
{
    if (sdl_ctx->currMenu != NONE_MENU) {
        // Fill the background with a specific color
        renderFillRect(sdl_ctx->renderer, &(SDL_FRect){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, menu->bgColor);

        // Render each button of the menu
        da_foreach (button *, button, &menu->btns) {
            renderButton(sdl_ctx, *button);
        }
        da_foreach (slider *, slider, &menu->sliders) {
            renderSlider(sdl_ctx, *slider);
        }
    }
}

void destroyButtons(buttons *btns)
{
    da_foreach (button *, button, btns) {
        destroyButton(button);
    }
    free(btns->items);
}

void destroySliders(sliders *sliders)
{
    da_foreach (slider *, slider, sliders) {
        destroySlider(slider);
    }
    free(sliders->items);
}

void destroyMenu(gui_menu **menu)
{
    if (*menu) {
        destroyButtons(&(*menu)->btns);
        destroySliders(&(*menu)->sliders);
    }
    free(*menu);
    *menu = NULL;
}

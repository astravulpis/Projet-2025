/**
 * @file button.h
 * @brief File to define what a button is and how it works
 *
 * Author: Rossignol François <francois_rossignol@outlook.fr>
 * Last Modified: 2026-03-26
 * Date: 2026-03-22
 *
 * * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "common.h"

/* Ancienne version du type boutons
typedef struct button {
    char *buttonText;
    SDL_FRect *buttonBox;
    SDL_Color baseColor;
    SDL_Color hoverColor;
    SDL_Color clickColor;
    bool isHovered;
    bool isLeftClicked;
    bool isRightClicked;
} button;
*/

typedef struct button {
    char *buttonText;
    SDL_FRect *buttonBox;
    SDL_Texture *baseImg;
    SDL_Texture *hoverImg;
    SDL_Texture *clickImg;
    bool isHovered;
    bool isLeftClicked;
    bool isRightClicked;
} button;

bool createButton(sdl_ctx_t *sdl_ctx, button **b, const char *text, SDL_FRect rect, char *baseImgPath, char *hoverImgPath,
                  char *clickImgPath);
void updateButtonState(button *b, V2f mouseCoord, int mouseFlag);
void buttonRender(sdl_ctx_t *sdl_ctx, button *b);
void destroyButton(button **b);

#endif // BUTTONS_H_

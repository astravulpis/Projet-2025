/**
 * @file buttons.c
 * @brief File to implement a button
 *
 * Author: Rossignol François <francois_rossignol@outlook.fr>
 * Last Modified: 2026-03-26
 * Date: 2026-03-22
 *
 * * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 * Liam B. <liam.berge72@gmail.com>
 **/

#include "buttons.h"
#include "common.h"
#include "sdl_helpers.h"

button *initButton(SDL_FRect *b_box, char *b_text, SDL_Color *b_baseC, SDL_Color *b_hoverC, SDL_Color *b_clickC)
{
    // création du boutton
    button *b = malloc(sizeof(button));

    // contenu et taille du bouton
    // pas de copie des chaines, juste un pointeur vers la chaine
    //(comme ça au peut modifier le contenu dans le main)
    b->buttonText = b_text;
    // b->buttonhoveredText = b_hoveredText;
    b->buttonBox = b_box;

    // couleurs du boutton
    b->baseColor = *b_baseC;
    b->hoverColor = *b_hoverC;
    b->clickColor = *b_clickC;

    // variables d'état du bouton
    b->hovered = false;
    b->leftClicked = false;
    b->rightClicked = false;

    return b;
}

void destroyButton(button **b)
{
    free((*b)->buttonBox);
    free(*b);
    b = NULL;
}

void updateButtonState(button *b, SDL_FPoint mouseCoord, int mouseFlag)
{
    if (SDL_PointInRectFloat(&mouseCoord, b->buttonBox)) {

        b->hovered = true;

        // clici gauche
        if (mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) b->leftClicked = true;
        else
            b->leftClicked = false;

        // clic droit
        if (mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)) b->rightClicked = true;
        else
            b->rightClicked = false;

    } else { // par sécurité on remet tout a False
        b->hovered = false;
        b->leftClicked = false;
        b->rightClicked = false;
    }
}

void buttonRender(sdl_ctx_t *sdl_ctx, button *b)
{
    static size_t pixelWidth = 0;
    static int buttonTextWidth = 0;
    static int buttonTextHeight = 0;

    // Measure the width of the button's text
    TTF_MeasureString(sdl_ctx->font, temp_sprintf("%s", b->buttonText), 0, 0, NULL, &pixelWidth);
    TTF_GetStringSize(sdl_ctx->font, b->buttonText, pixelWidth, &buttonTextWidth, &buttonTextHeight);

    // Calculates the center of the button
    float XCentering = ((b->buttonBox)->w / 2.0f) - buttonTextWidth / 2.0f;
    float YCentering = ((b->buttonBox)->h / 2.0f) - buttonTextHeight / 2.0f;
    V2f buttonPos = {(b->buttonBox)->x + XCentering, (b->buttonBox)->y + YCentering};

    if (b->leftClicked || b->rightClicked) {
        renderFillRect(sdl_ctx->renderer, b->buttonBox, b->clickColor);
    } else if (b->hovered) {
        renderFillRect(sdl_ctx->renderer, b->buttonBox, b->hoverColor);
    } else {
        // rendu de la'arrière plan du bouton
        renderFillRect(sdl_ctx->renderer, b->buttonBox, b->baseColor);
    }
    renderText_Ex(sdl_ctx, temp_sprintf("%s", b->buttonText), WHITE, buttonPos);
}

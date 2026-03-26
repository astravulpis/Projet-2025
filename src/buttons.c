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

bool createButton(button **b, const char *text, SDL_FRect rect,
                  SDL_Color baseColor, SDL_Color hoveredColor, SDL_Color clickedColor)
{
    *b = calloc(1, sizeof(button));
    if ((*b) == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for button", __FILE__, __LINE__);
        return false;
    }

    (*b)->buttonText = strdup(text);
    (*b)->buttonBox = createRect_Ex(rect);

    (*b)->baseColor = baseColor;
    (*b)->hoverColor = hoveredColor;
    (*b)->clickColor = clickedColor;

    (*b)->isHovered = false;
    (*b)->isLeftClicked = false;
    (*b)->isRightClicked = false;

    return true;
}

void destroyButton(button **b)
{
    free((*b)->buttonText);
    (*b)->buttonText = NULL;
    free((*b)->buttonBox);
    (*b)->buttonBox = NULL;
    free(*b);
    b = NULL;
}

void updateButtonState(button *b, V2f mouseCoord, int mouseFlag)
{
    SDL_FPoint coords = (SDL_FPoint){mouseCoord.x, mouseCoord.y};
    if (SDL_PointInRectFloat(&coords, b->buttonBox)) {

        b->isHovered = true;

        // Right click
        if (mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) b->isLeftClicked = true;
        else
            b->isLeftClicked = false;

        // Right click
        if (mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)) b->isRightClicked = true;
        else
            b->isRightClicked = false;

    } else {
        // If no collision is detected: reset
        b->isHovered = false;
        b->isLeftClicked = false;
        b->isRightClicked = false;
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

    if (b->isLeftClicked || b->isRightClicked) {
        renderFillRect(sdl_ctx->renderer, b->buttonBox, b->clickColor);
    } else if (b->isHovered) {
        renderFillRect(sdl_ctx->renderer, b->buttonBox, b->hoverColor);
    } else {
        // rendu de la'arrière plan du bouton
        renderFillRect(sdl_ctx->renderer, b->buttonBox, b->baseColor);
    }
    renderText_Ex(sdl_ctx, temp_sprintf("%s", b->buttonText), WHITE, buttonPos);
}

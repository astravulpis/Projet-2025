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

bool createButton(sdl_ctx_t *sdl_ctx, button **b, const char *text, SDL_FRect rect, char *baseImgPath, char *hoverImgPath,
                  char *clickImgPath)
{
    *b = calloc(1, sizeof(button));
    if ((*b) == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for button", __FILE__, __LINE__);
        return false;
    }

    (*b)->buttonText = strdup(text);
    (*b)->buttonBox = createRect_Ex(rect);

    (*b)->baseImg = IMG_LoadTexture(sdl_ctx->renderer, baseImgPath);
    (*b)->hoverImg = IMG_LoadTexture(sdl_ctx->renderer, hoverImgPath);
    (*b)->clickImg = IMG_LoadTexture(sdl_ctx->renderer, clickImgPath);

    // j'ai mis cela car sinon le rendu lisse l'image, alors que moi, j'aime les pixels
    SDL_SetTextureScaleMode((*b)->baseImg, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode((*b)->hoverImg, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode((*b)->clickImg, SDL_SCALEMODE_NEAREST);

    (*b)->isHovered = false;
    (*b)->isLeftClicked = false;
    (*b)->isRightClicked = false;

    return true;
}

void destroyButton(button **b)
{
    SDL_DestroyTexture((*b)->baseImg);
    SDL_DestroyTexture((*b)->hoverImg);
    SDL_DestroyTexture((*b)->clickImg);

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

void renderButton(sdl_ctx_t *sdl_ctx, button *b)
{
    size_t mark = temp_save();
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
        if (b->clickImg == NULL) // si l'image n'a pa été chargée avec succès un rectangle s'affiche par défaut
            renderFillRect(sdl_ctx->renderer, b->buttonBox, (SDL_Color){20, 20, 20, 255});
        else
            renderImage(sdl_ctx, b->clickImg, b->buttonBox);
    } else if (b->isHovered) {
        if (b->hoverImg == NULL) renderFillRect(sdl_ctx->renderer, b->buttonBox, (SDL_Color){80, 80, 80, 255});
        else
            renderImage(sdl_ctx, b->hoverImg, b->buttonBox);
    } else {
        // rendu de la'arrière plan du bouton
        if (b->baseImg == NULL) renderFillRect(sdl_ctx->renderer, b->buttonBox, (SDL_Color){60, 60, 60, 255});
        else
            renderImage(sdl_ctx, b->baseImg, b->buttonBox);
    }
    renderText_Ex(sdl_ctx, temp_sprintf("%s", b->buttonText), WHITE, buttonPos);
    temp_rewind(mark);
}

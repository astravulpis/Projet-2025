/**
 * @file sliders.c
 * @brief File to implement a text input box
 *
 * Author: Rossignol François <francois_rossignol@outlook.fr>
 * Last Modified: 2026-03-30
 * Date: 2026-03-30
 *
 * * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#include "sliders.h"
#include "common.h"
#include "sdl_helpers.h"
#include <math.h>

bool createSlider(sdl_ctx_t *sdl_ctx, slider **s, SDL_FRect rect, char *bgImg_Path, char *cursorImg_Path, float nbValue,
                  float borderSize)
{
    *s = calloc(1, sizeof(slider));
    if ((*s) == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for sliders", __FILE__, __LINE__);
        return false;
    }

    float cursorSize = (rect.w - borderSize * 2.0f) / 20.0f;
    (*s)->sliderBox = createRect_Ex(rect);
    (*s)->cursorBox =
        createRect_Ex((SDL_FRect){rect.x + (borderSize), rect.y + (borderSize), cursorSize, ((rect.h - borderSize * 2))});

    if (bgImg_Path != NULL) (*s)->backgroundImg = IMG_LoadTexture(sdl_ctx->renderer, bgImg_Path);
    else
        (*s)->backgroundImg = NULL;

    if (cursorImg_Path != NULL) (*s)->cursorImg = IMG_LoadTexture(sdl_ctx->renderer, cursorImg_Path);
    else
        (*s)->cursorImg = NULL;

    (*s)->nbValue = nbValue;
    (*s)->borderSize = borderSize;
    (*s)->currentValue = 1;
    (*s)->prevX = -1.0f;

    (*s)->hovered = false;
    (*s)->clicked = false;
    (*s)->focused = false;

    return true;
}

void destroySliders(slider **s)
{
    SDL_DestroyTexture((*s)->backgroundImg);
    SDL_DestroyTexture((*s)->cursorImg);

    free(*s);
    s = NULL;
}

// incrémente le X d'un FRect, selon un step, et permet de limiter la valeur dans un interval
void FRect_Change_x(SDL_FRect *rect, float step, float minLimit, float maxLimit)
{
    bool inLimits = ((rect->x + step) >= minLimit) && ((rect->x + step) <= maxLimit);

    if (inLimits) {
        rect->x = (rect->x + step);
        return;
    }

    if (rect->x + step > maxLimit) {
        rect->x = maxLimit;
        return;
    }

    else if (rect->x + step < minLimit) {
        rect->x = minLimit;
        return;
    }

    // normalement on n'arrive jamais ici, mais si un malheur se produit, alors, rien ne se passe
    return;
}

void updateSliderStates(slider *s, V2f mouseCoord, int mouseFlag, sdl_ctx_t *sdl_ctx)
{
    SDL_FPoint coords = (SDL_FPoint){mouseCoord.x, mouseCoord.y};

    // vérification pour voir si le curseur subis un focus (cliqué précédemment, et encore en clic)
    if (s->clicked && ((mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) || (mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT))))
        s->focused = true;
    else
        s->focused = false;

    // mise a jour des autres booléens, et increment du x de cursor box si focused
    if (SDL_PointInRectFloat(&coords, s->cursorBox)) {
        s->hovered = true;

        // Right or Left click, pas de distinction pour ce composant
        if ((mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) || (mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT))) {
            s->clicked = true;
        } else
            s->clicked = false;

        s->prevX = mouseCoord.x;
    } else if (s->focused) {
        float difference = mouseCoord.x - s->prevX; // prevX est normalement toujours différent de -1 si on est arrivé ici
        float step = (s->sliderBox->w - (2 * s->borderSize) - s->cursorBox->w) / (float)(s->nbValue - 1);

        if (fabsf(difference) >= step) {
            float moveStep = roundf(difference / step);
            float move = moveStep * step;

            if (move != 0) {
                float minX = s->sliderBox->x + s->borderSize;
                float maxX = s->sliderBox->x + s->sliderBox->w - s->cursorBox->w - s->borderSize;

                // Moves the cursor & update the value
                FRect_Change_x(s->cursorBox, move, minX, maxX);

                // Calculate the current index
                s->currentValue = roundf((s->cursorBox->x - minX) / step);
                s->prevX += move;
            }
        }
    } else {
        s->hovered = false;
        s->clicked = false;
        s->focused = false;
        s->prevX = -1.0f;
    }
}

void renderSlider(sdl_ctx_t *sdl_ctx, slider *s)
{
    // vérifie qu'une image a été chargée pour le slider background
    if (s->backgroundImg == NULL) renderFillRect(sdl_ctx->renderer, s->sliderBox, (SDL_Color){50, 50, 50, 255});
    else
        renderImage(sdl_ctx, s->backgroundImg, s->sliderBox);

    // vérifie qu'une image a été chargé pour le curseur
    if (s->cursorImg == NULL) {
        renderFillRect(sdl_ctx->renderer, s->cursorBox, (SDL_Color){250, 250, 250, 255});
        renderFillRect(sdl_ctx->renderer, &(SDL_FRect){s->cursorBox->x, s->cursorBox->y, s->cursorBox->w, s->cursorBox->h / 5},
                       (SDL_Color){200, 200, 200, 255});

        if (s->clicked) renderFillRect(sdl_ctx->renderer, s->cursorBox, (SDL_Color){50, 50, 50, 128});
    } else
        renderImage(sdl_ctx, s->backgroundImg, s->cursorBox);

    // Calculates the center of the button
    static int sliderTextWidth = 0;
    static int sliderTextHeight = 0;

    // Measure the width of the button's text
    TTF_GetStringSize(sdl_ctx->font, temp_sprintf("value = %i", s->currentValue), 0, &sliderTextWidth, &sliderTextHeight);

    float XCentering = (s->sliderBox->w / 2.0f) - sliderTextWidth / 2.0f;
    float YCentering = (s->sliderBox->h / 2.0f) - sliderTextHeight / 2.0f;
    V2f textPos = {s->sliderBox->x + XCentering, s->sliderBox->y + YCentering};
    renderText_Ex(sdl_ctx, temp_sprintf("value = %i", s->currentValue), WHITE, textPos);
}

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

bool createSlider(slider **s, SDL_FRect rect, float nbValue, float borderSize, float baseVal, const char *name)
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

    (*s)->nbValue = nbValue;
    (*s)->borderSize = borderSize;
    (*s)->currentValue = baseVal;
    (*s)->prevX = -1.0f;
    (*s)->name = strdup(name);

    (*s)->hovered = false;
    (*s)->clicked = false;
    (*s)->focused = false;

    return true;
}

void destroySlider(slider **s)
{
    if ((*s) != NULL) {
        free((*s)->sliderBox);
        free((*s)->cursorBox);
        free((*s)->name);
    }

    free(*s);
    s = NULL;
}

// incrémente le X d'un FRect, selon un step, et permet de limiter la valeur dans une intervale
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

void updateSliderStates(slider *s, V2f mouseCoord, int mouseFlag, float *val)
{
    SDL_FPoint coords = (SDL_FPoint){mouseCoord.x, mouseCoord.y};

    // vérification pour voir si le curseur subis un focus (cliqué précédemment, et encore en clic)
    s->focused = s->clicked && mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);

    // mise a jour des autres booléens, et increment du x de cursor box si focused
    if (SDL_PointInRectFloat(&coords, s->cursorBox)) {
        s->hovered = true;
        s->clicked = mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);
        s->prevX = mouseCoord.x;
    } else if (s->focused) {
        // prevX est normalement toujours différent de -1 si on est arrivé ici
        // difference représente la distance entre la dernière position X de la souris et l'actuelle
        float difference = mouseCoord.x - s->prevX;
        // step est la largeur d'une valeur dans la représentation du slider
        float step = (s->sliderBox->w - (2 * s->borderSize) - s->cursorBox->w) / (float)(s->nbValue - 1);

        // si la différence est supérieure a une largeur de valeur, alors on bouge le curseur en fonction de cela
        if (fabsf(difference) >= step) {
            // arrondi utile pour avoir un 'snap' des différentes valeurs, cela empêche le curseur de se trouver n'importe ou
            // sur le slider
            float moveStep = roundf(difference / step);
            float move = moveStep * step; // déplacement que va subir le curseur ->  cursorBox.x + move (négatif ou positif)

            if (move != 0) {
                float minX = s->sliderBox->x + s->borderSize;
                float maxX = s->sliderBox->x + s->sliderBox->w - s->cursorBox->w - s->borderSize;

                // Moves the cursor & update the value
                FRect_Change_x(s->cursorBox, move, minX, maxX);

                // Calculate the current index
                s->currentValue = roundf((s->cursorBox->x - minX) / step);
                if (val != NULL) *val = s->currentValue;
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
    size_t mark = temp_save();
    renderFillRect(sdl_ctx->renderer, s->sliderBox, (SDL_Color){50, 50, 50, 255});

    renderFillRect(sdl_ctx->renderer, s->cursorBox, (SDL_Color){250, 250, 250, 255});
    renderFillRect(sdl_ctx->renderer, &(SDL_FRect){s->cursorBox->x, s->cursorBox->y, s->cursorBox->w, s->cursorBox->h / 5},
                   (SDL_Color){200, 200, 200, 255});

    if (s->clicked) renderFillRect(sdl_ctx->renderer, s->cursorBox, (SDL_Color){50, 50, 50, 128});

    // Calculates the center of the slider
    static int sliderTextWidth = 0;
    static int sliderTextHeight = 0;

    // Measure the width of the slider text
    TTF_GetStringSize(sdl_ctx->font, temp_sprintf("%s: %i", s->name, s->currentValue), 0, &sliderTextWidth, &sliderTextHeight);

    float XCentering = (s->sliderBox->w / 2.0f) - sliderTextWidth / 2.0f;
    float YCentering = (s->sliderBox->h / 2.0f) - sliderTextHeight / 2.0f;
    V2f textPos = {s->sliderBox->x + XCentering, s->sliderBox->y + YCentering};
    renderText_Ex(sdl_ctx, temp_sprintf("%s: %i", s->name, s->currentValue), WHITE, textPos);
    temp_rewind(mark);
}

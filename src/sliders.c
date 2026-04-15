/**
 * @file sliders.c
 * @brief File to implement a slider
 *
 * @author Rossignol François <francois_rossignol@outlook.fr>
 * @date 2026-03-30
 * @remark last Modified: 2026-04-07
 *
 * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 * Liam B. <liam.berge72@gmail.com>
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
    float xBox = rect.x + (borderSize) + baseVal * nbValue / cursorSize;
    float yBox = rect.y + (borderSize);
    (*s)->sliderBox = createRect_Ex(rect);
    (*s)->cursorBox = createRect(xBox, yBox, cursorSize, ((rect.h - borderSize * 2)));

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
    *s = NULL;
}

// Increments the X value of an FRect by a specified step and allows the value to be restricted to a specific range
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

    // Normally, we never end up here, but if something bad happens, then nothing happens
    return;
}

void updateSliderStates(slider *s, mouseDevice mouse, float *val)
{
    SDL_FPoint coords = (SDL_FPoint){mouse.position.x, mouse.position.y};

    // Check whether the cursor has focus (was previously clicked and is still being clicked)
    s->focused = s->clicked && mouse.currState & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);

    // Update the other Booleans, and increment the cursor box's x value if it is focused
    if (SDL_PointInRectFloat(&coords, s->cursorBox)) {
        s->hovered = true;
        s->clicked = mouse.currState & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);
        s->prevX = coords.x;
    } else if (s->focused) {
        // prevX is normally always different from -1 if you've reached this point; difference
        // represents the distance between the mouse's last X position and the current one
        float difference = coords.x - s->prevX;
        // step est la largeur d'une valeur dans la représentation du slider
        float step = (s->sliderBox->w - (2 * s->borderSize) - s->cursorBox->w) / (float)(s->nbValue - 1);

        // If the difference is greater than a certain threshold, then the cursor is moved accordingly
        if (fabsf(difference) >= step) {
            // Useful rounding to get a “snap” to different values; this prevents the cursor from ending up anywhere on the
            // slider
            float moveStep = roundf(difference / step);
            float move = moveStep * step; // the distance the cursor will move -> cursorBox.x + move (negative or positive)

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

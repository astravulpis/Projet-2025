/**
 * @file bars.c
 * @brief File to implement a \ref bar (create, update, render and destroy)
 *
 * @author Rossignol François <francois_rossignol@outlook.fr>
 * @date 2026-03-31
 * @remark last modified 2026-02-07
 *
 * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 * Liam B. <liam.berge72@gmail.com>
 **/

#include "bars.h"
#include "common.h"
#include "sdl_helpers.h"
#include <math.h>

bool createBar(bar **h, SDL_FRect rect, SDL_Color bgColor, SDL_Color fillColor, SDL_Color cursorColor, float baseVal,
               float barPadding, float minCursorWidth, bool displayHpValue)
{
    *h = calloc(1, sizeof(bar));
    if ((*h) == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for  bar", __FILE__, __LINE__);
    }

    (*h)->BarBox = createRect_Ex(rect);
    (*h)->bgColor = bgColor;
    (*h)->fillColor = fillColor;
    (*h)->cursorColor = cursorColor;

    (*h)->val = baseVal;
    (*h)->barPadding = barPadding;
    (*h)->minCursorWidth = minCursorWidth; // must have experienced a ratio

    (*h)->displayHpValue = displayHpValue; // By default, hpValue will be displayed as a number in addition to the gauge

    return true;
}

void destroyBar(bar **h)
{
    if (*h != NULL) {
        free((*h)->BarBox);
        (*h)->BarBox = NULL;
    }

    free(*h);
    *h = NULL;
}

// s = shadow et l = light
void barRender(sdl_ctx_t *sdl_ctx, bar *h, float hpValue, int s_intensity, int l_intensity, int ls_opacity)
{
    //Compare hpValue to the bar's maximum value, copy the value to hp, scale it down if it's too large, and set it to 0 if it's negative.
    // This only applies to the gauge, not the displayed number (it would be useful to have the actual value stored as a parameter for potential debugging).
    float hp = hpValue;
    if (hpValue > h->val) hp = h->val;
    else if (hpValue < 0)
        hp = 0;

    // Calculate fillBox using barPadding (which is normally smaller than h->BarBox; otherwise, there's a problem XD), 
    // with widths calculated based on hp; represents the health bar
    SDL_FRect fillBox = {(h->BarBox)->x + ((h->barPadding / 2) * sdl_ctx->screenRatio),
                         (h->BarBox)->y + ((h->barPadding / 2) * sdl_ctx->screenRatio),
                         ((h->BarBox)->w - (h->barPadding * sdl_ctx->screenRatio)) / h->val * hp,
                         (h->BarBox)->h - (h->barPadding * sdl_ctx->screenRatio)};

    // Calculation of emptyFillBox, which represents the space not occupied by fillBox
    SDL_FRect emptyFillBox = {fillBox.x + fillBox.w, fillBox.y,
                              ((h->BarBox)->w - (h->barPadding * sdl_ctx->screenRatio)) / h->val * (h->val - hp), fillBox.h};
    SDL_Color emptyBgColor = {(h->bgColor.r + 20) < 255 ? (h->bgColor.r + 20) : 255,
                              (h->bgColor.g + 20) < 255 ? (h->bgColor.g + 20) : 255,
                              (h->bgColor.b + 20) < 255 ? (h->bgColor.b + 20) : 255, 200};

    // rectangles and fill color for shadow effects on the health bar
    SDL_FRect horizontal_shadowFillBox = {fillBox.x, fillBox.y + fillBox.h - (h->barPadding * sdl_ctx->screenRatio), fillBox.w,
                                          h->barPadding * sdl_ctx->screenRatio};
    SDL_FRect vertical_shadowFillBox = {fillBox.x, fillBox.y, h->barPadding * sdl_ctx->screenRatio, fillBox.h};
    SDL_Color shadowColor = {
        ((h->fillColor.r - s_intensity) >= 0) && ((h->fillColor.r - s_intensity) <= 255) ? (h->fillColor.r - s_intensity) : 0,
        ((h->fillColor.g - s_intensity) >= 0) && ((h->fillColor.g - s_intensity) <= 255) ? (h->fillColor.g - s_intensity) : 0,
        ((h->fillColor.b - s_intensity) >= 0) && ((h->fillColor.b - s_intensity) <= 255) ? (h->fillColor.b - s_intensity) : 0,
        (ls_opacity >= 0 && ls_opacity <= 255) ? ls_opacity
                                               : 255}; // If the opacity isn't set correctly, the shadow won't be transparent

    // rectangles et couleur de rendu pour effets de lumières sur la barre de vie
    SDL_FRect horizontal_lightFillBox = {fillBox.x, fillBox.y, fillBox.w, h->barPadding * sdl_ctx->screenRatio};
    SDL_FRect vertical_lightFillBox = {fillBox.x + fillBox.w - (h->barPadding * sdl_ctx->screenRatio), fillBox.y,
                                       h->barPadding * sdl_ctx->screenRatio, fillBox.h};
    SDL_Color lightColor = {
        ((h->fillColor.r + l_intensity) >= 0) && ((h->fillColor.r + l_intensity) <= 255) ? (h->fillColor.r + l_intensity) : 255,
        ((h->fillColor.g + l_intensity) >= 0) && ((h->fillColor.g + l_intensity) <= 255) ? (h->fillColor.g + l_intensity) : 255,
        ((h->fillColor.b + l_intensity) >= 0) && ((h->fillColor.b + l_intensity) <= 255) ? (h->fillColor.b + l_intensity) : 255,
        (ls_opacity >= 0 && ls_opacity <= 255) ? (ls_opacity * (hp / h->val))
                                               : 255}; // If the opacity isn't set correctly, the shadow won't be transparent

    // Initialization of boxes and colors for the cursor and its shadows/highlights 
    // (fmaxf is used to ensure that the width is always greater than or equal to minCursorWidth)
    SDL_FRect cursorBox = {fillBox.x + fillBox.w - (h->barPadding * sdl_ctx->screenRatio / 2), fillBox.y,
                           fmaxf(h->barPadding * sdl_ctx->screenRatio / 2, h->minCursorWidth), fillBox.h};

    SDL_FRect cursorShadowBox = {cursorBox.x, cursorBox.y, cursorBox.w / 3, cursorBox.h};
    SDL_Color cursorShadowColor = {50, 50, 50, 100}; // Fixed values—I might revisit this later, but it seems to me that the health bars are already pretty customizable

    SDL_FRect cursorLightBox = {cursorBox.x + cursorBox.w - (cursorBox.w / 3), cursorBox.y, cursorBox.w / 3, cursorBox.h};
    SDL_Color cursorLightColor = {205, 205, 205, 100}; // Fixed values—I might revisit this later, but it seems to me that the health bars are already pretty customizable

    // rendering of the bar and its background
    renderFillRect(sdl_ctx->renderer, h->BarBox, h->bgColor);
    renderFillRect(sdl_ctx->renderer, &fillBox, h->fillColor);
    renderFillRect(sdl_ctx->renderer, &emptyFillBox, emptyBgColor);

    // shading on the health bar
    renderFillRect(sdl_ctx->renderer, &horizontal_shadowFillBox, shadowColor);
    renderFillRect(sdl_ctx->renderer, &vertical_shadowFillBox, shadowColor);
    renderFillRect(sdl_ctx->renderer, &horizontal_lightFillBox, lightColor);
    renderFillRect(sdl_ctx->renderer, &vertical_lightFillBox, lightColor);

    // Cursor rendering and its shadows/highlights
    renderFillRect(sdl_ctx->renderer, &cursorBox, h->cursorColor);
    renderFillRect(sdl_ctx->renderer, &cursorShadowBox, cursorShadowColor);
    renderFillRect(sdl_ctx->renderer, &cursorLightBox, cursorLightColor);

    if (h->displayHpValue) { // Displays the hpValue numerically in addition to the gauge if the boolean is true
        int textWidth = 0;
        int textHeight = 0;

        size_t mark = temp_save();
        char *hpText = temp_sprintf("%.1f", hpValue);
        temp_rewind(mark);

        TTF_GetStringSize(sdl_ctx->font, hpText, 0, &textWidth, &textHeight);

        // Calculates the center of the life point information text
        float XCentering = ((h->BarBox)->w / 2.0f) - textWidth / 2.0f;
        float YCentering = ((h->BarBox)->h / 2.0f) - textHeight / 2.0f;

        V2f barTextPos = {(h->BarBox)->x + XCentering, (h->BarBox)->y + YCentering};
        V2f barTextPosShadow = {(h->BarBox)->x + XCentering + 2 * sdl_ctx->screenRatio,
                                (h->BarBox)->y + YCentering + 1 * sdl_ctx->screenRatio};

        // text rendering, with a shadow effect that perfectly mimics ray tracing
        renderText_Ex(sdl_ctx, hpText, BLACK, barTextPosShadow);
        renderText_Ex(sdl_ctx, hpText, WHITE, barTextPos);
    }
}

/**
 * @file checkboxes.c
 * @brief File to implement a checkbox (create, update, render and destroy)
 *
 * @author Rossignol François <francois_rossignol@outlook.fr>
 * @date 2026-03-31
 * @remark last modified 2026-03-31
 *
 * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#include "checkboxes.h"
#include "common.h"
#include "sdl_helpers.h"
#include <math.h>

bool createCheckbox(sdl_ctx_t *sdl_ctx, checkbox **c, SDL_FRect checkRect, SDL_FRect tickRect, char *bgImg_Path,
                    char *tickImg_Path, char *tickHoverImg_Path, float spaceWithText, float boxBorderSize)
{
    *c = calloc(1, sizeof(checkbox));
    if ((*c) == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for sliders", __FILE__, __LINE__);
        return false;
    }

    // FRect are created
    (*c)->checkBox = createRect_Ex(checkRect);
    (*c)->tickBox = createRect_Ex(tickRect);

    boxToScale((*c)->checkBox, sdl_ctx->screenRatio);
    boxToScale((*c)->tickBox, sdl_ctx->screenRatio);

    // Images are loaded
    if (bgImg_Path != NULL) (*c)->checkboxImg = IMG_LoadTexture(sdl_ctx->renderer, bgImg_Path);
    else
        (*c)->checkboxImg = NULL;

    if (tickImg_Path != NULL) (*c)->tickImg = IMG_LoadTexture(sdl_ctx->renderer, tickImg_Path);
    else
        (*c)->tickImg = NULL;

    if (tickHoverImg_Path != NULL) (*c)->tickHoverImg = IMG_LoadTexture(sdl_ctx->renderer, tickHoverImg_Path);
    else
        (*c)->tickHoverImg = NULL;

    // it's used to have sharp images, by default the images are upscaled and smooth, here we want pixel !
    SDL_SetTextureScaleMode((*c)->checkboxImg, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode((*c)->tickImg, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode((*c)->tickHoverImg, SDL_SCALEMODE_NEAREST);

    (*c)->spaceWithText = spaceWithText * sdl_ctx->screenRatio;
    (*c)->boxBorderSize =
        boxBorderSize; // its usefull only if there is no image loaded (error or NULL in path parameter)

    (*c)->hovered = false;
    (*c)->prevClicked = false;
    (*c)->clicked = false;
    (*c)->checked = false;

    return true;
}

void destroyCheckbox(checkbox **c)
{
    free((*c)->checkBox);
    free((*c)->tickBox);
    SDL_DestroyTexture((*c)->checkboxImg);
    SDL_DestroyTexture((*c)->tickImg);
    SDL_DestroyTexture((*c)->tickHoverImg);

    free(*c);
    *c = NULL;
}

void updateCheckboxStates(checkbox *c, V2f mouseCoord, int mouseFlag)
{
    SDL_FPoint coords = (SDL_FPoint){mouseCoord.x, mouseCoord.y};

    if (SDL_PointInRectFloat(&coords, c->checkBox)) {
        c->hovered = true;

        if (c->clicked) // prevents the checkbox from toggling between true and false when you hold down the click
            c->prevClicked = true;

        // Right-click or left-click—it makes no difference for this component
        if ((mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) || (mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT))) {
            c->clicked = true;

            // The `checked` attribute is updated only if the checkbox has not already been clicked in the previous frame
            if (!c->checked && !c->prevClicked) c->checked = true;
            else if (c->checked && !c->prevClicked)
                c->checked = false;
        } else {
            // Set the two Booleans associated with clicks to false if there is no hover
            c->clicked = false;
            c->prevClicked = false;
        }
    } else {
        c->hovered = false;
        c->prevClicked = false;
        c->clicked = false;
    }
}

void renderCheckbox(sdl_ctx_t *sdl_ctx, checkbox *c, char *text)
{
    // render of the background
    if (c->checkboxImg == NULL) { // image inexistante

        // Everything that comes in here must have already been screen-ratio-adjusted
        SDL_FRect insideBox = {
            c->checkBox->x + c->boxBorderSize,
            c->checkBox->y + c->boxBorderSize,
            c->checkBox->w - (c->boxBorderSize * 2),
            c->checkBox->h - (c->boxBorderSize * 2),
        };

        // Since there is supposed to be an image for the box, there is no option to set the color of the placeholder rectangles
        renderFillRect(sdl_ctx->renderer, c->checkBox, (SDL_Color){25, 25, 25, 255});

        // changes the color of insideBox when hovered over or when not hovered over
        if (c->hovered) {
            renderFillRect(sdl_ctx->renderer, &insideBox, (SDL_Color){40, 40, 40, 255});
        } else
            renderFillRect(sdl_ctx->renderer, &insideBox, (SDL_Color){35, 35, 35, 255});
    } else {
        renderImage(sdl_ctx, c->checkboxImg, c->checkBox);
    }

    // Render a ghost version of the image/rect from the tick, if the checkbox is hovered over and is not checked
    if (!c->checked && c->hovered) {
        if (c->tickHoverImg == NULL) // Image not found
            renderFillRect(sdl_ctx->renderer, c->tickBox,
                           (SDL_Color){50, 50, 50, 50}); // displays the tick rectangle with transparency
        else
            renderImage(sdl_ctx, c->tickHoverImg, c->tickBox);
    }

    // display the checkmark if the box is checked
    if (c->checked) {
        if (c->tickImg == NULL) renderFillRect(sdl_ctx->renderer, c->tickBox, (SDL_Color){19, 179, 19, 255});
        else
            renderImage(sdl_ctx, c->tickImg, c->tickBox);
    }

    // When rendered as text, `spaceWithText` must have been multiplied by the ratio by the time it reaches this point
    int checkTextHeight = 0;

    // Measure the width of the button's text
    TTF_GetStringSize(sdl_ctx->font, text, 0, NULL, &checkTextHeight);

    float YCentering = (c->checkBox->h / 2.0f) - checkTextHeight / 2.0f;

    // The text will appear next to the checkbox, depending on `spaceWithText`, and will be vertically centered relative to the checkbox attribute.
    V2f textPos = {c->checkBox->x + c->checkBox->w + c->spaceWithText, c->checkBox->y + YCentering};

    renderText_Ex(sdl_ctx, temp_sprintf("%s", text), WHITE, textPos);
}

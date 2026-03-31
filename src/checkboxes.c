#include "checkboxes.h"
#include "common.h"
#include "sdl_helpers.h"
#include <math.h>

bool createCheckbox(sdl_ctx_t *sdl_ctx, checkbox **c, SDL_FRect checkRect, SDL_FRect tickRect, char *bgImg_Path, char *tickImg_Path, char *tickHoverImg_Path, float spaceWithText, float boxBorderSize)
{
    *c = calloc(1, sizeof(checkbox));
    if ((*c) == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for sliders", __FILE__, __LINE__);
        return false;
    }

    // création des FRect
    (*c)->checkBox = createRect_Ex(checkRect);
    (*c)->tickBox = createRect_Ex(tickRect);

    // chargement des images
    if (bgImg_Path != NULL) (*c)->checkboxImg = IMG_LoadTexture(sdl_ctx->renderer, bgImg_Path);
    else
        (*c)->checkboxImg = NULL;

    if (tickImg_Path != NULL) (*c)->tickImg = IMG_LoadTexture(sdl_ctx->renderer, tickImg_Path);
    else
        (*c)->tickImg = NULL;

    if (tickImg_Path != NULL) (*c)->tickHoverImg = IMG_LoadTexture(sdl_ctx->renderer, tickHoverImg_Path);
    else
        (*c)->tickHoverImg = NULL;

    (*c)->spaceWithText = spaceWithText;
    (*c)->boxBorderSize = boxBorderSize; // utile seulement si il n'y pas d'image de chargé -> utilisé par insideBox dans le rendu

    (*c)->hovered = false;
    (*c)->prevClicked = false;
    (*c)->clicked = false;
    (*c)->checked = false;

    return true;
}

void destroyCheckbox(checkbox **c)
{
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

        if (c->clicked) // empêche le checked d'osciller entre true et false lorsque l'ont maintiens le clic
            c->prevClicked = true;

        // Right or Left click, pas de distinction pour ce composant
        if ((mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) || (mouseFlag & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT))){
            c->clicked = true;
            
            // checked est mis a jour seulement si la checkBox n'a pas déja été clickée a la frame précédente
            if (!c->checked && !c->prevClicked)
                c->checked = true;
            else if (c->checked && !c->prevClicked)
                c->checked = false;   
        } 
        else {
            // remise des 2 booléens liés aux clics a false si pas de hover
            c->clicked = false;
            c->prevClicked = false;
        }
    }
    else {
        c->hovered = false;
        c->prevClicked = false;
        c->clicked = false;
    }
}

void renderCheckbox(sdl_ctx_t *sdl_ctx, checkbox *c, char *text)
{   
    // rendu de l'arrière plan
    if (c->checkboxImg == NULL) { // image inexistante

        // tout ce qui arrive ici doit déja avoir subi un screenRatio
        SDL_FRect insideBox = {c->checkBox->x + c->boxBorderSize,
                               c->checkBox->y + c->boxBorderSize,
                               c->checkBox->w - (c->boxBorderSize * 2),
                               c->checkBox->h - (c->boxBorderSize * 2),};

        // étant donné qu'il est censé exister une image pour la box, on ne donne pas le moyen de paramétrer la couleur des rectangles de remplacement
        renderFillRect(sdl_ctx->renderer, c->checkBox, (SDL_Color){25, 25, 25, 255});

        // change la couleur de insideBox si hover ou !hover
        if (c->hovered){
            renderFillRect(sdl_ctx->renderer, &insideBox, (SDL_Color){40, 40, 40, 255});
        }
        else
            renderFillRect(sdl_ctx->renderer, &insideBox, (SDL_Color){35, 35, 35, 255});
    }
    else {
        renderImage(sdl_ctx, c->checkboxImg, c->checkBox);
    }

    // rendu d'un fantôme du de l'image/rect du tick, si la box est survolée, et qu'elle n'est pas checked
    if (!c->checked && c->hovered){
        if (c->tickHoverImg == NULL) // image inexistante
            renderFillRect(sdl_ctx->renderer, c->tickBox, (SDL_Color){50, 50, 50, 50}); // affiche le rectangle du tick avec une transparence
        else
            renderImage(sdl_ctx, c->tickHoverImg, c->tickBox);
    }

    //rendu du tick, si la box est bien checked
    if (c->checked){
        if (c->tickImg == NULL)
            renderFillRect(sdl_ctx->renderer, c->tickBox, (SDL_Color){19, 179, 19, 255});
        else
            renderImage(sdl_ctx, c->tickImg, c->tickBox);
    }     

    //rendu texte, spaceWithText doit avoir été multiplié par le ratio quand il arrive ici
    int checkTextHeight = 0;

    // Measure the width of the button's text
    TTF_GetStringSize(sdl_ctx->font, text, 0, NULL, &checkTextHeight);

    float YCentering = (c->checkBox->h / 2.0f) - checkTextHeight / 2.0f;

    // le texte va se trouver a côté du carré de checkbox, en fonction de spaceWithText et sera centré verticalement par rapport a checkBox
    V2f textPos = {c->checkBox->x + c->checkBox->w + c->spaceWithText, c->checkBox->y + YCentering};

    renderText_Ex(sdl_ctx, temp_sprintf("%s", text), WHITE, textPos);
}
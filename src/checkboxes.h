#ifndef CHECKBOXES_H_
#define CHECKBOXES_H_

#include "common.h"

typedef struct checkbox {
    SDL_FRect *checkBox; // forme sur laquelle il va falloir cliquer pour mettre checked a true
    SDL_FRect *tickBox; // j'ai hésité a créer un FRect pour cela, mais c'est plus pratique, si c'est un symbole '✓' en image, comme ça il peut dépasser de la taille de la checkbox

    SDL_Texture *checkboxImg;
    SDL_Texture *tickImg;
    SDL_Texture *tickHoverImg;
    // image qui est une version 'fantôme' de tick_img, ou n'importe quoi d'autre, dans l'idée cela doit être transparent, cela s'affiche quand on survole la checkbox et qu'elle n'est pas checked, 
    // partage le même box que tickImg

    float spaceWithText; // espace entre le texte et le carré/rectangle a cocher
    float boxBorderSize; // ne sert que quand il n'y a pas d'image

    bool hovered;
    bool prevClicked;
    bool clicked;
    bool checked;
} checkbox;

bool createCheckbox(sdl_ctx_t *sdl_ctx, checkbox **c, SDL_FRect checkRect, SDL_FRect tickRect, char *bgImg_Path, char *tickImg_Path, char *tickHoverImg_Path, float spaceWithText, float boxBorderSize);
void destroyCheckbox(checkbox **c);
void updateCheckboxStates(checkbox *c, V2f mouseCoord, int mouseFlag);
void renderCheckbox(sdl_ctx_t *sdl_ctx, checkbox *c, char *text);

#endif // CHECKBOXES_H_
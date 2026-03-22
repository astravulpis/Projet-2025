#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "common.h"

typedef struct button{
    //je vais sans doute ajouter la possibilité de mettre une texture d'arrière plan
    char *buttonText;
    SDL_FRect *buttonBox;
    SDL_Color *baseColor;
    SDL_Color *hoverColor;
    SDL_Color *clickColor;
    bool hovered;
    bool leftClicked;
    bool rightClicked;
}button;

button *initButton(SDL_FRect *b_box, char *b_text, SDL_Color *b_baseC, SDL_Color *b_hoverC, SDL_Color *b_clickC);
void destroyButton(button **b);
void updateButtonState(button *b, SDL_FPoint mouseCoord, int mouseFlag);
void buttonRender(sdl_ctx_t *sdl_ctx, button *b);

#endif // BUTTONS_H_
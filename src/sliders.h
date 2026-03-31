/**
 * @file sliders.h
 * @brief File to define what a text input box is and how it works
 *
 * Author: Rossignol François <francois_rossignol@outlook.fr>
 * Last Modified: 2026-03-30
 * Date: 2026-03-30
 *
 * * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#ifndef SLIDERS_H_
#define SLIDERS_H_

#include "common.h"

typedef struct slider {
    SDL_FRect *sliderBox;
    SDL_FRect *cursorBox; // calculé de manière automatique, par rapport a sliderBox

    // ces 2 images peuvent être a NULL et la fonction de rendu affichera des rectangles a la place
    SDL_Texture *backgroundImg;
    SDL_Texture *cursorImg;

    int nbValue; // pour avoir le bon ratio dans la jauge
    float borderSize; // n'augmente pas la taille des bordures, cela réduit la taille du contenu du slider -> donc les bordure prennent plus de place
    int currentValue;
    float prevX;

    bool hovered;
    bool clicked;
    bool focused; // booléen a true quand un clic est maintenu sur le cursorBox sur plusieurs frames consécutive
} slider;

bool createSlider(sdl_ctx_t *sdl_ctx, slider **s, SDL_FRect rect, char *bgImg_Path, char *cursorImg_Path, float nbValue,
                  float borderSize);
void destroySliders(slider **s);
void updateSliderStates(slider *s, V2f mouseCoord, int mouseFlag, sdl_ctx_t *sdl_ctx);
void renderSlider(sdl_ctx_t *sdl_ctx, slider *s);

#endif // SLIDERS_H_

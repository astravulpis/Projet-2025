/**
 * @file bar.h
 * @brief File to define what a bar is and how it works
 *
 * Author: Rossignol François <francois_rossignol@outlook.fr>
 * Last Modified: 2026-03-28
 * Date: 2026-03-28
 *
 * * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#ifndef BAR_H_
#define BAR_H_

#include "common.h"

typedef struct bar {
    SDL_FRect *BarBox;
    SDL_Color bgColor;
    SDL_Color fillColor;
    SDL_Color cursorColor;

    float maxHp;      // pour avoir le bon ratio dans la jauge
    float barPadding; // proportion que la jauge prend dans BarBox
} bar;

bool createBar(bar **h, SDL_FRect rect, SDL_Color bgColor, SDL_Color fillColor, SDL_Color cursorColor, float maxHp,
                     float barPadding);
void destroyBar(bar **h);

void barRender(sdl_ctx_t *sdl_ctx, bar *h, float hpValue, int s_intensity, int l_intensity, int ls_opacity);

#endif // BAR_H_

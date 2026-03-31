/**
 * @file health_bar.h
 * @brief File to define what a health bar is and how it works
 *
 * Author: Rossignol François <francois_rossignol@outlook.fr>
 * Last Modified: 2026-03-28
 * Date: 2026-03-28
 *
 * * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#ifndef HEALTH_BAR_H_
#define HEALTH_BAR_H_

#include "common.h"

typedef struct health_bar {
    SDL_FRect *healthBarBox;
    SDL_Color bgColor;
    SDL_Color fillColor;
    SDL_Color cursorColor;

    float maxHp;      // pour avoir le bon ratio dans la jauge
    float barPadding; // proportion que la jauge prend dans healthBarBox
} health_bar;

bool createHealthBar(health_bar **h, SDL_FRect rect, SDL_Color bgColor, SDL_Color fillColor, SDL_Color cursorColor, float maxHp,
                     float barPadding);
void destroyHealthBar(health_bar **h);

void healthBarRender(sdl_ctx_t *sdl_ctx, health_bar *h, float hpValue, int s_intensity, int l_intensity, int ls_opacity);

#endif // HEALTH_BAR_H_

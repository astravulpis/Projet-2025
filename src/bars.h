/**
 * @file bar.h
 * @brief File to define what a bar is and how it works
 *
 * Author: Rossignol François <francois_rossignol@outlook.fr>
 * Last Modified: 2026-03-28
 *
 * * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 * Liam B. <liam.berge72@gmail.com>
 **/

#ifndef BAR_H_
#define BAR_H_

#include "common.h"

/**
 * @struct bar
 * @brief Act as our sdl context with multiple variables used about everywhere
 *
 * It holds multiple variable for the rendering of the window, the surface of
 * said window, the event that can be pulled and if it's running or not
 */
typedef struct bar {
    SDL_FRect *BarBox;
    SDL_Color bgColor;
    SDL_Color fillColor;
    SDL_Color cursorColor;

    float val;      // pour avoir le bon ratio dans la jauge
    float barPadding; // proportion que la jauge prend dans BarBox
    float minCursorWidth;

    bool displayHpValue; // booléen qui permet d'indiquer si l'on doit afficher hpValue au rendu ou pas
} bar;

bool createBar(bar **h, SDL_FRect rect, SDL_Color bgColor, SDL_Color fillColor, SDL_Color cursorColor, float baseVal,
               float barPadding, float minCursorWidth, bool displayHpValue);
void barRender(sdl_ctx_t *sdl_ctx, bar *h, float hpValue, int s_intensity, int l_intensity, int ls_opacity);
void destroyBar(bar **h);

#endif // BAR_H_

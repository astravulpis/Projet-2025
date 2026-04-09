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
 * @brief a type with attributes used to display information such as the player's health or stamina
 *
 * a structure that includes all the features of a bar,
 * a bar represents a value via a gauge on a bar relative to a maximum
 */
typedef struct bar {
    SDL_FRect *BarBox; //!< represents the rectangle that the bar occupies in the game window, including the borders
    SDL_Color bgColor; //!< represents the color of the gauge borders (borders are invisible if barPadding == 0)
    SDL_Color fillColor; //!< represents the gauge color
    SDL_Color cursorColor; //!< represents the cursor color

    float val;      // pour avoir le bon ratio dans la jauge
    float barPadding; // proportion que la jauge prend dans BarBox
    float minCursorWidth;

    bool displayHpValue; // booléen qui permet d'indiquer si l'on doit afficher hpValue au rendu ou pas
} bar;

/**
 * @fn createBar(bar **h, SDL_FRect rect, SDL_Color bgColor, SDL_Color fillColor, SDL_Color cursorColor, float baseVal, float barPadding)
 * @param h a pointer to a bar pointer, which will hold the bar created when the function is called
 * @param rect the rectangular area where the bar will be located
 * @param bgColor the border color of the future bar
 * @param fillColor the gauge color of the future bar
 * @param baseVal the maximum value of the future bar
 * @param barPadding determines the proportion of the bar within the bar's rectangle and also the shadows/lights effects
 * @return returns nothing; to check if everything went well, you need to check if h == NULL after calling this function
 * @brief allocates memory for a bar and initializes its attributes
 *
 * allocates memory for a bar and initializes its attributes : rect, bgColor, fillColor, baseVal, barPadding with parameters
 */
bool createBar(bar **h, SDL_FRect rect, SDL_Color bgColor, SDL_Color fillColor, SDL_Color cursorColor, float baseVal,
               float barPadding, float minCursorWidth, bool displayHpValue);

/**
 * @fn barRender(sdl_ctx_t *sdl_ctx, bar *h, float hpValue, int s_intensity, int l_intensity, int ls_opacity)
 * @param sdl_ctx is set as a function parameter to access the renderer, and if the option is enabled, to display the value as text in addition to the gauge (to access the font used there)
 * @param h the bar that will be displayed in the game window
 * @param hpValue the value of the bar at the time of display
 * @param s_intensity shadow intensity; the higher the value, the darker the shadows
 * @param l_intensity light intensity; the higher the value, the brighter the light
 * @param ls_opacity  transparency of light and shadow effects
 * @brief returns a bar based on these attributes, in the area that was assigned when the bar was created
 */
void barRender(sdl_ctx_t *sdl_ctx, bar *h, float hpValue, int s_intensity, int l_intensity, int ls_opacity);

/**
 * @fn destroyBar(bar **h)
 * @param h a pointer to a pointer on a bar that is about to be destroyed (along with all its attributes) and set to NULL
 * @brief deallocates all the attributes of a bar that were manually allocated, along with the bar itself, and then sets them to NULL for safety
 */
void destroyBar(bar **h);

#endif // BAR_H_

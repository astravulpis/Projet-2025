/**
 * @file bar.h
 * @brief File to define what a bar is and how it works
 *
 * @author Rossignol François <francois_rossignol@outlook.fr>
 * @date 2026-03-31
 * @remark last modified 2026-02-07
 *
 * Contributors:
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
    SDL_FRect *BarBox; //!< represents the rectangle that the bar occupies in the game window, including the borders (position and dimensions)
    SDL_Color bgColor; //!< represents the color of the gauge borders (borders are invisible if barPadding == 0)
    SDL_Color fillColor; //!< represents the gauge color
    SDL_Color cursorColor; //!< represents the cursor color

    float val;      //!< maximum gauge reading on the display (regardless of the input value to be displayed)
    float barPadding; //!< the proportion of the gauge within \ref BarBox
    float minCursorWidth; //!< a minimum cursor size limit

    bool displayHpValue; //!< A Boolean that specifies whether to display \ref hpValue in the rendering (as text in addition to the gauge)
} bar;

/**
 * @fn createBar(bar **h, SDL_FRect rect, SDL_Color bgColor, SDL_Color fillColor, SDL_Color cursorColor, float baseVal, float barPadding, float minCursorWidth, bool displayHpValue)
 * @param h a pointer to a \ref bar pointer, which will hold the \ref bar created when the function is called
 * @param rect the rectangular area where the bar will be located (position and dimmensions)
 * @param bgColor the border color of the future \ref bar
 * @param fillColor the gauge color of the future \ref bar
 * @param cursorColor the cursor color of the future \ref bar
 * @param baseVal the maximum value of the future bar
 * @param barPadding determines the proportion of the \ref bar within the \ref bar's rectangle and also the shadows/lights effects
 * @param minCursorWidth allows you to set a minimum cursor size so you can see the gauge more clearly
 * @param displayHpValue set \ref displayHpValue (true or false)
 * @return returns a boolean value: true if everything went well, false otherwise
 * @brief allocates memory for a bar and initializes its attributes
 *
 * allocates memory for a bar and initializes its attributes : \ref BarBox, \ref bgColor, \ref fillColor, \ref cursorColor, \ref val, \ref barPadding, \ref minCursorWidth and \ref displayHpValue with parameters
 */
bool createBar(bar **h, SDL_FRect rect, SDL_Color bgColor, SDL_Color fillColor, SDL_Color cursorColor, float baseVal,
               float barPadding, float minCursorWidth, bool displayHpValue);

/**
 * @fn barRender(sdl_ctx_t *sdl_ctx, bar *h, float hpValue, int s_intensity, int l_intensity, int ls_opacity)
 * @param sdl_ctx is set as a function parameter to access the renderer, and if the option is enabled, to display the value as text in addition to the gauge (to access the font used there)
 * @param h the \ref bar that will be displayed in the game window
 * @param hpValue the value of the \ref bar at the time of display
 * @param s_intensity shadow intensity; the higher the value, the darker the shadows
 * @param l_intensity light intensity; the higher the value, the brighter the light
 * @param ls_opacity  transparency of light and shadow effects
 * @brief returns a bar based on these attributes, in the area that was assigned when the bar was created with \ref createBar
 */
void barRender(sdl_ctx_t *sdl_ctx, bar *h, float hpValue, int s_intensity, int l_intensity, int ls_opacity);

/**
 * @fn destroyBar(bar **h)
 * @param h a pointer to a pointer on a \ref bar that is about to be destroyed (along with all its attributes) and set to NULL
 * @brief deallocates all the attributes of a bar that were manually allocated, along with the bar itself, and then sets them to NULL for safety
 */
void destroyBar(bar **h);

#endif // BAR_H_

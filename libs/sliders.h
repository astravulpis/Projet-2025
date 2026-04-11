/**
 * @file sliders.h
 * @brief File to define what a text input box is and how it works
 *
 * @author Rossignol François <francois_rossignol@outlook.fr>
 * @date 2026-03-30
 * @remark last Modified: 2026-03-07
 *
 * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 * Liam B. <liam.berge72@gmail.com>
 **/

#ifndef SLIDERS_H_
#define SLIDERS_H_

#include "common.h"

/**
 * @struct slider
 * @brief a type that contains all the attributes needed to display a slider and handle its interactions (hover, click, move the cursor)
 *
 * A slider is a bar (in this case, horizontal) on which you can move a cursor; the cursor's position indicates a value.
 * It is, in a way, similar to a \ref checkbox, but it can have more than two different states, and the logic behind it is more complex.
 */
typedef struct slider {
    SDL_FRect *sliderBox; //!< the area of the vertical bar where the cursor can move (coordinates and dimensions)
    SDL_FRect *cursorBox; //!< the slider area: its position and size are calculated relative to \ref sliderBox and \ref borderSize: if it is equal to 0, the slider will have the same height as \ref sliderBox and a width of \ref sliderBox / \ref nbValue; otherwise, the slider will be scaled down relative to \ref borderSize

    char *name; //!< the name of the slider; it's useful because it helps distinguish between the different bars and lets you know, based on their names, what information they represent

    int nbValue; //!< The value used to calculate the step size by which the cursor should move, as well as its maximum value (the right-hand limit; the left-hand limit is set to 0). This value is also used to calculate \ref currentValue
    float borderSize; //!< allows you to control the size of the borders—or at least their proportions—since \ref sliderBox won't change; instead, it affects what happens inside
    int currentValue; //!< the value of the slider that corresponds to the cursor's current visual position
    float prevX; //!< the previous X coordinate of the mouse when \ref cursorBox was \ref clicked

    bool hovered; //!< a boolean that is true if the mouse hovers over the \ref cursorBox
    bool clicked; //!< a boolean value that is currently true, indicating that the \ref updateSliderStates function has detected a click on the \ref cursorBox (right-click or left-click)
    bool focused; //!< a boolean that indicates whether a click is held down across multiple frames on \ref cursorBox
} slider;


/**
 * @fn createSlider(slider **s, SDL_FRect rect, float nbValue, float borderSize, float baseVal, const char *name)
 * @param s a pointer to a \ref slider pointer, which will hold the \ref slider created when the function is called
 * @param rect the rectangular area where the \ref slider will be located (the \ref sliderBox SDL_FRect)
 * @param nbValue set the \ref nbValue of \ref s
 * @param borderSize set the \ref borderSize of \ref s
 * @param baseVal sets the x-coordinate of the cursor, \ref currentValue
 * @param name the string var copied to \ref name of \ref s
 * @return returns a boolean value: true if everything went well, false otherwise
 * @brief allocates memory for a \ref slider, initializes its attributes
 *
 * allocates memory for a \ref slider, initializes its attributes, \ref sliderBox, \ref cursorBox, \ref name, \ref name, \ref nbValue ... ( \ref slider)
 * \ref cursorBox has dimensions equal to those of \ref sliderBox minus \ref borderSize (for the width, this result is divided by \ref nbValue);
 * x and y are equal to those of \ref sliderBox plus \ref borderSize
 * \ref prevX is set to -1 and all booleans to false
 * for \ref sliderBox and \ref cursorBox, \ref renderRect_Ex is used
 */
bool createSlider(slider **s, SDL_FRect rect, float nbValue, float borderSize, float baseVal, const char *name);

/**
 * @fn destroySlider(slider **s)
 * @param s a pointer to a \ref slider pointer, which is going to be destroyed
 * @brief deletes a \ref slider and its properties
 *
 * deletes a \ref slider and its properties with free, there's no specific function called here
 */   
void destroySlider(slider **s);

/**
 * @fn updateSliderStates(slider *s, V2f mouseCoord, int mouseFlag, float *val)
 * @param s s is a pointer to the \ref slider whose states (Booleans) we want to update
 * @param mouseCoord mouse pointer coordinates in the game window ( \ref V2f)
 * @param mouseFlag Boolean mask to determine which mouse buttons are pressed
 * @param val which will contain the same value as \ref currentValue; this is useful for retrieving the value outside the structure
 * @brief This function updates the status booleans of a \ref slider
 *
 * This function updates the status booleans ( \ref hovered, \ref clicked and \ref focused) of a \ref slider
 * using SDL_PointInRectFloat with \ref mouseCoord on \ref cursorBox for \ref hovered attribute
 * after mouseFlag is used for clicked or focused
 * To set the \ref focused boolean to true, a click must be held down across multiple frames.
 * To detect this, we compare \ref clicked (when it hasn't been updated yet) with \ref mouseFlag; if both are true, then \ref focused becomes true. 
 * else, \ref clicked become true, but not focused
 *
 * To move the cursor, a difference is calculated between \ref prevX and the current mouse position (provided, of course, that \ref is focused).
 * If the absolute value (fabsf(), \ref math.h) of this difference is greater than or equal to the step size (the width of a value displayed on the slider), the difference is divided by the step size, 
 * the result is rounded (roundf(), \ref math.h), and then multiplied by the step size. \ref cursorBox will be set to this value to move along the X-axis, \ref currentValue will be set a the same method.
 */
void updateSliderStates(slider *s, V2f mouseCoord, int mouseFlag, float *val);

/**
 * @fn renderSlider(sdl_ctx_t *sdl_ctx, slider *s)
 * @param sdl_ctx SDL context, passed as a parameter to the function here to have acces to the renderer where the \ref slider will be rendered
 * @param s s is a pointer to the \ref slider to print
 * @brief This function displays a \ref slider based on its boolean state 
 *
 * This function displays a \ref slider based on its boolean state, with \ref renderRect_Ex for \ref sliderBox and \ref cursorBox
 * the cursor is rendered a the position updated before with \ref updateSliderStates, and with a darker color if it's clicked (there's a little shadow effect a the top of it)
 */
void renderSlider(sdl_ctx_t *sdl_ctx, slider *s);

/**
 * @fn FRect_Change_x(SDL_FRect *rect, float step, float minLimit, float maxLimit)
 * @param rect a pointer to an \ref SDL_FRect
 * @param step increment/decrement value
 * @param minLimit a minimum limit of rect.x decrement
 * @param maxLimit a maximum limit of rect.x increment
 * @brief this function is similar to clamp in CSS
 *
 * this function is similar to clamp in CSS, It allows you to change the X value of an SDL_FRect to fall within a specified interval;
 * if the sum of \step and the X value of \ref rect falls outside the interval, then it snaps to the nearest boundary ( \ref minLimit or \ref maxLimit)
 */
void FRect_Change_x(SDL_FRect *rect, float step, float minLimit, float maxLimit);

#endif // SLIDERS_H_

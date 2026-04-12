/**
 * @file button.h
 * @brief File to define what a button is and how it works
 *
 * @author Rossignol François <francois_rossignol@outlook.fr>
 * @date 2026-03-19
 * @remark last modified 2026-03-31 (added image support)
 *
 * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "common.h"

/**
 * @struct button
 * @brief a type that contains all the attributes needed to display a button and handle its interactions (click and hover)
 */
typedef struct button {
    char *buttonText; //!< text displayed when the button is rendered
    SDL_FRect *buttonBox; //!< the button's collision area, representing its coordinates and dimensions in the game window
    SDL_Texture *baseImg; //!< the image that appears by default on the button
    SDL_Texture *hoverImg; //!< the image displayed when the mouse hovers over the button
    SDL_Texture *clickImg; //!< the image displayed when the button is hovered and clicked
    bool isHovered; //!< A Boolean value updated by the call to the \ref updateButtonState function, which indicates whether the mouse is hovering over the button
    bool isLeftClicked; //!< A Boolean variable updated by the call to the \ref updateButtonState function, which indicates whether the button has been left-clicked
    bool isRightClicked; //!< A Boolean variable updated by the call to the \ref updateButtonState function, which indicates whether the button has been right-clicked
} button;


/**
 * @fn createButton(sdl_ctx_t *sdl_ctx, button **b, const char *text, SDL_FRect rect, char *baseImgPath, char *hoverImgPath, char *clickImgPath)
 * @param sdl_ctx SDL context, passed as a parameter to the function here to load the various \ref button images @param text the text that will appear next to the \ref button @param b a pointer to a \ref button pointer, which will hold the \ref button created when the function is called
 * @param rect the rectangular area where the \ref button will be located
 * @param baseImgPath the path to the base image that needs to be loaded
 * @param hoverImgPath the path to the hovered image that needs to be loaded
 * @param clickImgPath the path to the clicked image that needs to be loaded
 * @return returns a boolean value: true if everything went well, false otherwise
 * @brief allocates memory for a \ref button, initializes its attributes
 *
 * allocates memory for a \ref button, initializes its attributes :
 * \ref buttonBox, \ref createRect_Ex is used with rect
 * The images ( \ref baseImg, \ref hoverImg, \ref clickImg) are loaded along with their paths using the IMG_LoadTexture() function
 * The boolean are set to false
 */
bool createButton(sdl_ctx_t *sdl_ctx, button **b, const char *text, SDL_FRect rect, char *baseImgPath, char *hoverImgPath,
                  char *clickImgPath);

/**
 * @fn updateButtonState(button *b, mouseDevice *mouse)
 * @param b b is a pointer to the \ref button whose states (Booleans) we want to update
 * @param mouse The mouse device used to store the mouse's info ( \ref mouseDevice)
 * @brief This function updates the status booleans of a \ref button
 *
 * This function updates the status booleans ( \ref isHovered, \ref isLeftClicked and \ref isRightClicked) of a \ref button using SDL_PointInRectFloat with mouseCoord
 * after mouseFlag is compare with SDL MASK to get the mouse input
 */
void updateButtonState(button *b, mouseDevice *mouse);

/**
 * @fn renderButton(sdl_ctx_t *sdl_ctx, button *b)
 * @param sdl_ctx SDL context, passed as a parameter to the function here to have acces to the renderer where the \ref button will be rendered
 * @param b b is a pointer to the \ref button to print
 * @brief This function displays a \ref button based on its boolean state
 *
 * This function displays a \ref button based on its boolean state, with \ref renderImage() and \ref renderText_Ex()
 * If there was a problem loading the image, a rectangle is displayed in its place using \ref renderFillRect(), different colors for each state
 */
void renderButton(sdl_ctx_t *sdl_ctx, button *b);

/**
 * @fn destroyButton(button **b)
 * @param b a pointer to a \ref button pointer, which is going to be destroyed
 * @brief deletes a \ref button and its properties
 *
 * deletes a button and its properties with SDL_DestroyTexture() for each images (base, hover and click)
 * free up the previously allocated space, and set to NULL *\ref button.
 */
void destroyButton(button **b);

#endif // BUTTONS_H_

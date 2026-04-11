/**
 * @file checkboxes.h
 * @brief File to define what a checkbox is and how it works
 *
 * @author Rossignol François <francois_rossignol@outlook.fr>
 * @date 2026-03-31
 * @remark last modified 2026-03-31
 *
 * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#ifndef CHECKBOXES_H_
#define CHECKBOXES_H_

#include "common.h"

/**
 * @struct checkbox
 * @brief a type that contains all the attributes needed to display a checkbox and handle its interactions (click, hover, checked)
 *
 * a checkbox is a two state element that you can check or uncheck
 */
typedef struct checkbox {
    SDL_FRect *checkBox; //!< a rectangle that represents the position and dimensions of the rectangle or square that you need to click to check or uncheck a checkbox
    SDL_FRect *tickBox; //!< Area (position and dimensions) where the checkmark will appear: if it is a ‘✓’ icon, it may extend beyond the checkbox's borders

    SDL_Texture *checkboxImg; //!< the background image of the checkbox (in the \ref checkBox area)
    SDL_Texture *tickImg; //!< the image of the checkbox tick (in the \ref tickBox area)
    SDL_Texture *tickHoverImg; //!< a transparent version of the tick icon (it could be something else, but that's what it was designed for) from the checkbox (in the \ref tickBox area as well), displayed when the mouse hovers over the checkbox if it is not \ref checked

    float spaceWithText; //!< space between the text and the checkbox (with a ratio already set upon creation)
    float boxBorderSize; //!< Useful for displaying a border around a checkbox when the image could not be loaded or set

    bool hovered; //!< a boolean that is true if the mouse hovers over the \ref checkBox
    bool prevClicked; //!< a boolean value that is true if the \ref checkBox was clicked during the previous iteration of the \ref updateCheckbox function (right-click or left-click)
    bool clicked; //!< a Boolean value that is currently true, indicating that the \ref updateCheckbox function has detected a click on the \ref checkBox (right-click or left-click)
    bool checked; //!< a Boolean value indicating whether the checkbox is checked or not
} checkbox;


//note a moi même : rien n'empĉhe le tick box d'être placée a un endroit complètement différent de la checkBox ...

/**
 * @fn createCheckbox(sdl_ctx_t *sdl_ctx, checkbox **c, SDL_FRect checkRect, SDL_FRect tickRect, char *bgImg_Path, char *tickImg_Path, char *tickHoverImg_Path, float spaceWithText, float boxBorderSize)
 * @param sdl_ctx SDL context, passed as a parameter to the function here to load the various \ref checkbox images
 * @param c a pointer to a \ref checkbox pointer, which will hold the \ref checkbox created when the function is called
 * @param checkRect the rectangular area where the \ref checkbox will be located
 * @param tickRect the rectangular area where the tick will be located (As I'm reviewing the documentation, I realize that nothing has been done to prevent it from being placed anywhere—unlike the \ref checkbox)
 * @param bgImg_Path the path to the background image that needs to be loaded (in \ref checkboxImg)
 * @param tickImg_Path the path to the \ref tickImg image that needs to be loaded (in \ref tickImg)
 * @param tickHoverImg_Path the path to the \ref tickHoverImg image that needs to be loaded (in \ref tickHoverImg)
 * @param spaceWithText Used to initialize the \ref spaceWithText attribute; the text in this component does not have a predefined area, only a space with the \ref checkbox
 * @param boxBorderSize a useful option if there is no image, to ensure the \ref checkbox is clearly visible in that case
 * @return returns a boolean value: true if everything went well, false otherwise
 * @brief allocates memory for a \ref checkbox, initializes its attributes (rect, images and boolean  ...)
 *
 * allocates memory for a checkbox, initializes its attributes :
 * \ref checkBox, \ref createRect_Ex is used with checkRect
 * \ref tickBox, \ref createRect_Ex is used with tickRect
 * The images ( \ref checkboxImg, \ref tickImg, \ref tickHoverImg) are loaded along with their paths using the IMG_LoadTexture() function
 * The boolean are set to false by default
 */
bool createCheckbox(sdl_ctx_t *sdl_ctx, checkbox **c, SDL_FRect checkRect, SDL_FRect tickRect, char *bgImg_Path,
                    char *tickImg_Path, char *tickHoverImg_Path, float spaceWithText, float boxBorderSize);

/**
 * @fn destroyCheckbox(checkbox **c)
 * @param c a pointer to a \ref checkbox pointer, which is going to be destroyed
 * @brief deletes a \ref checkbox and its properties
 *
 * deletes a \ref checkbox and its properties with SDL_DestroyTexture() for each images ( \ref checkboxImg, \ref tickImg and \ref tickHoverImg)
 * free up the previously allocated space, and set to NULL *\ref checkbox.
 */                 
void destroyCheckbox(checkbox **c);

/**
 * @fn updateCheckboxStates(checkbox *c, V2f mouseCoord, int mouseFlag)
 * @param c c is a pointer to the \ref checkbox whose states (Booleans) we want to update
 * @param mouseCoord mouse pointer coordinates in the game window ( \ref V2f)
 * @param mouseFlag Boolean mask to determine which mouse buttons are pressed
 * @brief This function updates the status booleans of a \ref checkbox
 *
 * This function updates the status booleans ( \ref hovered, \ref prevClicked and \ref clicked and \ref checked) of a \ref checkbox using SDL_PointInRectFloat with mouseCoord
 * after mouseFlag is compare with SDL MASK to get the mouse input
 * To toggle the \ref checked state, we detect when the mouse checkbox is released, 
 * not when it's \ref clicked, because a frame is so short that by the next frame, the checkbox would still be clicked, so timing would be a bit of a gamble.
 * Hence the usefulness of ` \ref prevClicked`, which lets you know if the click occurred on the previous frame
 */
void updateCheckboxStates(checkbox *c, V2f mouseCoord, int mouseFlag);

/**
 * @fn renderCheckbox(sdl_ctx_t *sdl_ctx, checkbox *c, char *text)
 * @param sdl_ctx SDL context, passed as a parameter to the function here to have acces to the renderer where the \ref checkbox will be rendered
 * @param c c is a pointer to the \ref checkbox to print
 * @param text the text displayed to the right of the \ref checkbox (to the left if \ref spaceWithText was set to a negative value during \ref createCheckbox
 * @brief This function displays a \ref checkbox based on its boolean state 
 *
 * This function displays a \ref checkbox based on its boolean state, with \ref renderImage() and \ref renderText_Ex()
 * If there was a problem loading the image, there is a fallback with rectangles are displayed in their place using \ref renderFillRect(), different colors for each state
 */
void renderCheckbox(sdl_ctx_t *sdl_ctx, checkbox *c, char *text);

#endif // CHECKBOXES_H_
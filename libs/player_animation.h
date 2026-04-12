/**
 * @file player_animation.h
 * @brief File to define what a player animation is and how it works
 *
 * @author Rossignol François <francois_rossignol@outlook.fr>
 * @date 2026-04-08
 * @remark last Modified: 2026-04-09
 *
 * Contributors:
 * Rossignol François <francois_rossignol@outlook.fr>
 * Liam B. <liam.berge72@gmail.com>
 **/

#ifndef PLAYER_ANIMATION_H_
#define PLAYER_ANIMATION_H_

#include "common.h"

/**
 * \def ANIMATION_NB_FRAME
 * \brief number of images in an animation const
 */
#define ANIMATION_NB_FRAME 5

/**
 * @struct player_animation
 * @brief a type that contains all the attributes needed to display a \ref player_animation within a certain period of time
 *
 * A \ref player_animation is a structure that contains an array of \ref ANIMATION_NB_FRAME images (in this game, there are 5 images),
 * through which the images are cycled at a specified interval ( \ref loopInterval, in ms).
 */
typedef struct {
    SDL_Texture *animationTab[ANIMATION_NB_FRAME]; //!< table containing the image \ref ANIMATION_NB_FRAME (5 here).

    int currentAnimationIndex; //!< An integer indicating which frame of the \ref animationTab array the animation has reached (which frame to display); when this value is -1, it means the animation has not been played before (so it starts at the first frame)
    float cumulateTime; //!< A timer that increments until its duration exceeds that of \ref loopInterval; this timer is used to update \ref currentAnimationIndex (equal to \ref loopInterval divided by 5, rounded, in \ref cumulateTime) with \ref imgPerMs
    float imgPerMs; //!< indicates the number of frames displayed per millisecond (calculated at initialization using \ref loopInterval / \ref ANIMATION_NB_FRAME), used to calculate \ref currentAnimationIndex: (current time - \ref cumulativeTime) / \ref framesPerMs
    float loopInterval; //!< Time interval in milliseconds during which the animation repeats: 1000 ms = 5 fps, 500 ms = 10 fps, etc.
} player_animation;

// bool createPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation **pa, SDL_FRect paRect, char *frame0Path, char *frame1Path,
// char *frame2Path, char *frame3Path, char *frame4Path);

/**
 * @fn createPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation **pa, char *framesPath, float loopInterval)
 * @param sdl_ctx SDL context, used for load images ( \ref renderer)
 * @param pa pa is a pointer to a \ref player_animation pointer, which will hold the \ref player_animation created when the function is called
 * @param framesPath the path to the directory containing the animation images to be loaded; each image must be named as follows: [image number].png to be loaded
 * @param loopInterval set the \ref loopInterval, ImgPerMs will be calculated with him
 * @return returns a boolean value: true if everything went well, false otherwise
 * @brief allocates memory for a \ref player_animation, initializes its attributes with parameters
 *
 * allocates memory for a \ref player_animation, initializes its attributes, \ref animationTab, \ref currentAnimationIndex, \ref cumulateTime,
 * \ref imgPerMs, \ref loopInterval (see \ref player_animation).
 * All images are loaded from the \framesPath directory using IMG_LoadTexture in a loop,
 * which is why there are strict naming conventions for these images (one directory for IDLE, RUN, etc., each containing 5 images: 1.png, 2.png, ..., 5.png)
 * \ref cumulateTime is set to -1, \ref currentAnimationIndex to 0 and \ref imgPerMs is calculated
 */
bool createPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation **pa, char *framesPath, float loopInterval);

/**
 * @fn destroyPlayerAnimation(player_animation **pa)
 * @param pa a pointer to a \ref player_animation pointer, which is going to be destroyed
 * @brief deletes a \ref player_animation and its properties
 *
 * deletes a \ref player_animation and its properties with free, SDL_DestroyTexture is used to delete the 5 images contained in \ref framesPath using a loop
 */
void destroyPlayerAnimation(player_animation **pa);

/**
 * @fn resetPlayerAnimationState(player_animation *pa)
 * @param pa a pointer to the animation we want to reset
 * @brief This function resets \ref currentAnimationIndex to 0 and \ref cumulateTime to -1;
 *
 * This function resets \ref currentAnimationIndex to 0 and \ref cumulateTime to -1; it is used when a different animation is played. For example,
 * if the player is running and jumps, the running animation is reset because the jumping animation needs to be displayed. 
 * The next time the player runs, the animation will start over from the beginning. 
 */
void resetPlayerAnimationState(player_animation *pa);


// void renderPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation *pa, SDL_FlipMode flipped, float deg);



/**
 * @fn renderPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation *pa, SDL_FlipMode flipped, float deg, SDL_FRect *animationBox)
 * @param sdl_ctx SDL context, passed as a parameter to the function here to have acces to the renderer where the \ref player_animation will be rendered
 * @param pa pa is a pointer to the \ref player_animation to print
 * @param flipped is a SDL_FlipMode, here, it is used to flip the image horizontally based on its orientation (left or right) in the render
 * @param deg deg is a rotation value applied to the animation (in this game, it is always set to 0, since there is never a need to change it)
 * @param animationBox is a pointer to \ref SDL_FRect, which contains the coordinates and display dimensions of the animation (the \ref player area for example)
 * @brief This function displays a \ref player_animation
 *
 * This function displays a \ref player_animation based on \ref currentAnimationIndex using \ref imgPerMs, \ref cumulateTime, and the current time (SDL_GetTicks is used for this).
 * Here, the rendering is not separated from the update, because the animation does not depend on user input but on time ( \ref loopInterval).
 * The rendering area is specified as a parameter using \ref animationBox; the rendering function used is \ref SDL_RenderTextureRotated, as it supports the use of \ref deg and \ref flipped
 * Brief explanation of the algorithm: First, we check \ref cumulateTime; if it is not equal to -1, we check how long it has been since it was initialized. If it has been longer than \ref loopInterval,
 * then \ref currentAnimationIndex is reset to 0 and \ref cumulateTime is updated to the current time. If it is not equal to -1, then \ref currentAnimationIndex is calculated using this formula:
 * fminf(roundf((currentTick - \ref cumulateTime) / \ref imgPerMs), ANIMATION_NB_FRAME - 1), where fminf returns the minimum between two floats and roundf rounds the floats (\ref math.h).
 * there's not fallback monochrome rectangle if image loading went wrong.
 */
void renderPlayerAnimation(sdl_ctx_t *sdl_ctx, player_animation *pa, SDL_FlipMode flipped, float deg, SDL_FRect *animationBox);

#endif // PLAYER_ANIMATION_H_

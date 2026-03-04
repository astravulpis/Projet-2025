#ifndef SDL_HELPER_H_
#define SDL_HELPER_H_

#include "SDL3/SDL.h"
#include "common.h"
#include <assert.h>

/**
 * @fn init_all()
 * @brief Initializes the sdl context
 * @param[out] ctx Returns our SDL context
 */
sdl_ctx_t *init_all(void);

/**
 * @fn close_SDL()
 * @param[in] sdl_ctx Our SDL context
 * @brief Destroys the SDL window
 *
 * Helper function to deallocate and put all of the SDL context pointers to NULL.
 */
void close_SDL(sdl_ctx_t *sdl_ctx);

/**
 * @fn initBackgroundColor(int r, int g, int b)
 * @brief helper function to initialise the background
 * @param[in] sdl_ctx Our SDL context
 * @param[in] r red color of the background
 * @param[in] g green color of the background
 * @param[in] b blue color of the background
 *
 * Init sdl context's backgroud surface and texture and fills them with the color given in RGBA format
 */
void initBackgroundColor(sdl_ctx_t *sdl_ctx, int r, int g, int b);

/**
 * @fn updateBackgroundColor(int r, int g, int b)
 * @brief helper function to modify the color of the background
 * @param[in] sdl_ctx Our SDL context
 * @param[in] r red color of the background
 * @param[in] g green color of the background
 * @param[in] b blue color of the background
 *
 * Modifies the surface and the texture of the sdl context's window
 */
void updateBackgroundColor(sdl_ctx_t *sdl_ctx, int r, int g, int b);

/**
 * @fn renderBackground()
 * @param[in] sdl_ctx Our SDL context
 * @brief Renders the background texture if not NULL (?)
 */
void renderBackground(sdl_ctx_t *sdl_ctx);

SDL_FRect *createRect(float x, float y, float width, float height);
bool enableVsync(sdl_ctx_t *sdl_ctx);
bool disableVsync(sdl_ctx_t *sdl_ctx);
void clearContextSurface(sdl_ctx_t *sdl_ctx);

#endif //SDL_HELPER_H_

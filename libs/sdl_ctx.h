/**
 * @file sdl_ctx.h
 * @brief This where our SDL context is defined
 *
 * @author Liam B. <liam.berge72@gmail.com>
 * @date 2026-03-15
 * @remark last modified: 2026-03-18
 *
 * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#ifndef SDL_CTX_H_
#define SDL_CTX_H_

#include "common.h"

/**
 * @fn create_ctx(sdl_ctx_t **ctx)
 * @param[in]  ctx Our sdl context that's getting created
 * @param[out] result Returns true if creation of the context was successful, false otherwise.
 * @brief creates our SDL context
 */
bool createCtx(sdl_ctx_t **ctx);

/**
 * @fn initCtx(sdl_ctx_t *sdl_ctx)
 * @param[in] sdl_ctx Our sdl context
 * @param[out] result Returns true operations were a success, false otherwise.
 * @brief Initializes our sdl context
 */
bool initCtx(sdl_ctx_t *sdl_ctx);

/**
 * @fn closeCtx(sdl_ctx_t **sdl_ctx)
 * @param[in] sdl_ctx Our SDL context
 * @brief Destroys the SDL window
 *
 * Helper function to deallocate and put all of the SDL context pointers to NULL.
 */
void closeCtx(sdl_ctx_t **sdl_ctx);

/**
 * @fn enableVsync(sdl_ctx_t *sdl_ctx)
 * @param[in] sdl_ctx Our SDL context
 * @param[out] result if was successful, false otherwise
 * @brief Activate VSync
 */
bool enableVsync(sdl_ctx_t *sdl_ctx);

/**
 * @fn disableVsync(sdl_ctx_t *sdl_ctx)
 * @param[in] sdl_ctx Our SDL context
 * @param[out] result if was successful, false otherwise
 * @brief Deactivate VSync
 */
bool disableVsync(sdl_ctx_t *sdl_ctx);

/**
 * @fn clearContextSurface(sdl_ctx_t *sdl_ctx)
 * @param[in] sdl_ctx Our SDL context
 * @brief Clear the background surface
 */
void clearContextSurface(sdl_ctx_t *sdl_ctx);

/**
 * @fn loadBackgroundImage(sdl_ctx_t *sdl_ctx, char *path)
 * @param[in] sdl_ctx Our SDL context
 * @param[in] path path to the background image
 * @param[out] result if was successful, false otherwise
 * @brief calls IMG_LoadTexture to load the background image
 */
bool loadBackgroundImage(sdl_ctx_t *sdl_ctx, const char *path);

/**
 * @fn renderBackground(sdl_ctx_t *sdl_ctx);
 * @param[in] sdl_ctx Our SDL context
 * @brief Renders the background texture
 */
void renderBackground(sdl_ctx_t *sdl_ctx);

/**
 * @fn loadFont(char *path, float fontSize, int fontStyle, int outline)
 * @param[in] path to the font file
 * @param[in] fontSize litteraly just the font size
 * @param[in] fontStle litteraly just the font style
 * @param[in] outline litteraly the the font ouline
 * @brief Loads a font based on its path, font size, font styling and its outline if any
 */
TTF_Font *loadFont(char *path, float fontSize, int fontStyle, int outline);

/**
 * @fn setMasterTrackGain(sdl_ctx_t *ctx)
 * @param[in] ctx our context variable
 * @brief sets the mater track gain
 */
void setMasterTrackGain(sdl_ctx_t *ctx);

/**
 * @fn setMusicTrackGain(sdl_ctx_t *ctx)
 * @param[in] ctx our context variable
 * @brief sets the music track gain
 */
void setMusicTrackGain(sdl_ctx_t *ctx);

/**
 * @fn setSfxTrackGain(sdl_ctx_t *ctx)
 * @param[in] ctx our context variable
 * @brief sets the sound effets track gain
 */
void setSfxTrackGain(sdl_ctx_t *ctx);

/**
 * @fn initOptions(options *opts)
 * @param[in] opts our context variable
 * @brief sets the default sound volume
 */
void initOptions(options *opts);

#endif // SDL_CTX_H_

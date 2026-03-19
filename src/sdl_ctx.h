/**
 * @file sdl_ctx.h
 * @brief This where our SDL context is defined
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-03-15
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#ifndef SDL_CTX_H_
#define SDL_CTX_H_

#include "common.h"

/**
 * @fn create_ctx(sdl_ctx_t **ctx)
 * @brief Initializes our sdl context
 * @param[in]  ctx Our sdl context that's getting created
 * @param[out] result Returns true if creation of the context was successful, false otherwise.
 */
bool createCtx(sdl_ctx_t **ctx);

/**
 * @fn initCtx(sdl_ctx_t *sdl_ctx)
 * @brief Initializes our sdl context
 * @param[in] sdl_ctx Our sdl context
 * @param[out] result Returns true operations were a success, false otherwise.
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
 * @return true if was successful, false otherwise
 * @brief Activate VSync
 */
bool enableVsync(sdl_ctx_t *sdl_ctx);

/**
 * @fn disableVsync(sdl_ctx_t *sdl_ctx)
 * @return true if was successful, false otherwise
 * @brief Deactivate VSync
 */
bool disableVsync(sdl_ctx_t *sdl_ctx);

/**
 * @fn clearContextSurface(sdl_ctx_t *sdl_ctx)
 * @brief Clear the background surface
 */
void clearContextSurface(sdl_ctx_t *sdl_ctx);

/**
 * \fn loadBackgroundImage(sdl_ctx_t *sdl_ctx, char *path)
 * \return result Returns true if image was loaded successfully, false otherwise.
 * \brief cette fonction effectue un appel a chargerIamge(), et affecte le retour au contxet SDL, la bgTexture, bgSurface a été
 * détruit car il ne sert plus a rien
 */
bool loadBackgroundImage(sdl_ctx_t *sdl_ctx, char *path);

/**
 * @fn renderBackground(sdl_ctx_t *sdl_ctx);
 * @param[in] sdl_ctx Our SDL context
 * @brief Renders the background texture if not NULL (?)
 */
void renderBackground(sdl_ctx_t *sdl_ctx);

/**
 * @fn loadFont(char *path, float fontSize, int fontStyle, int outline)
 * @brief Loads a font based on its path, font size, font styling and its outline if any
 */
TTF_Font *loadFont(char *path, float fontSize, int fontStyle, int outline);

#endif // SDL_CTX_H_

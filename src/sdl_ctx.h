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
 * @fn init_ctx(sdl_ctx_t *sdl_ctx)
 * @brief Initializes our sdl context
 * @param[out] result Returns true operations were a success, false otherwise.
 */
bool initCtx(sdl_ctx_t *sdl_ctx);

/**
 * @fn close_SDL()
 * @param[in] sdl_ctx Our SDL context
 * @brief Destroys the SDL window
 *
 * Helper function to deallocate and put all of the SDL context pointers to NULL.
 */
void closeCtx(sdl_ctx_t **sdl_ctx);

/**
 * @fn renderBackground(sdl_ctx_t *sdl_ctx);
 * @param[in] sdl_ctx Our SDL context
 * @brief Renders the background texture if not NULL (?)
 */
void renderBackground(sdl_ctx_t *sdl_ctx);

/**
 * \fn void chargerImageBg(char *chemin)
 * \return ne retourne rien, il faut vérifier dans le contexte SDL si la Texture est a NULL ou pas
 * \brief cette fonction effectue un appel a chargerIamge(), et affecte le retour au contxet SDL, la bgTexture, bgSurface a été
 * détruit car il ne sert plus a rien
 */
void loadBackgroundImage(sdl_ctx_t *sdl_ctx, char *chemin);

/**
 * @fn enableVsync(sdl_ctx_t *sdl_ctx)
 * @return true if was successful, false otherwise
 * @brief Active la vsync
 */
bool enableVsync(sdl_ctx_t *sdl_ctx);

/**
 * @fn disableVsync(sdl_ctx_t *sdl_ctx)
 * @return true if was successful, false otherwise
 * @brief Active la vsync
 */
bool disableVsync(sdl_ctx_t *sdl_ctx);

/**
 * @fn clearContextSurface(sdl_ctx_t *sdl_ctx)
 * @brief Vide le buffer de surface du context SDL
 */
void clearContextSurface(sdl_ctx_t *sdl_ctx);

/**
 * @fn loadFont(char *path, float size, int fontStyle, int outline)
 */
TTF_Font *loadFont(char *path, float size, int fontStyle, int outline);

#endif // SDL_CTX_H_

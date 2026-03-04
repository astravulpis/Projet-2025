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
 * @fn renderBackground(sdl_ctx_t *sdl_ctx);
 * @param[in] sdl_ctx Our SDL context
 * @brief Renders the background texture if not NULL (?)
 */
void renderBackground(sdl_ctx_t *sdl_ctx);

/**
 * @fn renduImage(sdl_ctx_t *sdl_ctx, SDL_Texture *textureImg, float x, float y, float width, float height)
 * @brief Fait le rendu d'une SDL_Texture au coordonnées x, y.
 */
void renduImage(sdl_ctx_t *sdl_ctx, SDL_Texture *textureImg, SDL_FRect *rect);

/**
 * \fn SDL_Texture *chargerImage(char *chemin)
 * \return retourne un pointeur de type SDL_Texture
 * \brief charge une image, l'alloue en mémoire, et retourne son pointeur en mémoire
 */
SDL_Texture *chargerImage(sdl_ctx_t *sdl_ctx, char *chemin);

/**
 * \fn void chargerImageBg(char *chemin)
 * \return ne retourne rien, il faut vérifier dans le contexte SDL si la Texture est a NULL ou pas
 * \brief cette fonction effectue un appel a chargerIamge(), et affecte le retour au contxet SDL, la bgTexture, bgSurface a été détruit car il ne sert plus a rien
 */
void loadBackgroundImage(sdl_ctx_t *sdl_ctx, char *chemin);

SDL_FRect *createRect(float x, float y, float width, float height);
bool enableVsync(sdl_ctx_t *sdl_ctx);
bool disableVsync(sdl_ctx_t *sdl_ctx);
void clearContextSurface(sdl_ctx_t *sdl_ctx);

#endif //SDL_HELPER_H_

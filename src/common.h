#ifndef COMMON_H_
#define COMMON_H_

#ifndef NOB_IMPLEMENTATION
#include "../thirdparty/nob.h"
#endif // NOB_IMPLEMENTATION

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#if defined(DEBUG)
#define BREAKPOINT __asm("int3");
#else
#define BREAKPOINT
#endif // DEBUG

/**
 * @typedef struct sdl_ctx_t
 * @brief shorter abreviation for @ref sdl_context_s
 */
typedef struct sdl_context_s sdl_ctx_t;

/**
 * @struct sdl_context_s
 * @brief Act as our sdl context with multiple variables used about everywhere
 *
 * It holds multiple variable for the rendering of the window, the surface of
 * said window, the event that can be pulled and if it's running or not
 */
struct sdl_context_s {
    SDL_Window *window;     //!< SDL3 window context
    SDL_Renderer *renderer; //!< SDL3 renderer context
    SDL_Texture *bgTexture; //!< SDL3 surface renderer texture context
    SDL_FRect *bgRect;      //!< SDL3 surface renderer rectangle
    SDL_Event event;        //!< Structure commune au contexte SDL.
    bool vsyncActivation;   //!< boolean qui permet de prevenir si l'application est en VSync.
    bool quit;              //!< Boolean permettant d'indiquer la fin d'arret du programme
};

#endif // COMMON_H_

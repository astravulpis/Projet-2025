#ifndef COMMON_H_
#define COMMON_H_

#include "../thirdparty/nob.h"
#include "SDL3/SDL.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#if defined(DEBUG)
#    define BREAKPOINT __asm("int3");
#else
#    define BREAKPOINT
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
 * It holds multiple variable for the rendering of the window, the surface of said window, the event that can be pulled and if the program is running or not.
 */
struct sdl_context_s {
    SDL_Window *window;     //!< SDL window context
    SDL_Renderer *renderer; //!< SDL renderer context
    SDL_Surface *bgSurface; //!< SDL window's background surface (needs to be made into a struct sdl_RenderSurface or smth)
    SDL_Texture *bgTexture; //!< SDL window's background texture (needs to be made into a struct sdl_RenderSurface or smth)
    SDL_Event event;
    bool vsyncActivation;   //!< bool that allows the program to not hog the system's ressources (TEMPORARY SOLUTION)
    bool quit;              //!< bool to quit the main loop
};

#endif //COMMON_H_

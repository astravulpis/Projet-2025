#ifndef COMMON_H_
#define COMMON_H_

#ifndef NOB_IMPLEMENTATION
#include "../thirdparty/nob.h"
#endif // NOB_IMPLEMENTATION

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"

#include <assert.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#if defined(DEBUG)
#define BREAKPOINT __asm("int3");
#else
#define BREAKPOINT
#endif // DEBUG

#define WHITE (SDL_Color){255, 255, 255, 255}
#define BLACK (SDL_Color){0, 0, 0, 0}

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (y) : (x))

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
    TTF_Font *font;         //!< SDL3 ttf font
    SDL_Event event;        //!< Event struct to pull from
    bool vsyncActivation;   //!< Global state for application to tell if it's in vsyc or not
    bool quit;              //!< Global state to keep the application running or not
};

typedef struct {
    int x;
    int y;
} V2i;

typedef struct {
    float x;
    float y;
} V2f;

#endif // COMMON_H_

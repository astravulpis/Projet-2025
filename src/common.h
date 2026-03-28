/**
 * @file common.h
 * @brief Regroups all of the common includes, defines and structures used across the program
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-03-03
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Rossignol François <francois_rossignol@outlook.fr>
 **/

#ifndef COMMON_H_
#define COMMON_H_

#ifndef NOB_IMPLEMENTATION
#include "../thirdparty/nob.h"
#endif // NOB_IMPLEMENTATION

#ifndef FLAG_IMPLEMENTATION
#include "../thirdparty/flag.h"
#endif // FLAG_IMPLEMENTATION

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"

#include <assert.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#if defined(DEBUG)
#define BREAKPOINT __asm("int3");
#else
#define BREAKPOINT
#endif // DEBUG

#define WHITE (SDL_Color){255, 255, 255, 255}
#define BLACK (SDL_Color){0, 0, 0, 0}
#define RED (SDL_Color){255, 0, 0, 255}
#define PURPLE (SDL_Color){255, 255, 0, 255}
#define GREEN (SDL_Color){0, 255, 0, 255}
#define BLUE (SDL_Color){0, 0, 255, 255}

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
    float screenRatio;      //!< Ratio for adaptative ui link the screen size
    bool vsyncActivation;   //!< Global state for application to tell if it's in vsyc or not
    bool quit;              //!< Global state to keep the application running or not
    bool paused;            //!< Global state to stop the game running or not
};

typedef struct {
    int x;
    int y;
} V2i;

typedef struct {
    float x;
    float y;
} V2f;

/**
 * @struct obj
 * @brief contains the bounding box and texture for the player
 *
 * Contains an SDL_FRect and SDL_Texture for the bounding box and textures respectively
 */
typedef struct {
    SDL_FRect *boundingBox;
    SDL_Texture *texture;
} obj;

/**
 * @struct objs
 * @brief is a list of obj
 *
 * contains a list of obj called items, with a count and a upper capacity
 */
typedef struct {
    obj *items;
    size_t count;
    size_t capacity;
} objs;

#define obj_create(array, ctx, path, x, y, width, height)                                                           \
    da_append((array), ((obj){createRect((x), (y), (width), (height)), IMG_LoadTexture((ctx)->renderer, (path))}));

#endif // COMMON_H_

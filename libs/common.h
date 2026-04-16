/**
 * @file common.h
 * @brief Regroups all of the common includes, defines and structures used across the program
 *
 * @author Liam B. <liam.berge72@gmail.com>
 * @date 2026-03-01
 * @remark last lodified: 2026-04-11
 *
 * Contributors:
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
#include "SDL3_mixer/SDL_mixer.h"
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
 * @typedef enum track_kind
 * @brief enum for all the sound effects in the game, including player and enemy
 */
typedef enum {
    BACKGROUND_MUSIC,
    START_MENU_MUSIC,
    SFX_PLAYER_MOVE,
    SFX_PLAYER_FALL,
    SFX_PLAYER_GUNS,
    SFX_PLAYER_INTERACTIONS,
    SFX_ENEMY_MOVE,
    SFX_ENEMY_DIE,
    SFX_ENEMY_INTERACTIONS,
    SFX_ENEMY_SPAWN,
    __count_tracks = 10,
} track_kind;

/**
 * @typedef enum menu_kind
 * @brief contains the enum to the different menus found in the game
 */
typedef enum {
    NONE_MENU,
    PAUSE_MENU,
    OPTIONS_MENU,
    START_MENU,
    LEVEL_SELECTION_MENU,
    __menu_count = 5,
} menu_kind;


/**
 * @typedef struct options
 * @brief contains the different sound options for the game
 */
typedef struct {
    float masterVolume;
    float musicVolume;
    float sfxVolume;
} options;

/**
 * @typedef struct sdl_ctx_t
 * @brief shorter abreviation for @ref sdl_context_s
 */
typedef struct sdl_context_s sdl_ctx_t;

/**
 * @struct sdl_context_s
 * @brief Act as structure
 *
 * It holds multiple variable for the rendering of the window, the surface of
 * said window, the event that can be pulled and if it's running or not
 * also countains options such as vsync and audio elements
 */
struct sdl_context_s {
    SDL_Window *window;     //!< SDL3 window context
    SDL_Renderer *renderer; //!< SDL3 renderer context
    SDL_Texture *bgTexture; //!< SDL3 surface renderer texture context
    SDL_Texture *logoImg;   //!< The logo of the game used in menu's
    SDL_FRect *bgRect;      //!< SDL3 surface renderer rectangle
    TTF_Font *font;         //!< SDL3 ttf font
    SDL_Event event;        //!< Event struct to pull from
    options opts;
    float screenRatio;      //!< Ratio for adaptative ui link the screen size
    bool vsyncActivation;   //!< Global state for application to tell if it's in vsyc or not
    bool quit;              //!< Global state to keep the application running or not
    bool isLevelReset;      //!< Global state to tell if the background music is playing or not
    menu_kind currMenu;
    menu_kind prevMenu;
    MIX_Mixer *mixer;
    MIX_Track *tracks[__count_tracks];
};

/**
 * @typedef struct V2f
 * @brief positional values
 */
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
    SDL_FRect *boundingBox; //!< the hitbox of the object
    SDL_Texture *texture; //!< the texture of the object
} obj;

/**
 * @struct objs
 * @brief is a list of obj
 *
 * contains a list of obj called items, with a count and a upper capacity
 */
typedef struct {
    obj *items; //!< list of all objects
    size_t count; //!< current amount of objects in the list
    size_t capacity; //!< upper capacity of the list (should never be reached since it's a dynamic array)
} objs;

/**
 * @typedef struct entity_t
 * @brief entity information
 *
 * containes the bounding box, texture, max health, current health and score value of the entity
 */
typedef struct {
    sdl_ctx_t **ctx;        //!< Address of our sdl context to be saved
    SDL_FRect *boundingBox; //!< Player's BB
    SDL_Texture *tex;       //!< Player's texture
    float hp; //!< Player's current HP
    float maxHp; //!< Player's max HP
    float score; //!< Player's score
    int selectedGunIndex;
    bool isAlive;
    float shotcooldown;
    float contactDamage;
} entity_t;

#define obj_create(array, ctx, path, x, y, width, height)                                                           \
    da_append((array), ((obj){createRect((x), (y), (width), (height)), IMG_LoadTexture((ctx)->renderer, (path))}));

/**
 * @typedef struct mouseDevice
 * @brief the mouse's info
 *
 * contains the position of the mouse, it's current and previous state
 */
typedef struct {
    V2f position; //!< the mouse's position
    int currState; //!< the mouse's current state
    int prevState; //!< the mouse's previous state
} mouseDevice;


#endif // COMMON_H_

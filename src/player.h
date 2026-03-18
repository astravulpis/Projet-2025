#ifndef PLAYER_H_
#define PLAYER_H_

#include "common.h"

typedef struct {
    sdl_ctx_t **ctx;        //!< Address of our sdl context to be saved
    SDL_FRect *boundingBox; //!< Player's BB
    SDL_Texture *tex;       //!< Player's texture
    float speed;            //!< Value may depend on preference
    bool onGround;          //!< State to tell whenever the player is on the ground or not
    // bool stunned;           //!< State used to avoid key mashing
    float stunnedTimer;     //!< Float used as a timer for the amount of time stunned
} player_t;

/**
 * @fn createPlayer(player_t **player, V2f playerSize, sdl_ctx_t **sdl_ctx, const char *path)
 * @param[in] player pointer pointer to the player itself
 * @param[in] playerSize X and Y value for it's height and width
 * @param[in] sdl_ctx pointer pointer to the ctx variable
 * @param[in] path where the player file is located
 * @brief creates the player, makes sur it exists and then loads it
 *
 * creates the player and is only called once
 */
bool createPlayer(player_t **player, V2f playerSize, sdl_ctx_t **sdl_ctx, const char *path);

/**
 * @fn destroyPlayer(player_t **p)
 * @param[in] player pointer pointer to the player itself
 * @brief destroys the player
 *
 * Self explanatory
 */
void destroyPlayer(player_t **p);

/**
 * @fn UpdatePlayer(player_t *p, SDL_FRect *objects, int object_count, float deltaTime)
 * @param[in] p pointer to the player
 * @param[in] objects array of objects that we will be going through to check collisions
 * @param[in] object_count so we know how many elements are in objects
 * @param[in] deltaTime this is so we buffer the next frame (I think?)
 * @brief updates the player position
 *
 * updates the player position and checks for collisions with all other textures on the map
 */
void UpdatePlayer(player_t *p, SDL_FRect *objects, int object_count, float deltaTime);

/**
 * @fn renderPlayer(player_t *p)
 * @param[in] p this is the player
 * @brief drenders the player image
 *
 * renders the player what more do you want?
 */
void renderPlayer(player_t *p);

#endif // PLAYER_H_

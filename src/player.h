/**
 * @file player.h
 * @brief Header file to represent the player and all of its related features
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-03-16
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 **/

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
    // float stunnedTimer;     //!< Float used as a timer for the amount of time stunned
    float jumpForce;
    int dashAmount;
    float dashTimer;
    V2f velocity;
    uint8_t lastKey;
} player_t;

#define getBB(p) (p)->boundingBox

#define Top(p)     (p)->y
#define Bottom(p)  (p)->y + (p)->h
#define Left(p)    (p)->x
#define Right(p)   (p)->x + (p)->w

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
 * @param[in] p The player
 * @param[in] object Object that we check collisions against
 * @param[in] deltaTime To make thing independent of refresh of one's screen
 * @brief updates the player position
 *
 * updates the player position and checks for collisions with all other textures on the map
 */
void UpdatePlayer(player_t *p, objs *arr, float deltaTime);

/**
 * @fn renderPlayer(player_t *p)
 * @param[in] p this is the player
 * @brief drenders the player image
 *
 * renders the player what more do you want?
 */
void renderPlayer(player_t *p);

/**
 * @fn movePlayer(player_t *p, V2f newPos)
 * @param[in] p player structure
 * @param[in] newPos the player's new position after moving
 * @brief moves the player
 *
 * moves the player's bounding box based on the new position
 */
void movePlayer(player_t *p, V2f newPos);

/**
 * @fn keep_player_inbound(SDL_FRect *r, float minX, float minY, float maxX, float maxY)
 * @param[in] r The SDL rect given to check
 * @param[in] minX Minimum X bound
 * @param[in] minY Minimum Y bound
 * @param[in] maxX Maximum X bound
 * @param[in] maxY Maximum Y bound
 * @brief keep rectangle in the given bounds
 *
 * Takes the location of the rect we are moving then compares it to the bounds given.
 */
void keepPlayerInbound(SDL_FRect *r, float minX, float minY, float maxX, float maxY);
#endif // PLAYER_H_

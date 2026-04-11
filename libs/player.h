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
 * François Rossignol <francois_rossignol@outlook.fr>
 **/

#ifndef PLAYER_H_
#define PLAYER_H_

#include "bars.h"
#include "common.h"
#include "music.h"
#include "player_animation.h"

typedef enum {
    IDLE_ANIM,
    RUN_ANIM,
    ON_AIR_ANIM,
    DASH_ANIM,
    SLAM_ANIM,
    ON_WALL_ANIM,
    __count_player_anim_kind = 6
} player_anim_kind;

/**
 * @typedef struct player_t
 * @brief contains every single bit of information regarding the player
 *
 * contains the player's speed, hitbox, jump force, health, animation information, sound effects and so on
 */
typedef struct {
    entity_t entity_attribs;
    player_animation *anims[__count_player_anim_kind];
    SDL_Texture *onWallTex;
    sfxs audios;
    float speed;            //!< Value may depend on preference
    V2f velocity;
    // bool stunned;           //!< State used to avoid key mashing
    // float stunnedTimer;     //!< Float used as a timer for the amount of time stunned
    float stamina;
    float jumpForce;
    uint8_t lastKey;
    float lastX; // the latest x coord of deltaPos

    float dashAnimationTime;
    float prevDashTick;

    float hp;

    float score;

    bool onGround; //!< State to tell whenever the player is on the ground or not
    bool onWall; //!< State to tell whenever the player is glued to a wall or not
    bool isSlamming;
    bool isDashing;
    bool flight;
    bool noclip;

    bool run;
} player_t;

#define getBB(p) (p)->entity_attribs.boundingBox

#define Top(p) (p)->y
#define Bottom(p) (p)->y + (p)->h
#define Left(p) (p)->x
#define Right(p) (p)->x + (p)->w

/**
 * @fn createPlayer(player_t **player, V2f playerSize, sdl_ctx_t **sdl_ctx, const char *path)
 * @param[in] player pointer pointer to the player itself
 * @param[in] playerSize X and Y value for it's height and width
 * @param[in] sdl_ctx pointer pointer to the ctx variable
 * @brief creates the player, makes sur it exists and then loads it
 *
 * creates the player and is only called once
 */
bool createPlayer(player_t **player, V2f playerSize, sdl_ctx_t **sdl_ctx);

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
void updatePlayer(player_t *p, objs *arr, float deltaTime);

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
 * @fn createPlayerStatusBar(sdl_ctx_t *sdl_ctx, bar **b1, bar **b2, bar **b3, bar **hpB)
 * @param[in] sdl_ctx SDL context variable
 * @param[in] b1 first portion of the dash bar
 * @param[in] b2 second portion of the dash bar
 * @param[in] b3 third portion of the dash bar
 * @param[in] hpB player health bar
 * @brief creates all the player's information bars (for now health and dash cooldown)
 */
bool createPlayerStatusBar(sdl_ctx_t *sdl_ctx, bar **b1, bar **b2, bar **b3, bar **hpB);

/**
 * @fn renderPlayerStatusBar(sdl_ctx_t *sdl_ctx, player_t *player, bar *b1, bar *b2, bar *b3, bar *hpB)
 * @param[in] sdl_ctx SDL context variable
 * @param[in] b1 first portion of the dash bar
 * @param[in] b2 second portion of the dash bar
 * @param[in] b3 third portion of the dash bar
 * @param[in] hpB player health bar
 * @brief renders all the player's information bars (for now just health and dash information)
 */
void renderPlayerStatusBar(sdl_ctx_t *sdl_ctx, player_t *player, bar *b1, bar *b2, bar *b3, bar *hpB);

/**
 * @fn destroyPlayerStatusBar(bar **b1, bar **b2, bar **b3, bar **hpB)
 * @param[in] b1 first portion of the dash bar
 * @param[in] b2 second portion of the dash bar
 * @param[in] b3 third portion of the dash bar
 * @param[in] hpB player health bar
 * @brief destorys all the player's bars
 *
 * moves the player's bounding box based on the new position
 */
void destroyPlayerStatusBar(bar **b1, bar **b2, bar **b3, bar **hpB);

/**
 * @fn initAllPlayerAnimation(sdl_ctx_t *sdl_ctx, player_t *player)
 * @param[in] sdl_ctx first portion of the dash bar
 * @param[in] player player structure
 * @brief initializes all of the player animations
 */
bool initAllPlayerAnimation(sdl_ctx_t *sdl_ctx, player_t *player);

/**
 * @fn inputUpdate(player_t *p, const float dt)
 * @param[in] p player structure
 * @param[in] dt delta time
 * @brief updates the player's inputs and adjusts them according to the delta time
 */
V2f inputUpdate(player_t *p, const float dt);

/**
 * @fn collision_test_player(player_t *p, objs *tiles)
 * @param[in] p player structure
 * @param[in] tiles list of all objects
 * @brief checks to see if the player is colliding with anything by going through the entire list of objects in the room
 */
objs collision_test_player(player_t *p, objs *tiles);

/**
 * @fn resetAnimationStates(player_t *player, player_anim_kind excluded_anim)
 * @param[in] player player structure
 * @param[in] excluded_anim animation we are excluding
 * @brief resets the player's animation state
 */
void resetAnimationStates(player_t *player, player_anim_kind excluded_anim);
#endif // PLAYER_H_

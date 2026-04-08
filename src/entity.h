/**
 * @file entity.h
 * @brief Definition of the entity structures
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Date: 2026-03-27
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 **/

#ifndef ENTITY_H_
#define ENTITY_H_

#include "common.h"
#include "player.h"
#include "sdl_helpers.h"

#define BASE_SPEED 250

/**
 * @enum entity_type
 * @brief entity types
 *
 * enum containing a list of all entities
 */
typedef enum {
    E_FILTH,
    E_STRAY,
    E_SWORDSMACHINE,
    E_PROVIDENCE,
    E_VERTU,
    E_MAURICE,
    E_MINOS_PRIME,
    E_SISYPHUS,

    E_TYPE_COUNT = 8
} entity_type;

extern SDL_Texture *entity_textures[E_TYPE_COUNT];

/**
 * @enum entity_state
 * @brief entity states
 *
 * enum containing the current state of the associated entity
 */
typedef enum {
    STATE_IDLE,
    STATE_PURSUING,
    // STATE_ATTACKING,
} entity_state;

/**
 * @struct entity_t
 * @brief entity attributes
 *
 * contains all the attributes one entity can have such as speed and jump force
 */
typedef struct {
    float entity_speed;
    float projectile_speed;
    // -------------
    // projectile_pattern pattern;
    // This will be useful for when we create bullet pattern for the player's gun and need to
    // re-use for the ennemies.
    // -------------
    float jumpForce; //!< Unsure whenever it is needed, or not.
    float hp;
    float maxHP;
    // BEHAVIOUR PART
    entity_state state;
} entity_attributs;

/**
 * @struct entity_t
 * @brief entity data
 *
 * contains all the relevant and corresponding entity information
 */
typedef struct {
    SDL_FRect *boundingBox;
    SDL_Texture *texture;
    sdl_ctx_t **ctx;
    entity_type type;
    entity_attributs attributs;
    V2f velocity;
    bool onGround;
} entity_t;

/**
 * @struct entities_t
 * @brief list of entities
 */
typedef struct {
    entity_t **items;
    size_t count;
    size_t capacity;
} entities;


/**
 * @fn createEntity(sdl_ctx_t **sdl_ctx, entity_type type, V2f basePos)
 * @param[in] sdl_ctx our won sdl context variable
 * @param[in] basePos spawning location
 * @brief creates a creature of a certain type and with a set default spawning location
 */
entity_t *createEntity(sdl_ctx_t **sdl_ctx, entity_type type, V2f basePos);

/**
 * @fn renderEntity(entity_t *e)
 * @param[in] e The entity to render
 * @brief renders the texture of the entity onto the screen
 */
void renderEntity(entity_t *e);

/**
 * @fn renderEntities(entities *entities)
 * @param[in] entities The array of entities to loop through and render
 * @brief wrapper for render in a for-loop of each entity in the current level
 */
void renderEntities(entities *entities);

/**
 * @fn updateEntity(entity_t e, player_t *player, bullets *projectiles, objs *objects, float deltaTime)
 * @param[in] e one specific entity
 * @param[in] player does nothing (for now...)
 * @param[in] objects list of all level objects that we iterate through
 * @param[in] deltaTime time interval used to control the enemy speeds
 * @brief updates the attributs and behaviour of the entity
 *
 * takes one entity and tests it for collisions against all elements of the level
 * and updates its related information
 */
void updateEntity(entity_t *e, player_t *player, objs *objects, float deltaTime);

/**
 * @fn updateEntities(entities *entities, player_t *player, bullets *projectiles, objs *objects, float deltaTime)
 * @param[in] entities array of entities
 * @param[in] player does nothing (for now...)
 * @param[in] objects list of all level objects that we iterate through
 * @param[in] deltaTime time interval used to control the enemy speeds
 * @brief wrapper for the update in a for-loop of each entity in the current level
 *
 * goes through a loop to check on every entity by then calling the updateEntity function
 */
void updateEntities(entities *entities, player_t *player, objs *objects, float deltaTime);

#define setEntityAttributs(e, ...) _setEntityAttributs((e), (entity_attributs){__VA_ARGS__});

/**
 * @fn _setEntityAttributs(entity_t *e, entity_attributs attributs)
 * @param[in] e structure of 1 entity
 * @param[in] attributs enemy attributs
 * @brief sets corresponding enemy attributs
 *
 * takes one entity and gives it all the necessary attributs
 */
void _setEntityAttributs(entity_t *e, entity_attributs attributs);

/**
 * @fn setEntityState(entity_t *e, entity_state state)
 * @param[in] e structure of 1 entity
 * @param[in] state entity state
 * @brief sets the state of the enemy
 */
void setEntityState(entity_t *e, entity_state state);

/**
 * @fn getEntityState(entity_t *e)
 * @param[in] e structure of 1 entity
 * @brief gets the state of the enemy
 */
entity_state getEntityState(entity_t *e);

/**
 * @fn setMaxHP(entity_t *e, float maxHP)
 * @param[in] e structure of 1 entity
 * @param[in] maxHP total HP given to the enemy
 * @brief sets the max HP of the enemy
 */
void setMaxHP(entity_t *e, float maxHP);

/**
 * @fn setHP(entity_t *e, float HP)
 * @param[in] e structure of 1 entity
 * @param[in] HP current HP
 * @brief sets the current HP of the enemy
 */
void setHP(entity_t *e, float HP);

/**
 * @fn setEntitySpeed(entity_t *e, float speed)
 * @param[in] e structure of 1 entity
 * @param[in] speed corresponding speed
 * @brief sets the current speed of the entity
 */
void setEntitySpeed(entity_t *e, float speed);

/**
 * @fn destroyEntity(entity_t **e)
 * @param[in] e structure of 1 entity
 * @brief destroys one entity
 */
void destroyEntity(entity_t **e);

/**
 * @fn destroyEntity(entity_t **e)
 * @param[in] e structure of the entities
 * @brief destorys all entities
 *
 * Goes through the list of all entities and destroys them one by one by calling the destroyEntity function
 */
void destroyEntities(entities *entities);

#endif // ENTITY_H_

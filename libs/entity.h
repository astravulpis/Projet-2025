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

    __count_enemy_type = 8
} entity_type;

extern SDL_Texture *entity_textures[__count_enemy_type];

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
 * contains all the attributes one entity can have such as speed and jump force, health and score value
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
    float score;
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
    entity_t entity_attribs;
    entity_type type;
    entity_attributs attributs;
    V2f velocity;
    bool onGround;
    char *attackSfx;
} ennemy_t;

/**
 * @struct entities_t
 * @brief list of entities
 */
typedef struct {
    ennemy_t **items;
    size_t count;
    size_t capacity;
} entities;

/**
 * @fn createEntity(sdl_ctx_t **sdl_ctx, entity_type type, V2f basePos)
 * @param[in] sdl_ctx our won sdl context variable
 * @param[in] basePos spawning location
 * @param[in] type entity type (i.e. E_FILTH)
 * @brief creates a creature of a certain type and with a set default spawning location
 */
ennemy_t *createEntity(sdl_ctx_t **sdl_ctx, entity_type type, V2f basePos);

/**
 * @fn playEnemySpawning(sdl_ctx_t *ctx)
 * @param[in] sdl_ctx our sdl context variable
 * @brief plays the spawning sfx
 */
void playEnemySpawning(sdl_ctx_t *ctx);

/**
 * @fn playEnemySpawning(sdl_ctx_t *ctx)
 * @param[in] sdl_ctx our sdl context variable
 * @brief plays the death sfx
 */
void playEnemyDeath(sdl_ctx_t *ctx);

/**
 * @fn renderEntity(entity_t *e)
 * @param[in] e The entity to render
 * @brief renders the texture of the entity onto the screen
 */
void renderEntity(ennemy_t *e);

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
void updateEntity(ennemy_t *e, player_t *player, objs *objects, float deltaTime);

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
void _setEntityAttributs(ennemy_t *e, entity_attributs attributs);

/**
 * @fn setEntityState(entity_t *e, entity_state state)
 * @param[in] e structure of 1 entity
 * @param[in] state entity state
 * @brief sets the state of the enemy
 */
void setEntityState(ennemy_t *e, entity_state state);

/**
 * @fn getEntityState(entity_t *e)
 * @param[in] e structure of 1 entity
 * @brief gets the state of the enemy
 */
entity_state getEntityState(ennemy_t *e);

/**
 * @fn setMaxHP(entity_t *e, float maxHP)
 * @param[in] e structure of 1 entity
 * @param[in] maxHP total HP given to the enemy
 * @brief sets the max HP of the enemy
 */
void setMaxHP(ennemy_t *e, float maxHP);

/**
 * @fn setHP(entity_t *e, float HP)
 * @param[in] e structure of 1 entity
 * @param[in] HP current HP
 * @brief sets the current HP of the enemy
 */
void setHP(ennemy_t *e, float HP);

/**
 * @fn setEntitySpeed(entity_t *e, float speed)
 * @param[in] e structure of 1 entity
 * @param[in] speed corresponding speed
 * @brief sets the current speed of the entity
 */
void setEntitySpeed(ennemy_t *e, float speed);

/**
 * @fn destroyEntity(entity_t **e)
 * @param[in] e structure of 1 entity
 * @brief destroys one entity
 */
void destroyEntity(ennemy_t **e);

/**
 * @fn destroyEntity(entity_t **e)
 * @param[in] e structure of the entities
 * @brief destorys all entities
 *
 * Goes through the list of all entities and destroys them one by one by calling the destroyEntity function
 */
void destroyEntities(entities *entities);

/**
 * @fn loadEntityTex(sdl_ctx_t *ctx)
 * @param[in] ctx our context variable
 * @brief loads the entity's texture into the ctx
 */
void loadEntityTex(sdl_ctx_t *ctx);

/**
 * @fn getEntityTex(sdl_ctx_t *ctx, int index)
 * @param[in] ctx our context variable
 * @param[in] index position of the texture
 * @brief returns the texture of the entity at the index position
 */
SDL_Texture *getEntityTex(sdl_ctx_t *ctx, int index);

/**
 * @fn getAngle(ennemy_t *e)
 * @param[in] e enemy structure
 * @brief returns the angle of the enemy (aka is it going left or right to render it correctly)
 */
float getAngle(ennemy_t *e);

/**
 * @fn collision_test_entity(ennemy_t *e, objs *tiles)
 * @param[in] e enemy structure
 * @param[in] tiles list of objects
 * @brief collision test of the entity with the level objects
 */
objs collision_test_entity(ennemy_t *e, objs *tiles);

#endif // ENTITY_H_

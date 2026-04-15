/**
 * @file entity.h
 * @brief Definition of the entity structures
 *
 * @author Liam B. <liam.berge72@gmail.com>
 * @date 2026-03-27
 *
 * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 **/

#ifndef ENTITY_H_
#define ENTITY_H_

#include "common.h"
#include "player.h"
#include "bullets.h"

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
    float entity_speed; //!< entity movement speed, self explanatory
    float projectile_speed; //!< the speed of the projectiles shot by the entity (only those who can shoot (played is excluded because of the guns))
    // -------------
    // projectile_pattern pattern;
    // This will be useful for when we create bullet pattern for the player's gun and need to
    // re-use for the ennemies.
    // -------------
    float jumpForce; //!< Unsure whenever it is needed, or not.
    float hp; //!< the current HP of the entity, used to check for death and such
    float maxHP; //!< the maximum HP of the entity, used when initializing the entity
    float score; //!< the score value of the entity, used to give the player points when killed
    float contactDamage;
    // BEHAVIOUR PART
    entity_state state; //!< the current state of the entity, used to determine its behaviour in the update function
    float detection_range;

} entity_attributs;

/**
 * @struct entity_t
 * @brief entity data
 *
 * contains all the relevant and corresponding entity information
 */
typedef struct {
    entity_t entity_attribs; //!< all the entity's attributes
    entity_type type; //!< the type of the entity this is then used for texture and behaviour
    entity_attributs attributs; //!< i have no idea
    V2f velocity; //!< not used for now
    bool onGround; //!< used for the jump functions
    char *attackSfx; //!< path to the entity's attack sound effect
    int direction;
} ennemy_t;

/**
 * @struct entities_t
 * @brief list of entities
 */
typedef struct {
    ennemy_t **items; //!< list of all entities
    size_t count; //!< current amount of entities in the list
    size_t capacity; //!< upper capacity of the list (should never be reached since it's a dynamic array)
} entities;

/**
 * @fn createEntity(sdl_ctx_t **sdl_ctx, entity_type type, V2f basePos)
 * @param[in] sdl_ctx our won sdl context variable
 * @param[in] basePos spawning location
 * @param[in] type entity type (i.e. E_FILTH)
 * @brief creates a creature of a certain type and with a set default spawning location
 *
 * allocates the entity in memory, initializes it's attributes and ctx with calls to \ref getEntityTex and
 * \ref loadEnemySfx, gives it a bounding box and its attributes through \ref setEntityAttributs and then
 * returns the newly created entity
 */
ennemy_t *createEntity(sdl_ctx_t **sdl_ctx, entity_type type, V2f basePos);

/**
 * @fn playEnemySpawning(sdl_ctx_t *ctx)
 * @param[in] sdl_ctx our sdl context variable
 * @brief plays the spawning sfx
 *
 * calls \ref playSfx with the corresponding sound effect for the enemy spawning
 */
void playEnemySpawning(sdl_ctx_t *ctx);

/**
 * @fn playEnemySpawning(sdl_ctx_t *ctx)
 * @param[in] sdl_ctx our sdl context variable
 * @brief plays the death sfx
 *
 * calls \ref playSfx with the corresponding sound effect for the enemy death
 */
void playEnemyDeath(sdl_ctx_t *ctx);

/**
 * @fn renderEntity(entity_t *e)
 * @param[in] e The entity to render
 * @brief renders the texture of the entity onto the screen
 *
 * gets the angle of the entity with \ref getAngle to know if it should be flipped
 * or not and then renders it with the corresponding texture and hitbox by calling
 * \ref SDL_RenderTextureRotated
 */
void renderEntity(ennemy_t *e);

/**
 * @fn renderEntities(entities *entities)
 * @param[in] entities The array of entities to loop through and render
 * @brief wrapper for render in a for-loop of each entity in the current level
 *
 * goes through the list of entities and calls \ref renderEntity for each of them to render them
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
 * takes one entity and tests it for collisions against all elements of the level with \ref collision_test_entity
 * then moves it and makes sure it's still in bounds with \ref keepRectInbounds
 */
void updateEntity(ennemy_t *e, player_t *player, objs *objects, float deltaTime, bullets * bullet_array, sdl_ctx_t * ctx);

/**
 * @fn updateEntities(entities *entities, player_t *player, bullets *projectiles, objs *objects, float deltaTime)
 * @param[in] entities array of entities
 * @param[in] player does nothing (for now...)
 * @param[in] objects list of all level objects that we iterate through
 * @param[in] deltaTime time interval used to control the enemy speeds
 * @brief wrapper for the update in a for-loop of each entity in the current level
 *
 * goes through a loop to check on every entity by then calling \ref updateEntity for each of them
 */
void updateEntities(entities *entities, player_t *player, objs *objects, float deltaTime, bullets * bullet_array, sdl_ctx_t * ctx);

#define setEntityAttributs(e, ...) _setEntityAttributs((e), (entity_attributs){__VA_ARGS__});

/**
 * @fn _setEntityAttributs(entity_t *e, entity_attributs attributs)
 * @param[in] e structure of 1 entity
 * @param[in] attributs enemy attributs
 * @brief sets corresponding enemy attributs
 *
 * wrapper to call all the attribute functions with them being \ref setEntitySpeed, \ref setMaxHP,
 * \ref setHP, \ref setEntityState, then the projectile speed, jump force and the score
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
 *
 * frees the entity's bounding box then the entity itself
 */
void destroyEntity(ennemy_t **e);

/**
 * @fn destroyEntity(entity_t **e)
 * @param[in] e structure of the entities
 * @brief destorys all entities
 *
 * Goes through the list of all entities and destroys them one by one by calling \ref destroyEntity
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
 *
 * takes the entity and all the level objects and checks it for collisions with \ref SDL_HasRectIntersectionFloat to
 * put the collisions intto an array which it returns
 */
objs collision_test_entity(ennemy_t *e, objs *tiles);

/**
 * @fn lineOfSight(objs *objects, player_t *player, ennemy_t *e)
 * @param[in] objects list of level objects
 * @param[in] player player structure for the coordinates
 * @param[in] e entity structure for the coordinates
 * @brief checks for a line of sight between the player and an entity
 * 
 * goes through the list of all level objects and draws a line between the player and an entity, then checks if that line is colliding
 * with anything and if it isn't that means there's a line of sight for the enemty to shoot the player
 */
bool lineOfSight(objs *objects, player_t *player, ennemy_t *e);

/**
 * @fn enemyIdle(ennemy_t *e, objs *objects)
 * @param[in] objects list of level objects
 * @param[in] e entity structure for the coordinates
 * @brief simple ennemy idle function for when the player is not within its range
 */
void enemyIdle(ennemy_t *e, objs *objects);

/**
 * @fn entityShootGun(bullets *bullet_arr, V2f position, V2f vel, sdl_ctx_t * ctx, ennemy_t * e)
 * @param[in] bullet_arr list of bullets
 * @param[in] position entity position
 * @param[in] vel normalised direction for the bullet
 * @param[in] ctx our context variable to pass into \ref createBulletEntity
 * @param[in] e entity structure to handle the shot cooldown
 * @brief shooting function for an entity
 * 
 * We are not reusing the \ref shootGun function as we end up with a circular dependancy but it functions in the same way
 */
void entityShootGun(bullets *bullet_arr, V2f position, V2f vel, sdl_ctx_t * ctx, ennemy_t * e);

/**
 * @fn createBulletEntity(bullets *bullet_arr, V2f position, V2f vel, sdl_ctx_t * ctx)
 * @param[in] bullet_arr list of bullets, the same as the one for the player
 * @param[in] position entity position
 * @param[in] vel normalised direction for the bullets
 * @param[in] ctx our context variable to handle the rendering
 * @brief creates a bullet that is shot by an entity. functions in the same way \ref createBullet does but adjusted to avoid 
 * a circular dependancy
 */
bool createBulletEntity(bullets *bullet_arr, V2f position, V2f vel, sdl_ctx_t * ctx);

/**
 * @fn entityCollision(player_t * player, ennemy_t * entity)
 * @param[in] player list of bullets, the same as the one for the player
 * @param[in] entity entity position
 * @brief looks for a collision between the player and an entity and if it's the case then reduce the player's health
 */
void entityCollision(player_t * player, ennemy_t * entity);
#endif // ENTITY_H_

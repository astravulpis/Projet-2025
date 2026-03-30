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

#include "bullets.h"
#include "common.h"
#include "player.h"
#include "sdl_helpers.h"

#define BASE_SPEED 250

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

typedef enum {
    STATE_IDLE,
    STATE_PURSUING,
    // STATE_ATTACKING,
} entity_state;

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

typedef struct {
    sdl_ctx_t **ctx;
    SDL_FRect *boundingBox;
    SDL_Texture *tex;
    entity_type type;
    entity_attributs attributs;
    V2f velocity;
    bool onGround;
} entity_t;

typedef struct {
    entity_t **items;
    size_t count;
    size_t capacity;
} entities;

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
 * @brief updates the attributs and behaviour of the entity
 */
void updateEntity(entity_t *e, player_t *player, bullets *projectiles, objs *objects, float deltaTime);

/**
 * @fn updateEntities(entities *entities, player_t *player, bullets *projectiles, objs *objects, float deltaTime)
 * @brief wrapper for the update in a for-loop of each entity in the current level
 */
void updateEntities(entities *entities, player_t *player, bullets *projectiles, objs *objects, float deltaTime);

#define setEntityAttributs(e, ...) _setEntityAttributs((e), (entity_attributs){__VA_ARGS__});
void _setEntityAttributs(entity_t *e, entity_attributs attributs);

void setEntityState(entity_t *e, entity_state state);
entity_state getEntityState(entity_t *e);

void setMaxHP(entity_t *e, float maxHP);
void setHP(entity_t *e, float HP);
void setEntitySpeed(entity_t *e, float speed);

void destroyEntity(entity_t **e);
void destroyEntities(entities *entities);

#endif // ENTITY_H_

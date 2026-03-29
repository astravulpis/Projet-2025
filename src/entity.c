/**
 * @file entity.c
 * @brief Implementation of the entity structures
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Date: 2026-03-28
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 **/

#include "entity.h"

entity_t *createEntity(sdl_ctx_t **sdl_ctx, const char *texturePath, entity_type type, V2f basePos)
{
    entity_t *e = calloc(1, sizeof(entity_t));
    if (e == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for an entity", __FILE__, __LINE__);
        return NULL;
    }

    e->tex = IMG_LoadTexture((*sdl_ctx)->renderer, texturePath);
    if (e->tex == NULL) {
        nob_log(ERROR, "%s:%d: Failed to load the entity's texture. See error: %s", __FILE__, __LINE__, SDL_GetError());
        free(e);
        return NULL;
    }

    e->type = type;
    e->ctx = sdl_ctx;

    // Each entity has its own parameters
    // That it'd be the size of its bounding box, to each and every attribut defined
    switch (type) {
    case E_FILTH: {
        e->boundingBox = createRect(basePos.x, basePos.y, 45, 100);
        setEntityAttributs(e, .entity_speed = 240, .maxHP = 15.0f);
        break;
    }
    case E_STRAY: {
        TODO("skism");
        break;
    }
    case E_SWORDSMACHINE: {
        TODO("swordmachine");
        break;
    }
    case E_PROVIDENCE: {
        TODO("providence");
        break;
    }
    case E_ANGEL: {
        TODO("angel");
        break;
    }
    case E_MAURICE: {
        TODO("maurice");
        break;
    }
    case E_MINOS_PRIME: {
        TODO("thy end is now");
        break;
    }
    case E_SISYPHUS: {
        TODO("you cannot escape");
        break;
    }
    default:
        UNREACHABLE("entity_type");
        break;
    }

    return e;
}

void setMaxHP(entity_t *e, float maxHP)
{
    e->attributs.maxHP = maxHP;
}

void setHP(entity_t *e, float HP)
{
    e->attributs.hp = (HP >= e->attributs.maxHP) ? e->attributs.maxHP : HP;
}

void setEntitySpeed(entity_t *e, float speed)
{
    e->attributs.entity_speed = speed;
}

void _setEntityAttributs(entity_t *e, entity_attributs attrib)
{
    if (attrib.entity_speed > 0) setEntitySpeed(e, attrib.entity_speed);

    if (attrib.maxHP > 0) setMaxHP(e, attrib.maxHP);

    if (attrib.hp > 0) setHP(e, attrib.hp);

    if (attrib.projectile_speed > 0) e->attributs.projectile_speed = attrib.projectile_speed;

    if (attrib.state >= 0) setEntityState(e, attrib.state);

    if (attrib.jumpForce > 0) e->attributs.jumpForce = attrib.jumpForce;
}

void setEntityState(entity_t *e, entity_state state)
{
    e->attributs.state = state;
}

entity_state getEntityState(entity_t *e)
{
    return e->attributs.state;
}

float getAngle(entity_t *e)
{
    return (e->velocity.x > 0) ? 180.0f : 0.0f;
}

objs collision_test_entity(entity_t *e, objs *tiles)
{
    objs collisions = {0};
    da_foreach (obj, tile, tiles) {
        if (SDL_HasRectIntersectionFloat(getBB(e), getBB(tile))) {
            da_append(&collisions, *tile);
        }
    }

    return collisions;
}

void updateEntity(entity_t *e, player_t *player, bullets *projectiles, objs *objects, float deltaTime)
                  // void (*behaviour_func)(entity_t *, player_t *, bullets *, objs *, float))
{
    float gravity = 28.0f;
    SDL_FRect *rect = getBB(e);
    V2f frame_movement = {e->velocity.x, e->velocity.y};
    e->onGround = false;

    rect->y += frame_movement.y;
    objs collisions = collision_test_entity(e, objects);
    da_foreach (obj, it, &collisions) {
        SDL_FRect *tile = it->boundingBox;
        if (frame_movement.y > 0) {
            rect->y = Top(tile) - rect->h - 0.01f; // Set the player's right edge to the tile's left edge
            e->onGround = true;
        }
        if (frame_movement.y < 0) {
            rect->y = Bottom(tile) + 0.01f; // Set the player's left edge to the tile's right edge
        }
        e->velocity.y = 0;
    }
    free(collisions.items);

    e->velocity.y = MIN(100.0f, e->velocity.y + (gravity * deltaTime));
    // p->velocity.y = p->velocity.y + (gravity * deltaTime);

    keepPlayerInbound(e->boundingBox, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // behaviour_func(e, player, projectiles, objects, deltaTime);
}

void updateEntities(entities *entities, player_t *player, bullets *projectiles, objs *objects, float deltaTime)
{
    da_foreach (entity_t *, e, entities) {
        updateEntity((*e), player, projectiles, objects, deltaTime);
    }
}

void renderEntity(entity_t *e)
{
    SDL_FlipMode flip = (getAngle(e) >= 180.0f) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderTextureRotated((*e->ctx)->renderer, e->tex, NULL, e->boundingBox, 0.0f, NULL, flip);
}

void renderEntities(entities *entities)
{
    da_foreach (entity_t *, e, entities) {
        renderEntity(*e);
    }
}

void destroyEntity(entity_t **e)
{
    if (*e != NULL) {
        // Bounding box
        free((*e)->boundingBox);
        (*e)->boundingBox = NULL;

        // Texture
        SDL_DestroyTexture((*e)->tex);
        (*e)->tex = NULL;
    }

    free(*e);
    *e = NULL;
}

void destroyEntities(entities *entities)
{
    da_foreach (entity_t *, e, entities) {
        destroyEntity(e);
        *e = NULL;
    }

    free(entities->items);
}

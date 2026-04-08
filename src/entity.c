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
#include "player.h"

static struct entityBaseAttributs {
    entity_type type;
    entity_attributs stats;
    V2f size;
} baseStats[E_TYPE_COUNT] = {
    {.type = E_FILTH, .stats = {.maxHP = 0.5f, .entity_speed = 300.0f}, .size = (V2f){60, 100}},
    {.type = E_STRAY, .stats = {}, .size = (V2f){80, 140}},
    {.type = E_SWORDSMACHINE, .stats = {}, .size = (V2f){100, 180}},
    {.type = E_PROVIDENCE, .stats = {}, .size = (V2f){128, 128}},
    {.type = E_VERTU, .stats = {}, .size = (V2f){128, 128}},
    {.type = E_MAURICE, .stats = {}, .size = (V2f){118, 128}},
    {.type = E_MINOS_PRIME, .stats = {}, .size = (V2f){120, 180}},
    {.type = E_SISYPHUS, .stats = {}, .size = (V2f){140, 200}},
};

SDL_Texture *entity_textures[E_TYPE_COUNT] = {0};

void loadEntityTex(sdl_ctx_t *ctx)
{
    // See TODO(2026-03-30 08:14:05)
    // See TODO(2026-03-30 08:14:40)
    const char *texPaths[E_TYPE_COUNT] = {
        "./assets/img/filth.png",
    };

    for (size_t i = 0; i < ARRAY_LEN(texPaths); ++i) {
        entity_textures[i] = IMG_LoadTexture(ctx->renderer, texPaths[i]);
    }
}

SDL_Texture *getEntityTex(sdl_ctx_t *ctx, int index)
{
    if (entity_textures[assert(index < E_TYPE_COUNT && index >= 0), index] == NULL) {
        loadEntityTex(ctx);
    }
    return entity_textures[index];
}

ennemy_t *createEntity(sdl_ctx_t **sdl_ctx, entity_type type, V2f basePos)
{
    // See TODO(2026-03-30 08:15:26)
    ennemy_t *e = calloc(1, sizeof(ennemy_t));
    if (e == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for an entity", __FILE__, __LINE__);
        return NULL;
    }

    e->entity_attribs.tex = getEntityTex(*sdl_ctx, type);
    e->entity_attribs.ctx = sdl_ctx;
    e->type = type;

    // Each entity has its own parameters
    // That it'd be the size of its bounding box, to each and every attribut defined
    switch (type) {
    case E_FILTH: {
        e->entity_attribs.boundingBox =
            createRect_Ex((SDL_FRect){basePos.x, basePos.y, baseStats[type].size.x, baseStats[type].size.y});
        setEntityAttributs(e, .entity_speed = 240, .maxHP = 15.0f);
        break;
    }
    case E_STRAY: {
        TODO("stray");
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
    case E_VERTU: {
        TODO("vertu");
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

void setMaxHP(ennemy_t *e, float maxHP)
{
    e->attributs.maxHP = maxHP;
}

void setHP(ennemy_t *e, float HP)
{
    e->attributs.hp = (HP >= e->attributs.maxHP) ? e->attributs.maxHP : HP;
}

void setEntitySpeed(ennemy_t *e, float speed)
{
    e->attributs.entity_speed = speed;
}

void _setEntityAttributs(ennemy_t *e, entity_attributs attrib)
{
    // See TODO(2026-03-30 08:17:02)
    if (attrib.entity_speed > 0) setEntitySpeed(e, attrib.entity_speed);

    if (attrib.maxHP > 0) setMaxHP(e, attrib.maxHP);

    if (attrib.hp > 0) setHP(e, attrib.hp);

    if (attrib.projectile_speed > 0) e->attributs.projectile_speed = attrib.projectile_speed;

    if (attrib.state >= 0) setEntityState(e, attrib.state);

    if (attrib.jumpForce > 0) e->attributs.jumpForce = attrib.jumpForce;
}

void setEntityState(ennemy_t *e, entity_state state)
{
    e->attributs.state = state;
}

entity_state getEntityState(ennemy_t *e)
{
    return e->attributs.state;
}

float getAngle(ennemy_t *e)
{
    return (e->velocity.x > 0) ? 180.0f : 0.0f;
}

objs collision_test_entity(ennemy_t *e, objs *tiles)
{
    objs collisions = {0};
    da_foreach (obj, tile, tiles) {
        if (SDL_HasRectIntersectionFloat(getBB(e), tile->boundingBox)) {
            da_append(&collisions, *tile);
        }
    }

    return collisions;
}

void updateEntity(ennemy_t *e, player_t *player, objs *objects, float deltaTime)
                  // void (*behaviour_func)(entity_t *, player_t *, objs *, float))
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

    keepRectInbounds(getBB(e), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // behaviour_func(e, player, projectiles, objects, deltaTime);
}

void updateEntities(entities *entities, player_t *player, objs *objects, float deltaTime)
{
    da_foreach (ennemy_t *, e, entities) {
        updateEntity((*e), player, objects, deltaTime);
    }
}

void renderEntity(ennemy_t *e)
{
    SDL_FlipMode flip = (getAngle(e) >= 180.0f) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderTextureRotated((*e->entity_attribs.ctx)->renderer, e->entity_attribs.tex, NULL, getBB(e), 0.0f, NULL, flip);
}

void renderEntities(entities *entities)
{
    da_foreach (ennemy_t *, e, entities) {
        renderEntity(*e);
    }
}

void destroyEntity(ennemy_t **e)
{
    if (*e != NULL) {
        // Bounding box
        free((*e)->entity_attribs.boundingBox);
        (*e)->entity_attribs.boundingBox = NULL;

        // Texture
        SDL_DestroyTexture((*e)->entity_attribs.tex);
        (*e)->entity_attribs.tex = NULL;
    }

    free(*e);
    *e = NULL;
}

void destroyEntities(entities *entities)
{
    da_foreach (ennemy_t *, e, entities) {
        destroyEntity(e);
        *e = NULL;
    }

    free(entities->items);
}
// TODO(2026-03-30 08:14:05): Create test for loading every image, when every image will be added
// TODO(2026-03-30 08:14:40): Change the entity_textures to take arrays of texture for when the animation part of the project
// comes into mind
// TODO(2026-03-30 08:15:26): Create test to test the creation of a valid entity, wrongly typed entity, with both bad and good
// position (test with keepInbounds)
// TODO(2026-03-30 08:17:02): Test exhausively this function

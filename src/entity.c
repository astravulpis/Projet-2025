/**
 * @file entity.c
 * @brief Implementation of the entity structures
 *
 * @author Liam B. <liam.berge72@gmail.com>
 * @date 2026-03-28
 *
 * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 **/

#include "entity.h"
#include "common.h"
#include "music.h"
#include "player.h"
#include "sdl_helpers.h"
#include <math.h>

#define PURSUIT_STOP_RANGE 50.0

static struct entityBaseAttributs {
    entity_type type;
    entity_attributs stats;
    V2f size;
    float detection_range;
} baseStats[__count_enemy_type] = {
    {.type = E_FILTH, .stats = {.maxHP = 0.5f, .entity_speed = 300.0f}, .size = (V2f){60, 100}, .detection_range = 200.0f},
    {.type = E_STRAY, .stats = {}, .size = (V2f){80, 140}, .detection_range = 150.0f},
    {.type = E_SWORDSMACHINE, .stats = {}, .size = (V2f){100, 180}, .detection_range = 180.0f},
    {.type = E_PROVIDENCE, .stats = {}, .size = (V2f){128, 128}, .detection_range = 250.0f},
    {.type = E_VERTU, .stats = {}, .size = (V2f){128, 128}, .detection_range = 200.0f},
    {.type = E_MAURICE, .stats = {}, .size = (V2f){118, 128}, .detection_range = 160.0f},
    {.type = E_MINOS_PRIME, .stats = {}, .size = (V2f){120, 180}, .detection_range = 200.0f},
    {.type = E_SISYPHUS, .stats = {}, .size = (V2f){140, 200}, .detection_range = 250.0f},
};

SDL_Texture *entity_textures[__count_enemy_type] = {0};

// 2 tracks per enemy:
//    - interact (e.g. hurt, attack, fall)
//    - death
// +1 for the spawn track
sfxs enemySfxs = {0};

void loadEntityTex(sdl_ctx_t *ctx)
{
    const char *texPaths[__count_enemy_type] = {
        "./assets/img/filth.png",
    };

    for (size_t i = 0; i < ARRAY_LEN(texPaths); ++i) {
        entity_textures[i] = IMG_LoadTexture(ctx->renderer, texPaths[i]);
    }
}

char *enemyDisplayName(entity_type type)
{
    switch (type) {
    case E_FILTH:
        return "filth";
    case E_STRAY:
        return "STRAY";
    case E_SWORDSMACHINE:
        return "SWORDSMACHINE";
    case E_PROVIDENCE:
        return "PROVIDENCE";
    case E_VERTU:
        return "VERTU";
    case E_MAURICE:
        return "MAURICE";
    case E_MINOS_PRIME:
        return "MINOS_PRIME";
    case E_SISYPHUS:
        return "SISYPHUS";
    default:
        break;
    }
    UNREACHABLE("enemy display name");
}

char *loadEnemySfx(ennemy_t *e, sdl_ctx_t *ctx, char *sfxName)
{
    // "spawn", "./assets/audio/SFX/spawn.ogg"
    // "death", "./assets/audio/SFX/die.ogg"
    static const char *WorldSfx[2] = {
        "./assets/audio/SFX/spawn.wav",
        "./assets/audio/SFX/die.ogg",
    };

    static const char *enemySfx[__count_enemy_type] = {
        "./assets/audio/SFX/filth.wav",
        "./assets/audio/SFX/stray.wav",
    };

    size_t mark = temp_save();
    if (enemySfxs.count == 0) {
        for (size_t i = 0; i < __count_enemy_type; ++i) {
            temp_rewind(mark);
            loadSfx(ctx, &enemySfxs, SFX_ENEMY_INTERACTIONS, temp_sprintf("%s%s", enemyDisplayName(e->type), sfxName),
                    enemySfx[e->type]);
        }
        loadSfx(ctx, &enemySfxs, SFX_ENEMY_SPAWN, "enemySpawn", WorldSfx[0]);
        loadSfx(ctx, &enemySfxs, SFX_ENEMY_DIE, "enemyDie", WorldSfx[1]);
    }

    return enemySfxs.items[e->type]->name;
}

SDL_Texture *getEntityTex(sdl_ctx_t *ctx, int index)
{
    if (entity_textures[assert(index < __count_enemy_type && index >= 0), index] == NULL) {
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
    e->attackSfx = loadEnemySfx(e, *sdl_ctx, "attack");
    e->direction = -1; // default movement direction
    e->entity_attribs.selectedGunIndex = 0;
    // Each entity has its own parameters
    // That it'd be the size of its bounding box, to each and every attribut defined
    switch (type) {
    case E_FILTH: {
        e->entity_attribs.boundingBox =
            createRect_Ex((SDL_FRect){basePos.x, basePos.y, baseStats[type].size.x, baseStats[type].size.y});
        setEntityAttributs(e, .entity_speed = 240, .maxHP = 15.0f, .score = 20, .detection_range = 300.0f);
        break;
    }
    case E_STRAY: {
        e->entity_attribs.boundingBox =
            createRect_Ex((SDL_FRect){basePos.x, basePos.y, baseStats[type].size.x, baseStats[type].size.y});
        setEntityAttributs(e, .entity_speed = 120, .maxHP = 25.0f, .score = 30,
                           .detection_range = 500.0f); // need to scale this at one point
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

    if (attrib.score > 0) e->attributs.score = attrib.score;

    if (attrib.detection_range > 0) e->attributs.detection_range = attrib.detection_range;
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

void updateEntity(ennemy_t *e, player_t *player, objs *objects, float deltaTime, sdl_ctx_t *ctx, bullets *bullet_array,
                  Guns_t *guns)
                  // void (*behaviour_func)(entity_t *, player_t *, objs *, float))
{
    assert(e->entity_attribs.boundingBox != NULL);
    float xAxis = pow(player->entity_attribs.boundingBox->x - e->entity_attribs.boundingBox->x, 2);
    float yAxis = pow(player->entity_attribs.boundingBox->y - e->entity_attribs.boundingBox->y, 2);
    float distanceToPlayer = sqrt(xAxis + yAxis);
    printf("current distance between the entity and the player: %f \n", distanceToPlayer);
    UNUSED(player);
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
    switch (e->attributs.state) {
    case STATE_IDLE:
        printf("NOT in hot pursuit\n");

        enemyIdle(e, objects);
        if (distanceToPlayer < e->attributs.detection_range) {
            setEntityState(e, STATE_PURSUING);
            printf("starting pursuit\n");
        }
        break;

    case STATE_PURSUING:
        printf("in hot pursuit\n");
        switch (e->type) {
        case E_FILTH: {
            if (distanceToPlayer < e->attributs.detection_range + PURSUIT_STOP_RANGE) {
                if (player->entity_attribs.boundingBox->x < e->entity_attribs.boundingBox->x) {
                    e->entity_attribs.boundingBox->x -= 5.0;
                } else {
                    e->entity_attribs.boundingBox->x += 5.0;
                }
            } else {
                setEntityState(e, STATE_IDLE);
            }
        } break;
        case E_STRAY: {
            if (distanceToPlayer < e->attributs.detection_range + PURSUIT_STOP_RANGE) {
                if (lineOfSight(objects, player, e)) {
                    V2f ennemy_pos = {e->entity_attribs.boundingBox->x, e->entity_attribs.boundingBox->y};
                    shootGun(ctx, &guns->arsenal[e->entity_attribs.selectedGunIndex], bullet_array, ennemy_pos,
                             frame_movement); // cast to V2f idk how this works anymore
                }
            }
        } break;
        default:
            UNREACHABLE("enemy type");
        }

        break;
    }
    keepRectInbounds(getBB(e), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // behaviour_func(e, player, projectiles, objects, deltaTime);
}

void playEnemySpawning(sdl_ctx_t *ctx)
{
    playSfx(ctx, &enemySfxs, "enemySpawn");
}

void playEnemyDeath(sdl_ctx_t *ctx)
{
    playSfx(ctx, &enemySfxs, "enemyDie");
}

void updateEntities(entities *entities, player_t *player, objs *objects, float deltaTime, sdl_ctx_t *ctx, bullets *bullet_array,
                    Guns_t *guns)
{
    UNUSED(player);
    if (entities != NULL) {
        da_foreach (ennemy_t *, e, entities) {
            updateEntity((*e), player, objects, deltaTime, ctx, bullet_array, guns);
        }
    }
}

void renderEntity(ennemy_t *e)
{
    SDL_FlipMode flip = (getAngle(e) >= 180.0f) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderTextureRotated((*e->entity_attribs.ctx)->renderer, e->entity_attribs.tex, NULL, getBB(e), 0.0f, NULL, flip);
}

void renderEntities(entities *entities)
{
    if (entities != NULL) {
        da_foreach (ennemy_t *, e, entities) {
            renderEntity(*e);
        }
    }
}

void destroyEntity(ennemy_t **e)
{
    if (*e != NULL) {
        // Bounding box
        free((*e)->entity_attribs.boundingBox);
        (*e)->entity_attribs.boundingBox = NULL;
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

    for (int i = 0; i < __count_enemy_type; ++i) {
        SDL_DestroyTexture(entity_textures[i]);
    }
    destroySfxs(&enemySfxs);

    free(entities->items);
}

void enemyIdle(ennemy_t *e, objs *objects)
{
    float accel = 0.3f;
    float maxSpeed = 2.0f;
    e->velocity.x += accel * e->direction;

    if (e->velocity.x > maxSpeed) e->velocity.x = maxSpeed;
    if (e->velocity.x < -maxSpeed) e->velocity.x = -maxSpeed;

    e->entity_attribs.boundingBox->x += e->velocity.x;

    objs collisions = collision_test_entity(e, objects);

    if (collisions.count > 0 || e->entity_attribs.boundingBox->x < 0 || e->entity_attribs.boundingBox->x > WINDOW_WIDTH) {
        e->entity_attribs.boundingBox->x -= e->velocity.x;
        e->direction *= -1;
        e->velocity.x = 0;
    }
}

bool lineOfSight(objs *objects, player_t *player, ennemy_t *e)
{
    UNUSED(objects);
    UNUSED(player);
    UNUSED(e);
    return true;
}
// TODO(2026-03-30 08:15:26): Create test to test the creation of a valid entity, wrongly typed entity, with both bad and good
// position (test with keepInbounds)
// TODO(2026-03-30 08:17:02): Test exhausively this function

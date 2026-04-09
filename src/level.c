#include "level.h"
#include "common.h"
#include "entity.h"
#include "player.h"
#include "triggers.h"

room_t *createRoom(int id)
{
    room_t *r = calloc(1, sizeof(room_t));
    if (r == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for a `room_t`", __FILE__, __LINE__);
        return NULL;
    }

    r->roomID = (assert(id >= 0 && "`id` is below 0, which is undefined behaviour"), id);
    // memset(&r->structures, 0, sizeof(objs)); // Delete the trash values in the structures
    // memset(&r->e_waves, 0, sizeof(entities) * MAX_WAVE_COUNT); // Delete the trash values in the waves
    // memset(&r->startPos, 0, sizeof(V2f)); // set start pos at 0, 0

    return r;
}

void loadRoom(level_t *level, int id)
{
    bool hasRoom = false;
    da_foreach (room_t *, room, level) {
        if ((*room)->roomID == id) {
            hasRoom = true;
            break;
        }
    }

    if (hasRoom) {
        level->currentLoadedRoomID = id;
    }
}

void renderRoom(sdl_ctx_t *ctx, level_t *level)
{
    if (level->count > 0 && level->currentLoadedRoomID <= level->count - 1) {
        room_t *currRoom = getLoadedRoom(level);
        da_foreach (obj, it, &currRoom->structures) {
            renderImage(ctx, it->texture, it->boundingBox);
        }

        da_foreach (ennemy_t *, e, getCurrentEntityWave(level)) {
            renderEntity(*e);
        }
    }
}

objs *getRoomObjects(level_t *level)
{
    return &level->items[level->currentLoadedRoomID]->structures;
}

room_t *getLoadedRoom(level_t *level)
{
    return level->items[level->currentLoadedRoomID];
}

entities *getCurrentEntityWave(level_t *level)
{
    return &level->items[level->currentLoadedRoomID]->e_waves[level->items[level->currentLoadedRoomID]->currWaveIdx];
}

level_t *createLevel(char *title, int id)
{
    level_t *level = calloc(1, sizeof(level_t));
    if (level == NULL) {
        nob_log(ERROR, "%s:%d: Failed to allocate space for a `level_t`", __FILE__, __LINE__);
        return NULL;
    }

    if (title == NULL || strcmp(title, "") == 0) {
        nob_log(ERROR, "%s:%d: level title is empty", __FILE__, __LINE__);
        free(level);
        return NULL;
    }

    level->title = strdup(title);
    level->levelID = (assert(id >= 0), id);
    level->currentLoadedRoomID = -1;

    return level;
}

void assignRoomToLevel(level_t *level, room_t *room)
{
    // Simple if statement to be sure each room added is continious to the next one
    if (level->count > 0 && level->items[level->count - 1]->roomID >= room->roomID) {
        room->roomID = level->items[level->count - 1]->roomID + 1;
    }
    da_append(level, room);
}

void assignObject_Ex(room_t *room, obj object)
{
    da_append(&room->structures, object);
}

void assignObject(room_t *room, sdl_ctx_t *ctx, const char *path, float x, float y, float w, float h)
{
    obj_create(&room->structures, ctx, path, x, y, w, h);
}

entity_type typeDisplayName(String_View sv)
{
    if (sv_eq(sv, sv_from_cstr("FILTH"))) return E_FILTH;
    if (sv_eq(sv, sv_from_cstr("STRAY"))) return E_STRAY;
    if (sv_eq(sv, sv_from_cstr("SWORDSMACHINE"))) return E_SWORDSMACHINE;
    if (sv_eq(sv, sv_from_cstr("PROVIDENCE"))) return E_PROVIDENCE;
    if (sv_eq(sv, sv_from_cstr("VERTU"))) return E_VERTU;
    if (sv_eq(sv, sv_from_cstr("MAURICE"))) return E_MAURICE;
    if (sv_eq(sv, sv_from_cstr("MINOS_PRIME"))) return E_MINOS_PRIME;
    if (sv_eq(sv, sv_from_cstr("SISYPHUS"))) return E_SISYPHUS;
    UNREACHABLE("display name entity type");
}

void assignEntityToWave(room_t *room, sdl_ctx_t **ctx, String_View name, V2f basePos, int wave_id)
{
    entity_type type = typeDisplayName(name);
    ennemy_t *e = createEntity(ctx, type, basePos);
    da_append(&room->e_waves[(assert(wave_id >= 0 && wave_id < MAX_WAVE_COUNT), wave_id)], e);
}

void destroyObjects(objs *objects)
{
    da_foreach (obj, it, objects) {
        free(it->boundingBox);
        it->boundingBox = NULL;

        SDL_DestroyTexture(it->texture);
        it->texture = NULL;
    }

    free(objects->items);
    objects = NULL;
}

void destroyRoom(room_t **room)
{
    if (*room != NULL) {
        destroyObjects(&(*room)->structures);
        destroyTriggers(&(*room)->triggers);
        for (int i = 0; i < MAX_WAVE_COUNT; ++i) {
            destroyEntities(&(*room)->e_waves[i]);
        }
    }

    free((*room));
    *room = NULL;
}

void destroyLevel(level_t **level)
{
    if (*level != NULL) {
        da_foreach (room_t *, room, (*level)) {
            destroyRoom(room);
        }

        free((*level)->title);
        (*level)->title = NULL;
    }

    // dynamic array's items attribut gets realloced right when you first fill it
    // Thus needing to be freed in consequence
    free((*level)->items);
    free(*level);
    *level = NULL;
}

void assignTriggerToRoom(room_t *room, trigger_t *trigger)
{
    da_append(&room->triggers, trigger);
}

triggers_t *getRoomTriggers(level_t *level)
{
    return &level->items[level->currentLoadedRoomID]->triggers;
}

void updateTrigger(level_t *level, player_t *p, trigger_t *trigger)
{
    if (hasEntityCollidedWithTrigger(trigger, &p->entity_attribs)) {
        switch (trigger->kind) {
        case PORTAL: {
            loadRoom(level, trigger->room_dst);
            movePlayer(p, trigger->newPos);
        } break;
        case ONESHOT: {
            deathTrigger(&p->entity_attribs);
        } break;
        case SPAWNER: {
            getLoadedRoom(level)->currWaveIdx = trigger->waveId;
        } break;
        default:
            break;
        }
    }
}

void updateTriggers(level_t *level, player_t *p)
{
    room_t *curr = getLoadedRoom(level);
    assert(curr != NULL && "update triggers: current room is not set");
    for (size_t i = 0; i < curr->triggers.count; ++i) {
        updateTrigger(level, p, curr->triggers.items[i]);
    }
}

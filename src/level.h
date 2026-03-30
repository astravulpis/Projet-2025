#ifndef LEVEL_H_
#define LEVEL_H_

#include "common.h"
#include "entity.h"

#define MAX_WAVE_COUNT 5

typedef struct {
    objs structures;
    entities e_waves[MAX_WAVE_COUNT];

    int roomID;
} room_t;

typedef struct {
    room_t **items;
    size_t count;
    size_t capacity;

    // Specific attributs to the level
    char *title;
    int levelID;
    int currentLoadedRoomID;
} level_t;

room_t *createRoom(int id);

void loadRoom(level_t *level, int id);

void renderRoom(sdl_ctx_t *ctx, level_t *level);

objs *getRoomObjects(level_t *level);

level_t *createLevel(char *title, int id);

void assignRoomToLevel(level_t *level, room_t *room);

void assignObject_Ex(room_t *room, obj object);

void assignObject(room_t *room, sdl_ctx_t *ctx, const char *path, float x, float y, float w, float h);

void assignEntityToWave(room_t *room, sdl_ctx_t *ctx, entity_type type, V2f basePos, int wave_id);

void destroyObjects(objs *objects);

void destroyRoom(room_t **room);

void destroyLevel(level_t **level);

#endif // LEVEL_H_

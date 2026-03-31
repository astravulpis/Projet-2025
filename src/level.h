#ifndef LEVEL_H_
#define LEVEL_H_

#include "common.h"
#include "entity.h"

#define MAX_WAVE_COUNT 5

/**
 * @struct room_t
 * @brief the room structure
 *
 * contains all the information related to a singular room like the list of objects within
 */
typedef struct {
    objs structures;
    entities e_waves[MAX_WAVE_COUNT];
    V2f startPos;
    int currWaveIdx;

    int roomID;
} room_t;

/**
 * @struct level_t
 * @brief level structure containing all its attributes and all the rooms inside
 */
typedef struct {
    room_t **items;
    size_t count;
    size_t capacity;

    // Specific attributs to the level
    char *title;
    int levelID;
    int currentLoadedRoomID;
} level_t;

/**
 * @fn createRoom(int id)
 * @param[in] id level ID
 * @brief creates a room
 * 
 * returns a fully allocated and set room_t structure
 */
room_t *createRoom(int id);

/**
 * @fn loadRoom(level_t *level, int id)
 * @param[in] level level structure
 * @param[in] id level ID
 * @brief loads a room
 * 
 * loads the room in the level with the corresponding ID
 */
void loadRoom(level_t *level, int id);

/**
 * @fn renderRoom(sdl_ctx_t *ctx, level_t *level)
 * @param[in] ctx our own sdl context variable
 * @param[in] level level structure
 * @brief renders a room
 * 
 * renders a room by going through all of its objects
 */
void renderRoom(sdl_ctx_t *ctx, level_t *level);

/**
 * @fn getRoomObjects(level_t *level)
 * @param[in] level level structure
 * @brief returns the list of all level objects
 */
objs *getRoomObjects(level_t *level);

/**
 * @fn getLoadedRoom(level_t *level)
 * @param[in] level level structure
 * @brief returns the currently loaded room
 */
room_t *getLoadedRoom(level_t *level);

/**
 * @fn getCurrentEntityWave(level_t *level)
 * @param[in] level level structure
 * @brief returns the ongoing wave
 */
entities *getCurrentEntityWave(level_t *level);

/**
 * @fn createLevel(char *title, int id)
 * @param[in] title level title
 * @param[in] id level ID
 * @brief creates and allocates a level
 */
level_t *createLevel(char *title, int id);

/**
 * @fn assignRoomToLevel(level_t *level, room_t *room)
 * @param[in] level level
 * @param[in] room current room with all objects
 * @brief assigns a room to a level
 */
void assignRoomToLevel(level_t *level, room_t *room);

/**
 * @fn assignObject_Ex(room_t *room, obj object)
 * @param[in] room current room with all objects
 * @param[in] object our own sdl context variable
 * @brief adds an object to a room
 */
void assignObject_Ex(room_t *room, obj object);

/**
 * @fn assignObject(room_t *room, sdl_ctx_t *ctx, const char *path, float x, float y, float w, float h)
 * @param[in] room current room with all objects
 * @param[in] ctx our own sdl context variable
 * @param[in] path path to the object
 * @param[in] x x pos
 * @param[in] x y pos
 * @param[in] w width
 * @param[in] h height
 * @brief assigns an object to a room
 * 
 * assigns an object to a room with it's texture, position, width and height
 */
void assignObject(room_t *room, sdl_ctx_t *ctx, const char *path, float x, float y, float w, float h);

/**
 * @fn assignEntityToWave(room_t *room, sdl_ctx_t **ctx, entity_type type, V2f basePos, int wave_id)
 * @param[in] room current room with all objects
 * @param[in] ctx our own sdl context variable
 * @param[in] type the type of the entity
 * @param[in] basePos default position in the level
 * @param[in] wave_id ID of the wave
 * @brief assings an entity to a wave
 */
void assignEntityToWave(room_t *room, sdl_ctx_t **ctx, entity_type type, V2f basePos, int wave_id);

/**
 * @fn destroyObjects(objs *objects)
 * @param[in] objects
 * @brief destroys an object
 * 
 * destorys all objects given in the array
 */
void destroyObjects(objs *objects);

/**
 * @fn destroyRoom(room_t **room)
 * @param[in] room
 * @brief destroys a room
 * 
 * destorys all elements of a room by going through every object
 */
void destroyRoom(room_t **room);

/**
 * @fn destroyLevel(level_t **level)
 * @param[in] level
 * @brief destroys a level
 * 
 * destorys all elements of a level by going through every room
 */
void destroyLevel(level_t **level);

#endif // LEVEL_H_

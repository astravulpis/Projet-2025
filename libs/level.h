#ifndef LEVEL_H_
#define LEVEL_H_

#include "common.h"
#include "entity.h"
#include "triggers.h"

#define MAX_WAVE_COUNT 5

/**
 * @struct room_t
 * @brief the room structure
 *
 * contains all the information related to a singular room like the list of objects within and the entity waves
 */
typedef struct {
    objs structures;
    entities e_waves[MAX_WAVE_COUNT];
    V2f startPos;
    triggers_t triggers;
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
    size_t levelID;
    size_t currentLoadedRoomID;
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
 * @fn renderRoom(sdl_ctx_t *ctx, room_t *room)
 * @param[in] ctx our own sdl context variable
 * @param[in] room The room to render
 * @brief renders a room
 *
 * renders a room by going through all of its objects and entity for the current wave of entity
 */
void renderRoom(sdl_ctx_t *ctx, room_t *room);

/**
 * @fn getRoomObjects(room_t *room)
 * @param[in] room The room containing the objects
 * @brief helper function to return the list of all level objects
 */
objs *getRoomObjects(room_t *room);

/**
 * @fn getLoadedRoom(level_t *level)
 * @param[in] level level structure
 * @brief returns the currently loaded room
 */
room_t *getLoadedRoom(level_t *level);

/**
 * @fn getCurrentEntityWave(room_t *room)
 * @param[in] room The room of the wave
 * @brief helper function to return the ongoing wave of entities
 */
entities *getCurrentEntityWave(room_t *room);

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
 * assigns an object to a room with it's texture, position, width and height
 */
void assignObject(room_t *room, sdl_ctx_t *ctx, const char *path, float x, float y, float w, float h);

/**
 * @fn assignEntityToWave(room_t *room, sdl_ctx_t **ctx, entity_type type, V2f basePos, int wave_id)
 * @param[in] room current room with all objects
 * @param[in] ctx our own sdl context variable
 * @param[in] name Name of the type of the entity
 * @param[in] basePos default position in the level
 * @param[in] wave_id ID of the wave
 * @brief assings an entity to a wave
 */
void assignEntityToWave(room_t *room, sdl_ctx_t **ctx, String_View name, V2f basePos, int wave_id);

/**
 * @fn destroyObjects(objs *objects)
 * @param[in] objects
 * @brief destroys an object
 * destorys all objects given in the array
 */
void destroyObjects(objs *objects);

/**
 * @fn destroyRoom(room_t **room)
 * @param[in] room
 * @brief destroys a room
 * destorys all elements of a room by going through every object
 */
void destroyRoom(room_t **room);

/**
 * @fn destroyLevel(level_t **level)
 * @param[in] level
 * @brief destroys a level
 * destorys all elements of a level by going through every room
 */
void destroyLevel(level_t **level);

/**
 * @fn getRoomTriggers(level_t *level)
 * @param[in] level
 * @brief returns the list of all triggers within the given room
 */
triggers_t *getRoomTriggers(level_t *level);

/**
 * @fn assignTriggerToRoom(room_t *room, trigger_t *trigger)
 * @param[in] room The room in question
 * @param[in] trigger the trigger to assign
 * @brief Assigns a trigger to a room
 */
void assignTriggerToRoom(room_t *room, trigger_t *trigger);

/**
 * @fn updateTriggers(level_t *level, player_t *p)
 * @param[in] level The level that contains the triggers to update
 * @param[in] p The player that can interact with such triggers
 * @brief Assigns a trigger to a room
 */
void updateTriggers(level_t *level, player_t *p);

/**
 * @fn assignTriggerToRoom(room_t *room, trigger_t *trigger)
 * @param[in] room current room
 * @param[in] trigger trigger to be assigned to the room itself
 * @brief asigns a given trigger to the given room
 */
void assignTriggerToRoom(room_t *room, trigger_t *trigger);

/**
 * @fn updateTriggers(level_t *level, player_t *p)
 * @param[in] level current level
 * @param[in] p player pointer
 * @brief goes through the list of triggers in a level to update them if needed
 */
void updateTriggers(level_t *level, player_t *p);

/**
 * @fn updateTriggers(level_t *level, player_t *p)
 * @param[in] level current level
 * @param[in] p player pointer to update the position should it pass through a portal
 * @param[in] trigger the trigger to be checked for potential updates
 * @brief updates a trigger's state and executes the corresponding function
 */
void updateTrigger(level_t *level, player_t *p, trigger_t *trigger);

/**
 * @fn typeDisplayName(String_View sv)
 * @param[in] sv currently viewed string
 * @brief returns the type of the entity that corresponds to the given string
 */
entity_type typeDisplayName(String_View sv);

#endif // LEVEL_H_

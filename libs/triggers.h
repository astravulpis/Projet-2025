#ifndef TRIGGERS_H_
#define TRIGGERS_H_

#include "common.h"

typedef enum {
    PORTAL,
    ONESHOT,
    SPAWNER,
} trigger_kind;

typedef struct trigger_s trigger_t;

struct trigger_s {
    SDL_FRect *boundingBox;
    int waveId;              //!< Corresponding wave index
    size_t room_dst;
    bool toggled;
    V2f newPos;
    void (*trigger_func)(trigger_t *, int);
    trigger_kind kind;
};

typedef struct {
    trigger_t **items;
    size_t count;
    size_t capacity;
} triggers_t;

/**
 * @fn createTrigger(room_t *room, float x, float y, float w, float h, int waveID)
 * @param[in] room
 * @param[in] x X position
 * @param[in] y Y position
 * @param[in] w width
 * @param[in] h height
 * @param[in] waveID wave index to activate when trigger is hit
 * @brief creates a trigger
 *
 * creates a trigger in a room_t room without a texture like the boxes since we never need to see this
 */
trigger_t *createTrigger(float x, float y, float w, float h, trigger_kind kind);

/**
 * @fn renderTriggers(sdl_ctx_t *sdl_ctx, triggers_t *triggers)
 * @param[in] sdl_ctx Our sdl context
 * @param[in] triggers the triggers to render
 * @brief renders all of the current room's triggers
 *
 * goes through triggers_t to render each individual trigger
 */
void renderTriggers(sdl_ctx_t *sdl_ctx, triggers_t *triggers);

/**
 * @fn destroyTriggers(triggers_t *triggers)
 * @param[in] triggers
 * @brief destroys all triggers
 *
 * goes through triggers_t to destory each individual trigger
 */
void destroyTriggers(triggers_t *triggers);

/**
 * @fn getTriggerKindFromSV(String_View sv)
 * @param[in] sv currently viewed string
 * @brief returns the kind of the trigger sent from file_parsing
 */
trigger_kind getTriggerKindFromSV(String_View sv);

/**
 * @fn hasEntityCollidedWithTrigger(trigger_t *trigger, entity_t *e)
 * @param[in] trigger trigger structure we are checking
 * @param[in] e entity structure we are checking as well
 * @brief returns true if the given entity is colliding with the given trigger
 */
bool hasEntityCollidedWithTrigger(trigger_t *trigger, entity_t *e);

/**
 * @fn deathTrigger(entity_t *e)
 * @param[in] e entity structure we are checking as well
 * @brief sets the entity's health to 0, effectively killing them
 */
void deathTrigger(entity_t *e);

/**
 * @fn destroyTrigger(trigger_t **trigger)
 * @param[in] trigger trigger structure
 * @brief destroys a trigger
 */
void destroyTrigger(trigger_t **trigger);


#endif // TRIGGERS_H_

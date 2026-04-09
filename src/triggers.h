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
 * @fn destroyTriggers(triggers_t *triggers)
 * @param[in] triggers
 * @brief destroys all triggers
 *
 * goes through triggers_t to destory each individual trigger
 */
void destroyTriggers(triggers_t *triggers);
trigger_kind getTriggerKindFromSV(String_View sv);
bool hasEntityCollidedWithTrigger(trigger_t *trigger, entity_t *e);
void deathTrigger(entity_t *e);

#endif // TRIGGERS_H_

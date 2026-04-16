#include "triggers.h"
#include "common.h"
#include "sdl_helpers.h"

trigger_t *createTrigger(float x, float y, float w, float h, trigger_kind kind)
{
    trigger_t *trigger = calloc(1, sizeof(trigger_t));
    if (trigger == NULL) {
        nob_log(ERROR, "Failed to allocated space for a trigger_t");
        return NULL;
    }

    trigger->boundingBox = createRect(x, y, w, h);
    trigger->kind = kind;
    trigger->toggled = false;
    return trigger;
}

void destroyTrigger(trigger_t **trigger)
{
    if (*trigger != NULL) {
        free((*trigger)->boundingBox);
        (*trigger)->boundingBox = NULL;
    }

    free(*trigger);
    *trigger = NULL;
}

void destroyTriggers(triggers_t *triggers)
{
    da_foreach (trigger_t *, trigger, triggers) {
        destroyTrigger(trigger);
    }
    free(triggers->items);
}

bool hasEntityCollidedWithTrigger(trigger_t *trigger, entity_t *e)
{
    if (SDL_HasRectIntersectionFloat(e->boundingBox, trigger->boundingBox)) {
        if (trigger->toggled) return false;
        if (trigger->kind == SPAWNER) trigger->toggled = true;
        return true;
    }
    return false;
}

void deathTrigger(entity_t *e)
{
    e->hp = -1.f;
    e->isAlive = false;
}

trigger_kind getTriggerKindFromSV(String_View sv)
{
    if (sv_eq(sv, sv_from_cstr("PORTAL"))) return PORTAL;
    if (sv_eq(sv, sv_from_cstr("ONESHOT"))) return ONESHOT;
    if (sv_eq(sv, sv_from_cstr("SPAWNER"))) return SPAWNER;
    UNREACHABLE(temp_sprintf("trigger type display name |" SV_Fmt "|", SV_Arg(sv)));
}

void renderTriggers(sdl_ctx_t *sdl_ctx, triggers_t *triggers)
{
    da_foreach (trigger_t *, trigger, triggers) {
        renderFillRect(sdl_ctx->renderer, (*trigger)->boundingBox, (SDL_Color){0xFF, 0x7F, 0x7f, 0x7F});
    }
}

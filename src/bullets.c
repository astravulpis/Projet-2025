#include "bullets.h"
#include "common.h"
#include "entity.h"
#include "level.h"
#include "player.h"
#include "sdl_helpers.h"

bool createBullet(bullets *arr, V2f init_pos, V2f vel, int height, int width, SDL_Color color)
{
    bullet projectile = {0};
    projectile.boundingBox = createRect(init_pos.x, init_pos.y, height, width);
    projectile.velocity.x = vel.x;
    projectile.velocity.y = vel.y;
    projectile.color = color;

    da_append(arr, projectile);

    return true;
}

bool checkCollision(bullet *bullet, level_t *level)
{
    room_t *currRoom = level->items[level->currentLoadedRoomID];
    da_foreach (obj, tile, &currRoom->structures) {
        if (SDL_HasRectIntersectionFloat(getBB(bullet), getBB(tile))) {
            return true;
        }
    }

    da_foreach (entity_t *, entity, &currRoom->e_waves[currRoom->currWaveIdx]) {
        if (SDL_HasRectIntersectionFloat(getBB(bullet), getBB(*entity))) {
            return true;
        }
    }

    return false;
}

void updateBulletState(bullets *arr, level_t *level, float deltaTime)
{
    da_foreach (bullet, it, arr) {
        getBB(it)->x += it->velocity.x * deltaTime;
        getBB(it)->y += it->velocity.y * deltaTime;
    }

    size_t i = 0;
    while (i < arr->count) {
        bullet *it = &arr->items[i];
        if ((getBB(it)->x < -64 || getBB(it)->x >= WINDOW_WIDTH * 2) ||
            (getBB(it)->y < -64 || getBB(it)->y >= WINDOW_HEIGHT * 2) || checkCollision(it, level)) {
            deleteBullet(&it);
            da_remove_unordered(arr, i);
            i -= 1;
        }
        i += 1;
    }
}

void renderBullets(sdl_ctx_t *ctx, bullets *arr)
{
    da_foreach (bullet, it, arr) {
        renderFillRect(ctx->renderer, it->boundingBox, it->color);
    }
}

void deleteBullet(bullet **bullet)
{
    if (*bullet != NULL) free((*bullet)->boundingBox);
}

void deleteBullets(bullets *arr)
{
    da_foreach (bullet, it, arr) {
        deleteBullet(&it);
    }
    free(arr->items);
}

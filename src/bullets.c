#include "bullets.h"
#include "common.h"
#include "player.h"
#include "sdl_helpers.h"

bool createBullet(bullets *arr, V2f init_pos, V2f vel)
{
    bullet projectile = {0};
    projectile.boundingBox = createRect(init_pos.x, init_pos.y, 20, 20);
    projectile.velocity.x = vel.x;
    projectile.velocity.y = vel.y;

    da_append(arr, projectile);

    return true;
}

void updateBulletState(bullets *arr, float deltaTime)
{
    da_foreach (bullet, it, arr) {
        getBB(it)->x += it->velocity.x * deltaTime;
        getBB(it)->y += it->velocity.y * deltaTime;
    }

    size_t i = 0;
    while (i < arr->count) {
        bullet *it = &arr->items[i];
        if ((getBB(it)->x < -64 || getBB(it)->x >= WINDOW_WIDTH * 2) ||
            (getBB(it)->y < -64 || getBB(it)->y >= WINDOW_HEIGHT * 2)) {
            da_remove_unordered(arr, i);
            i -= 1;
        }
        i += 1;
    }
}

void renderBullets(sdl_ctx_t *ctx, bullets *arr)
{
    da_foreach (bullet, it, arr) {
        renderFillRect(ctx->renderer, it->boundingBox, (SDL_Color){0xFF, 0x00, 0x00, 0xFF});
    }
}

void deleteBullets(bullets *arr)
{
    da_foreach (bullet, it, arr) {
        free(it->boundingBox);
    }
    free(arr->items);
}

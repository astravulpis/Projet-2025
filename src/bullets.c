#include "bullets.h"
#include "common.h"
#include "entity.h"
#include "player.h"
#include "sdl_helpers.h"

bool createBullet(bullets *arr, V2f init_pos, V2f vel, int size, SDL_Texture *texture, float dmg)
{
    bullet projectile = {0};
    projectile.boundingBox = createRect(init_pos.x, init_pos.y, size, size);
    projectile.velocity.x = vel.x;
    projectile.velocity.y = vel.y;
    projectile.texture = texture;
    projectile.dmg = dmg;
    projectile.whoShot = 0;

    da_append(arr, projectile);

    return true;
}

bool checkCollision(bullet *bullet, objs *objects, player_t *p, entity_t **entities, size_t count)
{
    da_foreach (obj, tile, objects) {
        if (SDL_HasRectIntersectionFloat(bullet->boundingBox, tile->boundingBox)) {
            return true;
        }
    }

    if (entities != NULL) {
        for (size_t idx = 0; idx < count; idx++) {
            entity_t *e = entities[idx];
            if (!bullet->whoShot) {
                if (e->isAlive) {
                    if (SDL_HasRectIntersectionFloat(bullet->boundingBox, e->boundingBox)) {
                        e->hp -= bullet->dmg;
                        deleteBullet(&bullet);
                        if (e->hp <= 0) {
                            playEnemyDeath(*e->ctx);
                            p->score += e->score;
                            e->isAlive = false;
                        }
                        return true;
                    }
                }
            } else {
                if (SDL_HasRectIntersectionFloat(bullet->boundingBox, p->entity_attribs.boundingBox)) {
                    p->entity_attribs.hp -= bullet->dmg;
                    return true;
                }
            }
        }
    }
    return false;
}

// internal function
bool isInbounds(SDL_FRect *rect)
{
    return (rect->x < -64 || rect->x >= WINDOW_WIDTH * 2) || (rect->y < -64 || rect->y >= WINDOW_HEIGHT * 2);
}

void updateBulletStatePlayer(bullets *arr, objs *objects, entity_t **entities, size_t count, player_t *p, float deltaTime)
{
    size_t i = 0;
    while (i < arr->count) {
        bullet *it = &arr->items[i];
        it->boundingBox->x += it->velocity.x * deltaTime;
        it->boundingBox->y += it->velocity.y * deltaTime;
        if (isInbounds(it->boundingBox) || checkCollision(it, objects, p, entities, count)) {
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
        renderImage(ctx, it->texture, it->boundingBox);
    }
}

void deleteBullet(bullet **bullet)
{
    if (*bullet != NULL) {
        free((*bullet)->boundingBox);
        (*bullet)->boundingBox = NULL;
    }
}

void deleteBullets(bullets *arr)
{
    da_foreach (bullet, it, arr) {
        deleteBullet(&it);
    }
    free(arr->items);
}

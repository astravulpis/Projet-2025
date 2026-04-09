#include "bullets.h"
#include "common.h"
#include "entity.h"
#include "level.h"
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

    da_append(arr, projectile);

    return true;
}

bool checkCollision(bullet *bullet, level_t *level, player_t *p)
{
    room_t *currRoom = level->items[level->currentLoadedRoomID];
    da_foreach (obj, tile, &currRoom->structures) {
        if (SDL_HasRectIntersectionFloat(bullet->boundingBox, tile->boundingBox)) {
            return true;
        }
    }
    entities *s = &currRoom->e_waves[currRoom->currWaveIdx];
    for (size_t i = 0; i < s->count; i++) {
        ennemy_t *entity = s->items[i];
        if (SDL_HasRectIntersectionFloat(bullet->boundingBox, getBB(entity))) {
            entity->entity_attribs.hp -= bullet->dmg;
            if (entity->entity_attribs.hp <= 0) {
                playSfx(*entity->entity_attribs.ctx, &entity->audios, "death");
                p->score += entity->attributs.score;
                da_remove_unordered(s, i);
                destroyEntity(&entity);
            }
            return true;
        }
    }

    return false;
}

void updateBulletState(bullets *arr, level_t *level, float deltaTime, player_t *p)
{
    da_foreach (bullet, it, arr) {
        it->boundingBox->x += it->velocity.x * deltaTime;
        it->boundingBox->y += it->velocity.y * deltaTime;
    }

    size_t i = 0;
    while (i < arr->count) {
        bullet *it = &arr->items[i];
        if ((it->boundingBox->x < -64 || it->boundingBox->x >= WINDOW_WIDTH * 2) ||
            (it->boundingBox->y < -64 || it->boundingBox->y >= WINDOW_HEIGHT * 2) || checkCollision(it, level, p)) {
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
    if (*bullet != NULL) free((*bullet)->boundingBox);
}

void deleteBullets(bullets *arr)
{
    da_foreach (bullet, it, arr) {
        deleteBullet(&it);
    }
    free(arr->items);
}

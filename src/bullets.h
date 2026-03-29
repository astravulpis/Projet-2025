#include "common.h"

#ifndef BULLETS_H_
#define BULLETS_H_

/**
 * @struct bullet
 * @brief bullet bounding box and current velocity
 *
 * individual bullet information
 */
typedef struct {
    SDL_FRect *boundingBox;
    V2f velocity;
} bullet;

/**
 * @struct bullets
 * @brief list of all bullets
 *
 * contains all bullets with count and capacity
 */
typedef struct {
    bullet *items;
    size_t count;
    size_t capacity;
} bullets;

/**
 * @fn createBullet(bullets *arr, V2f init_pos, V2f vel)
 * @param[in] arr pointer to the bullets array
 * @param[in] init_pos X and Y origin point
 * @param[in] vel bullet speed
 * @brief creates bullet and puts it in the bullets array
 *
 * creates one bullet by putting in the dynamic array with its corresponding position and velocity
 */
bool createBullet(bullets *arr, V2f init_pos, V2f vel);

/**
 * @fn updateBulletState(bullets *arr, float deltaTime)
 * @param[in] arr pointer to the bullets array
 * @param[in] deltaTime X update interval
 * @brief updates bullet position
 *
 * moves all bullets according to delta time
 */
void updateBulletState(bullets *arr, float deltaTime);

/**
 * @fn checkBulletLevelCollisions(bullets *arr, objs *level)
 * @param[in] arr pointer to the bullets array
 * @param[in] level pointer to the level objects
 * @brief checks collisions between bullets and level pieces
 *
 * goes through all the bullets and removes any that are colliding with elements of the level
 */
void checkBulletLevelCollisions(bullets *arr, objs *level);

/**
 * @fn renderBullets(sdl_ctx_t *ctx, bullets *arr)
 * @param[in] ctx our own sdl context variable
 * @param[in] arr bullets array
 * @brief renders all bullets
 *
 * moves through the bullets array to render each individual one as a simple coloured rectangle
 */
void renderBullets(sdl_ctx_t *ctx, bullets *arr);

void deleteBullet(bullet **bullet);
/**
 * @fn deleteBullets(bullets *arr)
 * @param[in] arr bullets array
 * @brief detroyes all bullets
 *
 * moves through the bullets array to destroy each individual one to avoid leaks
 */
void deleteBullets(bullets *arr);

#endif // BULLETS_H

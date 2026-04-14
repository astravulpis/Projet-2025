#ifndef BULLETS_H_
#define BULLETS_H_

#include "common.h"
#include "level.h"

/**
 * @struct bullet
 * @brief bullet bounding box and current velocity with their texture and damage
 *
 * individual bullet information
 */

/**
 * @fn createBullet(bullets *arr, V2f init_pos, V2f vel)
 * @param[in] arr pointer to the bullets array
 * @param[in] init_pos X and Y origin point
 * @param[in] vel bullet speed
 * @param[in] size bullet size
 * @param[in] texture bullet texture
 * @param[in] dmg bullet damage
 * @brief creates bullet and puts it in the bullets array
 *
 * appends the bullet into the bullets array, gives it the corresponding damage, texture, speed and hitbox
 * that are dependent on the gun that shot said bullet
 */
bool createBullet(bullets *arr, V2f init_pos, V2f vel, int size, SDL_Texture *texture, float dmg);

/**
 * @fn updateBulletState(bullets *arr, float deltaTime)
 * @param[in] arr pointer to the bullets array
 * @param[in] deltaTime X update interval
 * @param[in] level pointer to the level objects
 * @param[in] p pointer to the player
 * @brief updates bullet position
 *
 * goes through the list of all bullets to updat their positions.
 * then it goes back through said list to check for collisions between the bullets and everything else 
 * by calling the \ref checkCollision function
 */
void updateBulletState(bullets *arr, level_t *level, float deltaTime, player_t *p);

/**
 * @fn checkCollision(bullets *arr, objs *level)
 * @param[in] arr pointer to the bullets array
 * @param[in] level pointer to the level objects
 * @param[in] p pointer to the player to pppass in update bullet state
 * @brief checks collisions between bullets and level pieces
 *
 * goes through all the bullets and checks for collisions against the level's structures (aka objects)
 * then does so for all the ennemies based on the current wave index. If there is a collision with an enemy,
 * the enemy's hp is reduced by the bullet's damage and if it reaches 0, the enemy is killed and the player score is increased by the enemy's score value
 */
bool checkCollision(bullet *bullet, level_t *level, player_t *p);

/**
 * @fn renderBullets(sdl_ctx_t *ctx, bullets *arr)
 * @param[in] ctx our own sdl context variable
 * @param[in] arr bullets array
 * @brief renders all bullets
 *
 * moves through the bullets array to render each individual one with it's texture and hitbox
 */
void renderBullets(sdl_ctx_t *ctx, bullets *arr);

/**
 * @fn deleteBullets(bullets *arr)
 * @param[in] bullet Address of the bullet to delete
 * @brief detroys the bullet
 * 
 * frees the bullet's hitbox, not the texture however as it's handled by \ref SDL_DestroyRenderer
 */
void deleteBullet(bullet **bullet);

/**
 * @fn deleteBullets(bullets *arr)
 * @param[in] arr bullets array
 * @brief detroyes all bullets
 *
 * moves through the bullets array to destroy each individual one to avoid leaks by calling the \ref deleteBullet function
 */
void deleteBullets(bullets *arr);

#endif // BULLETS_H

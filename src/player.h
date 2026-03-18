#ifndef PLAYER_H_
#define PLAYER_H_

#include "common.h"

typedef struct {
    sdl_ctx_t **ctx;        //!< Address of our sdl context to be saved
    SDL_FRect *boundingBox; //!< Player's BB
    SDL_Texture *tex;       //!< Player's texture
    float speed;            //!< Value may depend on preference
    bool onGround;          //!< State to tell whenever the player is on the ground or not
    // bool stunned;           //!< State used to avoid key mashing
    float stunnedTimer;     //!< Float used as a timer for the amount of time stunned
} player_t;

bool createPlayer(player_t **player, V2f playerSize, sdl_ctx_t **sdl_ctx, const char *path);
void destroyPlayer(player_t **p);
void UpdatePlayer(player_t *p, SDL_FRect *objects, int object_count, float deltaTime);
void renderPlayer(player_t *p);

#endif // PLAYER_H_

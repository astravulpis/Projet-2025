#include "SDL3/SDL_events.h"
#include "common.h"
#include "../shared.h"
#include "event.h"
#include "sdl_helpers.h"

/**
 * @file main.c
 * @brief File where every actions to run the game are being executed at.
 */

int main()
{
    sdl_ctx_t *sdl_ctx = init_all();
    if (sdl_ctx == NULL)
        exit(1);

    int direction = 0;
    float speed = 200.0f;
    float i=0;
    float color = 0x18/255.0f;
    Uint32 last = SDL_GetTicks();
    SDL_FRect * boxSDL = NULL;//les objets SDL_FRect contienent des coordonnées et des dimmensions
    SDL_Surface *surfaceImgSDL;
    SDL_Texture *textureImgSDL;

    sdl_ctx->quit = false;

    initBackgroundColor(sdl_ctx, 255, 255, 255);
    renderBackground(sdl_ctx);

    //Rendu du logo de SDL
    boxSDL=malloc(sizeof(SDL_FRect));
    boxSDL->x=0;
    boxSDL->y=0;
    boxSDL->w=100;
    boxSDL->h=100;

    surfaceImgSDL = SDL_LoadBMP("assets/img/SDL3.bmp");
    textureImgSDL = SDL_CreateTextureFromSurface(sdl_ctx->renderer, surfaceImgSDL);

    SDL_RenderTexture(sdl_ctx->renderer, textureImgSDL, NULL,  boxSDL);
    SDL_RenderPresent(sdl_ctx->renderer);

    //Rendu du logo du langage C
    SDL_FRect * boxC = NULL;
    boxC=malloc(sizeof(SDL_FRect));
    boxC->x=200;
    boxC->y=200;
    boxC->w=100;
    boxC->h=100;

    SDL_Surface *surfaceImgC;
    SDL_Texture *textureImgC;

    surfaceImgC = SDL_LoadBMP("assets/img/C.bmp");
    textureImgC = SDL_CreateTextureFromSurface(sdl_ctx->renderer, surfaceImgC);

    SDL_RenderTexture(sdl_ctx->renderer, textureImgC, NULL,  boxC);
    SDL_RenderPresent(sdl_ctx->renderer);
    //----------------------------------

    if (surfaceImgC==NULL)
        printf("impossible charger l'image du logo C ...\n");
    else
        printf("image chargée logo C avec succès (mais qui est succès ?) ...\n");

    if (surfaceImgSDL==NULL)
        printf("impossible charger l'image du logo SDL ...\n");
    else
        printf("image chargée logo SDL avec succès (mais qui est succès ?) ...\n");

    float tempOriginBox = boxC->x; //pour ne pas perdre la coordonnée x originelle de l'image logo C
    // ----------------------------------------------
    // * REMOVE THIS BELOW TO USE THE `tempOriginBox`
    // ----------------------------------------------
    UNUSED(tempOriginBox);

    while (!sdl_ctx->quit){
        Uint32 now = SDL_GetTicks();
        float deltaT = (now - last) / 1000.0f; // seconds since last frame
        last = now;

        while(SDL_PollEvent(&sdl_ctx->event)) {
            switch (sdl_ctx->event.type) {
                case SDL_EVENT_QUIT:
                    sdl_ctx->quit = true;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    basic_keyboard_events(sdl_ctx);
                    direction = basic_movements(sdl_ctx);
                    switch (direction){
                        case 'A':
                            boxC->x -= speed*deltaT;
                            break;
                        case 'S':
                            boxC->y += speed*deltaT;
                            break;
                        case 'D':
                            boxC->x += speed*deltaT;
                            break;
                        case 'W':
                            boxC->y -= speed*deltaT;
                            break;
                    }
                default:
                    break;
            }
        }

        // Updates the event queue and internal input device state
        SDL_PumpEvents();

        //call hit_box_test pour verifier que boxC ne sort pas de l'image avec la prochaine boucle
        keep_player_inbound(boxC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        //on récupère l'evenement en tête de file

        SDL_ClearSurface(sdl_ctx->bgSurface, color, color, color, 1.0f);
        SDL_RenderClear(sdl_ctx->renderer);

        updateBackgroundColor(sdl_ctx, i*(i/100), i*(i/100), i);
        renderBackground(sdl_ctx);
        SDL_RenderTexture(sdl_ctx->renderer, textureImgC, NULL, boxC);
        SDL_RenderTexture(sdl_ctx->renderer, textureImgSDL, NULL, boxSDL);

        SDL_RenderPresent(sdl_ctx->renderer);

    }

    free(boxSDL);
    free(boxC);

    SDL_DestroySurface(surfaceImgC);
    SDL_DestroySurface(surfaceImgSDL);

    SDL_DestroyTexture(textureImgC);
    SDL_DestroyTexture(textureImgSDL);

    close_SDL(sdl_ctx);

    return 0;
}

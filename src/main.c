#include "../shared.h"
#include "common.h"
#include "event.h"
#include "sdl_helpers.h"

/**
 * @file main.c
 * @brief File where every actions to run the game are being executed at.
 */

int main()
{
    int direction = 0;
    float speed = 200.0f;
    // float color = 0x18/255.0f;

    sdl_ctx_t *sdl_ctx = init_all();
    if (sdl_ctx == NULL) {
        nob_log(ERROR, "%s:%d: Failed to initialize context. See error backtrace above.", __FILE__, __LINE__);
        exit(1);
    }

    Uint32 last = SDL_GetTicks();
    float deltaT = 0;
    int frameCount = 0;
    float fps = 0;
    // Chemins absolue depuis la racine du projet. Cela se justifie
    // car le programme est tournee depuis `nob` qui est a la racine elle-meme.
    // loadBackgroundImage(sdl_ctx, "assets/img/bg.bmp");
    // if (!sdl_ctx->bgTexture) {
    //     nob_log(ERROR, "%s:%d: Failed to background image", __FILE__, __LINE__);
    //     return 1;
    // }

    SDL_Texture *SDL_Logo = chargerImage(sdl_ctx, "assets/img/SDL3.bmp");
    SDL_FRect *boxSDL = createRect(0.0f, 0.0f, 32.0f, 32.0f);
    if (!SDL_Logo) {
        nob_log(ERROR, "%s:%d: Failed to load SDL image", __FILE__, __LINE__);
        return 1;
    }

    SDL_Texture *C_Logo = chargerImage(sdl_ctx, "assets/img/C.bmp");
    SDL_FRect *boxC = createRect(350.0f, 200.0f, 100.0f, 100.0f);
    if (!C_Logo) {
        nob_log(ERROR, "%s:%d: Failed to load C logo image", __FILE__, __LINE__);
        return 1;
    }

    printf("\n");

    while (!sdl_ctx->quit) {
        while (SDL_PollEvent(&sdl_ctx->event)) {
            switch (sdl_ctx->event.type) {
            case SDL_EVENT_QUIT:
                sdl_ctx->quit = true;
                break;
            case SDL_EVENT_KEY_DOWN:
                basic_keyboard_events(sdl_ctx);
                direction = basic_movements(sdl_ctx);
                switch (direction) {
                case 'A':
                    boxC->x -= speed * deltaT;
                    break;
                case 'S':
                    boxC->y += speed * deltaT;
                    break;
                case 'D':
                    boxC->x += speed * deltaT;
                    break;
                case 'W':
                    boxC->y -= speed * deltaT;
                    break;
                }
            default:
                break;
            }
        }

        frameCount++;

        Uint32 now = SDL_GetTicks();
        deltaT = (now - last) / 1000.0f; // seconds since last frame
        if (now - frameCount >= 1000) {
            printf("\x1b[1FFPS : %.2f\n", fps);
            frameCount = 0;
            last = now;
        }

        // call hit_box_test pour verifier que boxC ne sort pas de l'image avec la prochaine boucle
        keep_player_inbound(boxC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        // on récupère l'evenement en tête de file

        // SDL_SetRenderDrawColorFloat(sdl_ctx->renderer, color, color, color, 1.0f);
        SDL_RenderClear(sdl_ctx->renderer);

        renderBackground(sdl_ctx);
        // renduImage(sdl_ctx, SDL_Logo, boxSDL->x, boxSDL->y, boxSDL->w, boxSDL->h);
        // renduImage(sdl_ctx, C_Logo, boxC->x, boxC->y, boxC->w, boxC->h);
        renduImage(sdl_ctx, SDL_Logo, boxSDL);
        renduImage(sdl_ctx, C_Logo, boxC);

        if (!SDL_RenderPresent(sdl_ctx->renderer)) {
            nob_log(ERROR, "%s:%d: Failed to render the renderer's buffer. See error: %s", __FILE__, __LINE__, SDL_GetError());
        }
    }

    SDL_DestroyTexture(SDL_Logo);
    SDL_DestroyTexture(C_Logo);
    free(boxC);
    free(boxSDL);

    close_SDL(sdl_ctx);
    return 0;
}

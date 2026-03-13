#include "../shared.h"
#include "SDL3/SDL_pixels.h"
#include "common.h"
#include "event.h"
#include "sdl_helpers.h"
#include <math.h>

#define WHITE (SDL_Color){255, 255, 255, 255}
#define BLACK (SDL_Color){0, 0, 0, 0}

/**
 * @file main.c
 * @brief File where every actions to run the game are being executed at.
 */
int main()
{
    float speed = 200.0f;
    float dx, dy;
    // float color = 0x18/255.0f;

    sdl_ctx_t *sdl_ctx = init_all();
    if (sdl_ctx == NULL) {
        nob_log(ERROR, "%s:%d: Failed to initialize context. See error backtrace above.", __FILE__, __LINE__);
        exit(1);
    }

    Uint32 last = SDL_GetTicks();
    float deltaT = 0;
    // int frameCount = 0;
    // float fps = 0;

    // Chemins absolue depuis la racine du projet. Cela se justifie
    // car le programme est tournee depuis `nob` qui est a la racine elle-meme.
    // loadBackgroundImage(sdl_ctx, "assets/img/bg.bmp");
    // if (!sdl_ctx->bgTexture) {
    //     nob_log(ERROR, "%s:%d: Failed to background image", __FILE__, __LINE__);
    //     return 1;
    // }

    // Test chargement du font Poppins
    TTF_Font *poppins_12pt = loadFont("./assets/font/Poppins/Poppins-Black.ttf", 150.0f, TTF_STYLE_NORMAL, 0);
    if (poppins_12pt == NULL) {
        printf("impossible de charger la police d'écriture ...\n");
        nob_log(ERROR, "%s:%d: Failed to load Poppins font", __FILE__, __LINE__);
        return 1;
    }

    // Couleur de la police
    SDL_FRect *boxDisplayFps = createRect(WINDOW_WIDTH / 2.0f - 256.0f, WINDOW_WIDTH / 2.0f, 300.0f, 128.0f);
    SDL_FRect *boxMouseCoord = createRect(WINDOW_WIDTH / 2.0f - 256.0f, WINDOW_WIDTH / 2.0f + 110.0f, 300.0f, 64.0f);

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

    // pointeur pour recevoir les coordonnées de la souris
    float mouse_X = 0;
    float mouse_Y = 0;

    // temps pour calculer le FPS
    Uint32 frameStart = 0;
    int frameCounter = 0;
    int frameRate = 0; // sert pour l'afichage, pour ne voir que le réel fps, pas pour voir frameCounter qui s'inécrémente et
                       // qui ne veut pas dire grand chose si il est afficher a chaque tour de while

    char *fpsDisplay = NULL;
    char *mouseCoordDisplay = NULL;

    size_t mark = temp_save();
    // Updates the event queue and internal input device state
    while (!sdl_ctx->quit) {
        temp_rewind(mark);
        Uint32 now = SDL_GetTicks();
        deltaT = (now - last) / 1000.0f; // seconds since last frame

        if (now - frameStart >= 1000) {
            // printf("\x1b[1FFPS : %d\n", frameCounter);
            frameRate = frameCounter;
            frameCounter = 0;
            frameStart = now;
        }

        while (SDL_PollEvent(&sdl_ctx->event)) {
            switch (sdl_ctx->event.type) {
            case SDL_EVENT_QUIT:
                sdl_ctx->quit = true;
                break;
            }
        }

        SDL_PumpEvents();

        dx = 0.0f;
        dy = 0.0f;
        // calls basic_movement with the adress of the image we are moving, it's X and Y coordinates
        basic_movement(sdl_ctx, &dx, &dy);

        if (dx != 0 && dy != 0) { // check for diagonal movement
            boxC->x += dx * speed * deltaT / sqrt(2);
            boxC->y += dy * speed * deltaT / sqrt(2);
        } else {
            // Apply movement without diagonal
            boxC->x += dx * speed * deltaT;
            boxC->y += dy * speed * deltaT;
        }

        keep_player_inbound(boxC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        // on récupère l'evenement en tête de file
        // SDL_SetRenderDrawColorFloat(sdl_ctx->renderer, color, color, color, 1.0f);
        SDL_RenderClear(sdl_ctx->renderer);

        renderBackground(sdl_ctx);

        renduImage(sdl_ctx, SDL_Logo, boxSDL);
        renduImage(sdl_ctx, C_Logo, boxC);

        fpsDisplay = temp_sprintf("fps : %i", frameRate);
        print_Sdl_Text(sdl_ctx, fpsDisplay, poppins_12pt, WHITE, boxDisplayFps);

        SDL_GetMouseState(&mouse_X, &mouse_Y);
        // x = 1920.0, y = 1080.0\0
        //                       ^ 22 chars
        mouseCoordDisplay = temp_sprintf("x = %.1f, y = %.1f", mouse_X, mouse_Y);
        print_Sdl_Text(sdl_ctx, mouseCoordDisplay, poppins_12pt, WHITE, boxMouseCoord);

        if (!SDL_RenderPresent(sdl_ctx->renderer)) {
            nob_log(ERROR, "%s:%d: Failed to render the renderer's buffer. See error: %s", __FILE__, __LINE__, SDL_GetError());
        }
        frameCounter++;
    }

    SDL_DestroyTexture(SDL_Logo);
    SDL_DestroyTexture(C_Logo);
    free(boxC);
    free(boxSDL);
    free(boxDisplayFps);
    free(boxMouseCoord);

    TTF_CloseFont(poppins_12pt);

    close_SDL(sdl_ctx);
    return 0;
}

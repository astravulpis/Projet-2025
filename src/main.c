#include "../shared.h"
#include "common.h"
#include "event.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "player.h"

/**
 * @file main.c
 * @brief File where every actions to run the game are being executed at.
 */
bool createImageRect();

int main()
{
    sdl_ctx_t *sdl_ctx = NULL;
    if (!createCtx(&sdl_ctx)) return 1; // Error handling is done in the function

    player_t *player = NULL;
    if (!createPlayer(&player, (V2f){100.0f, 100.0f}, &sdl_ctx, "assets/img/C.png")) return 1;

    Uint32 last = SDL_GetTicks();
    float deltaT = 0;

    size_t pixelWidth = 0;
    TTF_MeasureString(sdl_ctx->font, temp_sprintf("x = %d.0, y = %d.0", WINDOW_WIDTH, WINDOW_HEIGHT), 0, 0, NULL, &pixelWidth);
    V2f fpsTextPos = {10.0f, 10.0f};
    V2f MouseTextPos = {10.0f, 48.0f};
    SDL_FRect *boxDummy = createRect(400.0f, 400.0f, 50.0f, 50.0f);
    SDL_Texture *logoC = IMG_LoadTexture(sdl_ctx->renderer, "./assets/img/C.png");

    SDL_FRect *boxBouton1 = createRect(WINDOW_WIDTH/2-150, WINDOW_HEIGHT/2, 300.0f, 75.0f);
    V2f bouton1Pos = {boxBouton1->x+35, boxBouton1->y+20};

    printf("x = %.2f\ny = %.2f\nw = %.2f\nh = %.2f\n", boxBouton1->x, boxBouton1->y, boxBouton1->w, boxBouton1->h);

    if (logoC == NULL) {
        nob_log(ERROR, "%s:%d: Failed to load image. See error: %s", __FILE__, __LINE__, SDL_GetError());
        destroyPlayer(&player);
        closeCtx(&sdl_ctx);
        return 1;
    }


    float mouse_X = 0;
    float mouse_Y = 0;

    Uint32 frameStart = 0;
    int frameCounter = 0;
    int frameRate = 0;

    size_t mark = temp_save();

    // Updates the event queue and internal input device state
    while (!sdl_ctx->quit) {
        temp_rewind(mark);
        Uint32 now = SDL_GetTicks();
        deltaT = (now - last) / 1000.0f; // seconds since last frame
        last = now;

        if (now - frameStart >= 1000) {
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

        basicKeyboardEvents(sdl_ctx);
        UpdatePlayer(player, boxDummy, deltaT);

        SDL_GetMouseState(&mouse_X, &mouse_Y);//je l'ai bougé car il semble plus adapté ici
        SDL_RenderClear(sdl_ctx->renderer);
        renderBackground(sdl_ctx);
        
        //pour la zone de bouton c'est assez rapide de faire comme ça : on met une couleur de rendu, on rend dans la box bouton, avec la couleur mise
        //ensuite on remet en noir pour que le background soit noir a la prochaine itération
        SDL_SetRenderDrawColor(sdl_ctx->renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(sdl_ctx->renderer, boxBouton1);
        SDL_SetRenderDrawColor(sdl_ctx->renderer, 0, 0, 0, 255);

        //affichage du texte du boutton (dans la même box)
        renderText_Ex(sdl_ctx, temp_sprintf("bouton"), WHITE, bouton1Pos);
        

        renderPlayer(player);
        renderImage(sdl_ctx, logoC, boxDummy);

        renderText_Ex(sdl_ctx, temp_sprintf("Mouse: {%.1f, %.1f}", mouse_X, mouse_Y), WHITE, MouseTextPos);
        renderText_Ex(sdl_ctx, temp_sprintf("fps : %i", frameRate), WHITE, fpsTextPos);
        // renderText_Ex(sdl_ctx, temp_sprintf("timer: %.1f", player->stunnedTimer), WHITE, (V2f){10.0f, 148.0f});

        SDL_RenderPresent(sdl_ctx->renderer);
        frameCounter++;
    }

    free(boxDummy);

    destroyPlayer(&player);
    closeCtx(&sdl_ctx);
    return 0;
}

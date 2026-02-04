#include "SDL3/SDL.h"
#include "SDL3/SDL_surface.h"

#include "../shared.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static struct sdl_context_s {
    SDL_Window *window; //< SDL3 window context
    SDL_Surface *renderingSurface; //< SDL3 surface context
} sdl_context = {0};

/**
 * fonction qui initialiste tout les sous modules de SDL3 qui vont être utilisés
 */
bool init_all(void);

/**
 * @brief fonction qui effectue toutes les désallocation et mise a NULL des pointeurs SDL3
 * Detruit la fenetre et le context d'SDL3
 */
void close_SDL();


int main()
{
    init_all();

    bool quitterBool = false; //< booleen qui determine si la page doit continuer  s'afficher
    SDL_Event evenement;

    float color = 0x18/255.0f;

    while (!quitterBool){

        SDL_PollEvent(&evenement);
        switch (evenement.type){
            case SDL_EVENT_QUIT: quitterBool=true;
        }

        SDL_ClearSurface(sdl_context.renderingSurface, color, color, color, 1.0f);
        SDL_UpdateWindowSurface(sdl_context.window);
    }

    close_SDL();

    return 0;
}

bool init_all(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        return false;
    }

    sdl_context.window = SDL_CreateWindow("ULTRAC00L", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (sdl_context.window == NULL) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        close_SDL();
        return false;
    }

    sdl_context.renderingSurface = SDL_GetWindowSurface(sdl_context.window);
    return true;
}

void close_SDL()
{
    SDL_DestroyWindow(sdl_context.window);

    sdl_context.window = NULL;
    sdl_context.renderingSurface = NULL;

    SDL_Quit();
}

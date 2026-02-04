#include "SDL3/SDL.h"
#include "SDL3/SDL_surface.h"

#define NOB_IMPLEMENTATION
#include "../thirdparty/nob.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct sdl_context_s sdl_context_t;
static struct sdl_context_s {
    SDL_Window *window; //< SDL3 window context
    SDL_Surface *renderingSurface; //< SDL3 surface context
} sdl_context = {0};

SDL_Surface *WINDOW_IMAGE_TEST = NULL;

/**
 * fonction qui initialiste tout les sous modules de SDL3 qui vont être utilisés
 */
bool init_all(void);

/**
 * charge une image en recevant son chemin et un pointeur de type SDL_surface
 * @var chemin Chemin de l'image charger
 * @var surface Surface qui va recevoir l'image
 */
bool chargerImage(char *chemin, SDL_Surface *surface);

/**
 * @brief fonction qui effectue toutes les désallocation et mise a NULL des pointeurs SDL3
 * Detruit la fenetre et le context d'SDL3
 */
void close_SDL();


int main()
{
    init_all();

    // if(chargerImage("assets/img/SDL3.bmp", WINDOW_IMAGE_TEST)==true)
    //     printf("\n| chargement de l'image réussi\n");

    //met l'image dans la surface WINDOW_SURFACE_CONTEXT
    // SDL_BlitSurface(WINDOW_IMAGE_TEST, NULL, sdl_context.renderingSurface, NULL);

    bool quitterBool = false; //< booleen qui determine si la page doit continuer  s'afficher
    SDL_Event evenement;

    float rgb = 24.0f/255.0f;

    while ( !quitterBool ){

        SDL_PollEvent(&evenement);
        switch (evenement.type){
            case SDL_EVENT_QUIT: quitterBool=true;
        }

        SDL_ClearSurface(sdl_context.renderingSurface, rgb, rgb, rgb, 1.0f);
        // SDL_BlitSurface(WINDOW_IMAGE_TEST, NULL, sdl_context.renderingSurface, NULL);
        SDL_UpdateWindowSurface( sdl_context.window );
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

    sdl_context.window = SDL_CreateWindow("ULTRACOOL", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (sdl_context.window == NULL) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        close_SDL();
        return false;
    }

    sdl_context.renderingSurface = SDL_GetWindowSurface(sdl_context.window);
    return true;
}

bool chargerImage(char *chemin, SDL_Surface *surface)
{
    surface = SDL_LoadBMP(chemin);

    if (surface == NULL) {
        nob_log(ERROR, "Impossible de charger l'image %s! cause: %s\n", chemin, SDL_GetError());
        return false;
    }
    return true;
}

void close_SDL()
{
    SDL_DestroyWindow(sdl_context.window);

    WINDOW_IMAGE_TEST = NULL;
    sdl_context.window = NULL;
    sdl_context.renderingSurface = NULL;

    SDL_Quit();
}

#include "../thirdparty/SDL3/include/SDL3/SDL.h"
#include "../thirdparty/SDL3/include/SDL3/SDL_init.h"
#include "../thirdparty/SDL3/include/SDL3/SDL_error.h"
#include <SDL3/SDL_log.h>
#include <stdbool.h>

#define NOB_IMPLEMENTATION
#include "../thirdparty/nob.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//donnée de l'affichage
SDL_Window *WINDOW_GLOBAL = NULL;

//fonction qui initialiste tout les sous modules de SDL3 qui vont être utilisés
bool init_all(void){
        if ( !SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) ){
           nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        }
        return true;
}



int main()
{
    init_all();
    WINDOW_GLOBAL = SDL_CreateWindow("ULTRACOOL", WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if(WINDOW_GLOBAL == NULL) {
        nob_log(ERROR, "impossible de créer la fenêtre! cause : %s\n", SDL_GetError());
    return false;
    }


    bool quitterBool = false;
    SDL_Event evenement;

    while ( !quitterBool ){
        SDL_PollEvent(&evenement))
        switch (evenement.type){
            case SDL_EVENT_QUIT: quitterBool=true;
        }
    }

    SDL_DestroyWindow( WINDOW_GLOBAL );
    WINDOW_GLOBAL = NULL;
    SDL_Quit();
    return 0;
}

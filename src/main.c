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
SDL_Surface *WINDOW_SURFACE_GLOBAL = NULL;
SDL_Surface *WINDOW_IMAGE_TEST = NULL;

//fonction qui initialiste tout les sous modules de SDL3 qui vont être utilisés
bool init_all(void){
        if ( !SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) ){
           nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        }
        return true;
}

//charge une image en recevant son chemin et un pointeur de type SDL_surface, surface qui va recevoir l'image
bool chargerImage(char *chemin, SDL_Surface *surface){
   surface = SDL_LoadBMP(chemin);

   if( surface == NULL ){
       SDL_Log( "Impossible de charger l'image %s! cause: %s\n", chemin, SDL_GetError() );
       return false;
    }
   return true;
}

//fonction qui effectue toutes les désallocation et mise a NULL des pointeurs SDL3
void close_SDL(){
    SDL_DestroyWindow( WINDOW_GLOBAL );

    //mise a NULL avant la désacollaction opérée par SQL_Quit()
    WINDOW_IMAGE_TEST = NULL;
    WINDOW_SURFACE_GLOBAL = NULL;
    WINDOW_GLOBAL = NULL;

    SDL_Quit();
}

int main()
{
    init_all();
    WINDOW_GLOBAL = SDL_CreateWindow( "ULTRACOOL", WINDOW_WIDTH, WINDOW_HEIGHT, 0 );
    WINDOW_SURFACE_GLOBAL = SDL_GetWindowSurface( WINDOW_GLOBAL );

    if(WINDOW_GLOBAL == NULL) {
        nob_log(ERROR, "impossible de créer la fenêtre! cause : %s\n", SDL_GetError());
    return false;
    }
    
    if(chargerImage("/info/etu/l2info/s2400564/Documents/Projet-2025/assets/img/SDL3.bmp", WINDOW_IMAGE_TEST)==true)
        printf("\n| chargement de l'image réussi\n");

    //met l'image dans la surface WINDOW_SURFACE_GLOBAL
    SDL_BlitSurface( WINDOW_IMAGE_TEST, NULL, WINDOW_SURFACE_GLOBAL, NULL);

    //booleen qui determine si la page doit continuer  s'afficher
    bool quitterBool = false;
    //structure pour observer les évenements via son attribut '.type'
    SDL_Event evenement;

    while ( !quitterBool ){

        //récupération des évenements
        SDL_PollEvent(&evenement);
        //action en fonction de l'évenement courant
        switch (evenement.type){
            case SDL_EVENT_QUIT: quitterBool=true;
        }
        SDL_BlitSurface( WINDOW_IMAGE_TEST, NULL, WINDOW_SURFACE_GLOBAL, NULL );
        SDL_UpdateWindowSurface( WINDOW_GLOBAL );
    }

    close_SDL();

    return 0;
}

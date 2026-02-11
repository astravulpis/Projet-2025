#include "SDL3/SDL.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_scancode.h"

#include "../shared.h"

/**
 *  \file main.c
 * \brief Utilisation de SDL pour s'occuper du rendu d'image
 */

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static struct sdl_context_s {
    SDL_Window *window;     //< SDL3 window context
    SDL_Surface *bgSurface; //< SDL3 surface context
    SDL_Renderer *renderer; //< SDL3 renderer context
    SDL_Texture *bgTexture; //< SDL3 surface renderer texture context (je crois que chaque objet doit avoir sa surface et sa texture donc peut être que bgTexture et renderinSurface doivent dégager)
} sdl_ct = {0};

/**
 * \brief fonction qui initialiste tout les sous modules de SDL3 qui vont être utilisés
 */
bool init_all(void);

/**
 * @brief fonction qui effectue toutes les désallocation et mise a NULL des pointeurs SDL3
 * Detruit la fenetre et le context d'SDL3
 */
void close_SDL();

/**
 * \brief fonction qui initialise sdl_ct.bgSurface et sdl_ct.bgTexture
 * et qui rempli la surface d'une couleur renseignée en paramètre en code RGB
 * il est possible de spécifier l'alpha
 */
void initBackgroundColor(int r, int g, int b, int aplha);

/**
 * \brief fonction qui modifie la couleur de l'arrière plan de sdl_ct.window
 * via sa surface et sa texture, également présents dans sdl_ct,
 * a utiliser uniquement si initBackgroundColor() a été utilisé avant
 * (pas encore de verifications implémentés)
 */
void updateBackgroundColor(int r, int g, int b, int aplha);

/**
 * \brief fonction qui effectue le rendu de sdl_ct.bgTexture,
 * il est donc assez utile de faire un initBackgroundColor() avant ;D
 * si l'arrière plan n'est pas init, alors la fonction ne fait rien
 */
void renderBackground();

/**
 * \brief fonction main, on vient d'abord init_all(), puis on a les appels aux fonctions initBackgroundColor() et renderBackground  
 */
int main()
{
    init_all();

    bool quitterBool = false; //< booleen qui determine si la page doit continuer  s'afficher
    SDL_Event evenement;

    float color = 0x18/255.0f;

    initBackgroundColor(255, 255, 255, 255);
    renderBackground();

    //Rendu du logo de SDL
    SDL_FRect * boxSDL = NULL;//les objets SDL_FRect contienent des coordonnées et des dimmensions
    boxSDL=malloc(sizeof(SDL_FRect));
    boxSDL->x=0;
    boxSDL->y=0;
    boxSDL->w=100;
    boxSDL->h=100;

    SDL_Surface *surfaceImgSDL;
    SDL_Texture *textureImgSDL;

    surfaceImgSDL = SDL_LoadBMP("assets/img/SDL3.bmp");
    textureImgSDL = SDL_CreateTextureFromSurface(sdl_ct.renderer, surfaceImgSDL);

    SDL_RenderTexture(sdl_ct.renderer, textureImgSDL, NULL,  boxSDL);
    SDL_RenderPresent(sdl_ct.renderer);

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
    textureImgC = SDL_CreateTextureFromSurface(sdl_ct.renderer, surfaceImgC);

    SDL_RenderTexture(sdl_ct.renderer, textureImgC, NULL,  boxC);
    SDL_RenderPresent(sdl_ct.renderer);
    //----------------------------------

    if (surfaceImgC==NULL)
        printf("impossible charger l'image du logo C ...\n");
    else
        printf("image chargée logo C avec succès (mais qui est succès ?) ...\n");

    if (surfaceImgSDL==NULL)
        printf("impossible charger l'image du logo SDL ...\n");
    else
        printf("image chargée logo SDL avec succès (mais qui est succès ?) ...\n");

    float xC=boxC->x;//pour ne pas perdre la coordonnée x originelle de l'image logo C
    bool avancer=true;
    float i=0;

    while (!quitterBool){

        //comportement du logo
        if(i*(i/100)>254)
            avancer=false;
        else if(i<1)
            avancer=true;
        //execution du comportement
        if (avancer) {
            i+=0.1;
            boxC->x=xC+i*(i/100);
        }
        else {
            i-=0.1;
            boxC->x=xC+i*(i/100);
        }

        //on récupère l'evenement en tête de file
        SDL_PollEvent(&evenement);
        switch (evenement.type){
            case SDL_EVENT_QUIT: quitterBool=true;
        }

        SDL_ClearSurface(sdl_ct.bgSurface, color, color, color, 1.0f);
        SDL_RenderClear(sdl_ct.renderer);

        updateBackgroundColor( i*(i/100), i*(i/100), i, 255);
        renderBackground();
        SDL_RenderTexture(sdl_ct.renderer, textureImgC, NULL, boxC);
        SDL_RenderTexture(sdl_ct.renderer, textureImgSDL, NULL, boxSDL);

        SDL_RenderPresent(sdl_ct.renderer);
    }

    free(boxSDL);
    free(boxC);

    SDL_DestroySurface(surfaceImgC);
    SDL_DestroySurface(surfaceImgSDL);

    SDL_DestroyTexture(textureImgC);
    SDL_DestroyTexture(textureImgSDL);

    close_SDL();

    return 0;
}

/**
 * \brief initialise SDL, return true quand tout c'est bien passe et false sinon
 */
bool init_all(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        return false;
    }

    SDL_CreateWindowAndRenderer("ULTRAC00L", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &(sdl_ct.window), &(sdl_ct.renderer));
    if (sdl_ct.window == NULL) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        close_SDL();
        return false;
    }

    sdl_ct.bgSurface = SDL_GetWindowSurface(sdl_ct.window);
    return true;
}

/**
 * \brief cette fonction vient detruire la fenetre et mettre les pointeurs window et bgSurface a NULL pour eviter les pointeurs fous
 */
void close_SDL()
{
    SDL_DestroyWindow(sdl_ct.window);

    sdl_ct.window = NULL;
    sdl_ct.bgSurface = NULL;

    SDL_Quit();
}

/**
 * \brief cette fonction vient initialiser le background, pour l'instant avec une surface solide
 */
void initBackgroundColor(int r, int g, int b, int aplha){
    sdl_ct.bgSurface = SDL_GetWindowSurface(sdl_ct.window);

    SDL_FillSurfaceRect( sdl_ct.bgSurface, NULL, SDL_MapSurfaceRGB( sdl_ct.bgSurface, r, g, b) );

    sdl_ct.bgTexture = SDL_CreateTextureFromSurface(sdl_ct.renderer, sdl_ct.bgSurface);
}

/**
 * \brief fonction de mise a jour de l'arriere plan
 */
void updateBackgroundColor(int r, int g, int b, int aplha){
    SDL_DestroySurface(sdl_ct.bgSurface);
    SDL_DestroyTexture(sdl_ct.bgTexture);

    SDL_FillSurfaceRect( sdl_ct.bgSurface, NULL, SDL_MapSurfaceRGB( sdl_ct.bgSurface, r, g, b) );

    sdl_ct.bgTexture = SDL_CreateTextureFromSurface(sdl_ct.renderer, sdl_ct.bgSurface);
}

/**
 * \brief cette fonction vient render le background, a condition que la bgTexture ne soit pas NULL
 */
void renderBackground(){
    if (sdl_ct.bgTexture != NULL)
        SDL_RenderTexture(sdl_ct.renderer, sdl_ct.bgTexture, NULL,  NULL);
}


/**
 * \brief On ecoute les touches W et S sur un clavier qwerty, si les 2 sont appuiye en 
 * meme temps alors elles s'annullent et on ne bouge pas, return 1 ou -1 sinon
 */
int avantArriere()
{
    const bool *key_states = SDL_GetKeyboardState();
    int direction = 0;

    /*avant et arriere, les 2 peuvent ainsi s'anuler si on appuie sur les 2 en meme temps*/
    if (key_states[SDL_SCANCODE_W]) {
        direction += 1;  /* Touche W sur un clavier qwerty */
        SDL_Log("Vous avez appuyez sur la touche W");
    } 

    if (key_states[SDL_SCANCODE_S]) {
        direction += -1;  /* Toucher S sur un clavier qwerty */
        SDL_Log("Vous avez appuyez sur la touche S");
    }

    return direction;  /* Pas la touche S ou W alors on ne bouge pas*/
}

/**
 * \brief On ecoute les touches A et D sur un clavier qwerty, si les 2 sont appuiye en 
 * meme temps alors elles s'annullent et on ne bouge pas, return 1 ou -1 sinon
 */
int GaucheDroite()
{
    const bool *key_states = SDL_GetKeyboardState();
    int direction = 0;

    /* gauche et droite, les 2 peuvent ainsi s'anuler si on appuie sur les 2 en meme temps*/
    if (key_states[SDL_SCANCODE_A]) {
        direction += 1;  /* Touche A sur un clavier qwerty */
        SDL_Log("Vous avez appuyez sur la touche A");
    } 

    if (key_states[SDL_SCANCODE_D]) {
        direction += -1;  /* Toucher D sur un clavier qwerty */
        SDL_Log("Vous avez appuyez sur la touche D");
    }

    return direction;  /* Pas la touche A ou D alors on ne bouge pas*/
}

void ecoute_clavier(){
    /*ecoute les touches au clavier et affiche laquelle a ete appuiye (plus partie test) */
    bool quiterBool = false;
    while (!quiterBool) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            /* user has pressed a key? */
            if (e.type == SDL_EVENT_KEY_DOWN) {
                SDL_Log("Wow, you just pressed the %s key!", SDL_GetKeyName(e.key.key));
            }
            if(e.type == SDL_EVENT_KEY_DOWN)
        }
    }
}
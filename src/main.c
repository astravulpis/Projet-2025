#include "SDL3/SDL.h"
#include "../shared.h"

/**
 * @file main.c
 * @brief File where every actions to run the game are being executed at.
 */

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BREAKPOINT __asm("int3");

typedef struct sdl_context_s {
    SDL_Window *window;     //< SDL3 window context
    SDL_Surface *bgSurface; //< SDL3 surface context
    SDL_Renderer *renderer; //< SDL3 renderer context
    SDL_Texture *bgTexture; //< SDL3 surface renderer texture context (je crois que chaque objet doit avoir sa surface et sa texture donc peut être que bgTexture et renderinSurface doivent dégager)
    SDL_Event event;
    bool vsyncActivation;   //< bool that allows the program to not hog the system's ressources (TEMPORARY SOLUTION)
    bool quit;              //< booleen qui determine si la page doit continuer  s'afficher
} sdl_ctx_t;

sdl_ctx_t sdl_ct = {0};

/**
 * @fn init_all()
 * @brief fonction qui initialiste tout les sous modules de SDL3
 */
bool init_all();

/**
 * @fn close_SDL()
 * @brief Detruit la fenetre et le context d'SDL3
 * Fonction qui effectue toutes les désallocation et mise a NULL des pointeurs SDL3
 */
void close_SDL();

/**
 * @fn initBackgroundColor(int r, int g, int b, int aplha)
 * @brief helper function to initialise the background
 * @param[in] r red color of the background
 * @param[in] g green color of the background
 * @param[in] b blue color of the background
 * @param[in] alpha fadeness of the color
 *
 * Initialise sdl_ct.bgSurface et sdl_ct.bgTexture puis rempli la surface d'une couleur renseignée en paramètre en code RGB il est possible de spécifier l'alpha
 */
void initBackgroundColor(int r, int g, int b, int aplha);

/**
 * @fn updateBackgroundColor(int r, int g, int b, int aplha)
 * @brief helper function to modify the color of the background
 * @param[in] r red color of the background
 * @param[in] g green color of the background
 * @param[in] b blue color of the background
 * @param[in] alpha fadeness of the color
 *
 * Modifies the surface and the texture of the sdl context's window
 */
void updateBackgroundColor(int r, int g, int b, int aplha);

/**
 * @fn renderBackground()
 * @brief Renders the background texture if not NULL (?)
 */
void renderBackground();

/**
 * @fn gaucheDroite()
 * @brief Listens to keyboard inputs (specifically A and D)
 * @param[in] ctx SDL context
 * @param[out] direction Returns -1 or 1 whenever A or D are pressed respectively
 */
int gaucheDroite(struct sdl_context_s *ctx);

int main()
{
    init_all();

    int direction = 0;
    float speed = 200.0f;
    float i=0;
    float color = 0x18/255.0f;
    Uint32 last = SDL_GetTicks();
    SDL_FRect * boxSDL = NULL;//les objets SDL_FRect contienent des coordonnées et des dimmensions
    SDL_Surface *surfaceImgSDL;
    SDL_Texture *textureImgSDL;

    sdl_ct.quit = false;

    initBackgroundColor(255, 255, 255, 255);
    renderBackground();

    //Rendu du logo de SDL
    boxSDL=malloc(sizeof(SDL_FRect));
    boxSDL->x=0;
    boxSDL->y=0;
    boxSDL->w=100;
    boxSDL->h=100;

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

    float tempOriginBox = boxC->x;      //pour ne pas perdre la coordonnée x originelle de l'image logo C
    // ----------------------------------------------
    // * REMOVE THIS BELOW TO USE THE `tempOriginBox`
    // ----------------------------------------------
    UNUSED(tempOriginBox);

    while (!sdl_ct.quit){
        Uint32 now = SDL_GetTicks();
        float deltaT = (now - last) / 1000.0f; // seconds since last frame
        last = now;

        SDL_PollEvent(&sdl_ct.event);
        if (sdl_ct.event.type == SDL_EVENT_QUIT){
            sdl_ct.quit = true;
        }

        // Updates the event queue and internal input device state
        SDL_PumpEvents();

        direction = gaucheDroite(&sdl_ct);

        // execution du comportement
        boxC->x += direction * speed * deltaT;

        //on récupère l'evenement en tête de file

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
    sdl_ct.vsyncActivation=true;
    //Activation du Vsync pour avoir un contrôle du framerate et éviter une surcharge du pc
    if (SDL_SetRenderVSync(sdl_ct.renderer, 1) == false) {
        SDL_Log( "Impossible d'initialiser VSync, erreur : %s\n", SDL_GetError() );
        close_SDL();
        return false;
    }

    sdl_ct.bgSurface = SDL_GetWindowSurface(sdl_ct.window);
    return true;
}

void close_SDL()
{
    SDL_DestroyWindow(sdl_ct.window);

    sdl_ct.window = NULL;
    sdl_ct.bgSurface = NULL;

    SDL_Quit();
}

void initBackgroundColor(int r, int g, int b, int aplha)
{
    sdl_ct.bgSurface = SDL_GetWindowSurface(sdl_ct.window);

    SDL_FillSurfaceRect(sdl_ct.bgSurface, NULL, SDL_MapSurfaceRGB(sdl_ct.bgSurface, r, g, b));

    sdl_ct.bgTexture = SDL_CreateTextureFromSurface(sdl_ct.renderer, sdl_ct.bgSurface);
}

void updateBackgroundColor(int r, int g, int b, int aplha)
{
    SDL_DestroySurface(sdl_ct.bgSurface);
    SDL_DestroyTexture(sdl_ct.bgTexture);

    SDL_FillSurfaceRect(sdl_ct.bgSurface, NULL, SDL_MapSurfaceRGB(sdl_ct.bgSurface, r, g, b));

    sdl_ct.bgTexture = SDL_CreateTextureFromSurface(sdl_ct.renderer, sdl_ct.bgSurface);
}

void renderBackground()
{
    if (sdl_ct.bgTexture != NULL) SDL_RenderTexture(sdl_ct.renderer, sdl_ct.bgTexture, NULL,  NULL);
}

int gaucheDroite(struct sdl_context_s *ctx)
{
    SDL_Event e = ctx->event;
    if (e.type == SDL_EVENT_KEY_DOWN) {
        SDL_Log("Wow, you just pressed the %s key!", SDL_GetKeyName(e.key.key));
        switch (e.key.scancode) {
            case SDL_SCANCODE_A:
                return -1;
                break;
            case SDL_SCANCODE_D:
                return 1;
                break;
            case SDL_SCANCODE_Q:
                ctx->quit = true;
                break;
            default:
                break;
        }
    }
    return 0;
}

#include "SDL3/SDL.h"
#include "../shared.h"

/**
 * @file main.c
 * @brief File where every actions to run the game are being executed at.
 */

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BREAKPOINT __asm("int3");

/**
 * @typedef struct sdl_ctx_t
 * @brief shorter abreviation for @ref sdl_context_s
 */
typedef struct sdl_context_s sdl_ctx_t;

/**
 * @struct sdl_context_s
 * @brief Act as our sdl context with multiple variables used about everywhere
 *
 * It holds multiple variable for the rendering of the window, the surface of said window, the event that can be pulled and if the program is running or not.
 */
struct sdl_context_s {
    SDL_Window *window;     //!< SDL window context
    SDL_Renderer *renderer; //!< SDL renderer context
    SDL_Surface *bgSurface; //!< SDL window's background surface (needs to be made into a struct sdl_RenderSurface or smth)
    SDL_Texture *bgTexture; //!< SDL window's background texture (needs to be made into a struct sdl_RenderSurface or smth)
    SDL_Event event;
    bool vsyncActivation;   //!< bool that allows the program to not hog the system's ressources (TEMPORARY SOLUTION)
    bool quit;              //!< bool to quit the main loop
};

sdl_ctx_t sdl_ctx = {0}; //!< Global sdl context variable

/**
 * @fn init_all()
 * @brief Initializes the sdl context
 * @param[out] result Returns `true` or `false` depending on if SDL failed to init its own context.
 */
bool init_all();

/**
 * @fn close_SDL()
 * @brief Destroys the SDL window
 *
 * Helper function to deallocate and put all of the SDL context pointers to NULL.
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
 * Init sdl context's backgroud surface and texture and fills them with the color given in RGBA format
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
 * @fn gaucheDroite(struct sdl_context_s *ctx)
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

    sdl_ctx.quit = false;

    initBackgroundColor(255, 255, 255, 255);
    renderBackground();

    //Rendu du logo de SDL
    boxSDL=malloc(sizeof(SDL_FRect));
    boxSDL->x=0;
    boxSDL->y=0;
    boxSDL->w=100;
    boxSDL->h=100;

    surfaceImgSDL = SDL_LoadBMP("assets/img/SDL3.bmp");
    textureImgSDL = SDL_CreateTextureFromSurface(sdl_ctx.renderer, surfaceImgSDL);

    SDL_RenderTexture(sdl_ctx.renderer, textureImgSDL, NULL,  boxSDL);
    SDL_RenderPresent(sdl_ctx.renderer);

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
    textureImgC = SDL_CreateTextureFromSurface(sdl_ctx.renderer, surfaceImgC);

    SDL_RenderTexture(sdl_ctx.renderer, textureImgC, NULL,  boxC);
    SDL_RenderPresent(sdl_ctx.renderer);
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

    while (!sdl_ctx.quit){
        Uint32 now = SDL_GetTicks();
        float deltaT = (now - last) / 1000.0f; // seconds since last frame
        last = now;

        SDL_PollEvent(&sdl_ctx.event);
        if (sdl_ctx.event.type == SDL_EVENT_QUIT){
            sdl_ctx.quit = true;
        }

        // Updates the event queue and internal input device state
        SDL_PumpEvents();

        direction = gaucheDroite(&sdl_ctx);

        // execution du comportement
        boxC->x += direction * speed * deltaT;

        //on récupère l'evenement en tête de file

        SDL_ClearSurface(sdl_ctx.bgSurface, color, color, color, 1.0f);
        SDL_RenderClear(sdl_ctx.renderer);

        updateBackgroundColor( i*(i/100), i*(i/100), i, 255);
        renderBackground();
        SDL_RenderTexture(sdl_ctx.renderer, textureImgC, NULL, boxC);
        SDL_RenderTexture(sdl_ctx.renderer, textureImgSDL, NULL, boxSDL);

        SDL_RenderPresent(sdl_ctx.renderer);

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
    SDL_CreateWindowAndRenderer("ULTRAC00L", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &(sdl_ctx.window), &(sdl_ctx.renderer));
    if (sdl_ctx.window == NULL) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        close_SDL();
        return false;
    }
    sdl_ctx.vsyncActivation=true;
    //Activation du Vsync pour avoir un contrôle du framerate et éviter une surcharge du pc
    if (SDL_SetRenderVSync(sdl_ctx.renderer, 1) == false) {
        SDL_Log( "Impossible d'initialiser VSync, erreur : %s\n", SDL_GetError() );
        close_SDL();
        return false;
    }

    sdl_ctx.bgSurface = SDL_GetWindowSurface(sdl_ctx.window);
    return true;
}

void close_SDL()
{
    SDL_DestroyWindow(sdl_ctx.window);

    sdl_ctx.window = NULL;
    sdl_ctx.bgSurface = NULL;

    SDL_Quit();
}

void initBackgroundColor(int r, int g, int b, int aplha)
{
    sdl_ctx.bgSurface = SDL_GetWindowSurface(sdl_ctx.window);

    SDL_FillSurfaceRect(sdl_ctx.bgSurface, NULL, SDL_MapSurfaceRGB(sdl_ctx.bgSurface, r, g, b));

    sdl_ctx.bgTexture = SDL_CreateTextureFromSurface(sdl_ctx.renderer, sdl_ctx.bgSurface);
}

void updateBackgroundColor(int r, int g, int b, int aplha)
{
    SDL_DestroySurface(sdl_ctx.bgSurface);
    SDL_DestroyTexture(sdl_ctx.bgTexture);

    SDL_FillSurfaceRect(sdl_ctx.bgSurface, NULL, SDL_MapSurfaceRGB(sdl_ctx.bgSurface, r, g, b));

    sdl_ctx.bgTexture = SDL_CreateTextureFromSurface(sdl_ctx.renderer, sdl_ctx.bgSurface);
}

void renderBackground()
{
    if (sdl_ctx.bgTexture != NULL) SDL_RenderTexture(sdl_ctx.renderer, sdl_ctx.bgTexture, NULL,  NULL);
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

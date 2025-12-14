#include "../thirdparty/SDL2/include/SDL2/SDL.h"
#include <stdbool.h>
#include <stdlib.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "../thirdparty/nob.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { // Non-zero returns are errors
        nob_log(NOB_ERROR, "SDL_Init failed: %s\n", SDL_GetError());
        return EXIT_FAILURE; // EXIT_FAILURE = 1
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 - Test", 100, 100,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    if (window == NULL) {
        nob_log(NOB_ERROR, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return EXIT_FAILURE; // EXIT_FAILURE = 1
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        nob_log(NOB_ERROR, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return EXIT_FAILURE; // EXIT_FAILURE = 1
    }

    SDL_Event ev;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
        // HEX        -> RGBA  : R,G,B,A
        // 0x181818FF -> 0..255: 24,24,24,255
        SDL_SetRenderDrawColor(renderer, 24, 24, 24, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    // End of the application (when quitting)
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS; // EXIT_SUCCESS = 0
}

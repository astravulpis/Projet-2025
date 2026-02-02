#include "../thirdparty/SDL3/include/SDL3/SDL.h"
#include "../thirdparty/SDL3/include/SDL3/SDL_init.h"
#include "../thirdparty/SDL3/include/SDL3/SDL_error.h"
#include <SDL3/SDL_log.h>
#include <stdbool.h>

#define NOB_IMPLEMENTATION
#include "../thirdparty/nob.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        return 1;
    }

    SDL_Quit();
    return 0;
}

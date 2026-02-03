#include "../thirdparty/SDL3/include/SDL3/SDL_init.h"
#include "../thirdparty/SDL3/include/SDL3/SDL_error.h"

#include "../shared.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
    int result = 0;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        return_defer(1);
    }

defer:
    SDL_Quit();
    return result;
}

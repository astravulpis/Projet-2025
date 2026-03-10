#include "../shared.h"
#include "../thirdparty/SDL3/include/SDL3/SDL_error.h"
#include "../thirdparty/SDL3/include/SDL3/SDL_init.h"

int main(void)
{
    int result = 0;

    if (!SDL_Init(0x0)) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        return_defer(1);
    }

    nob_log(INFO, "SDL successfully initialized");

defer:
    if (result) nob_log(ERROR, "SDL failed to initialize");
    SDL_Quit();
    return result;
}

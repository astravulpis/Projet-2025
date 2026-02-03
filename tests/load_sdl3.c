#include "../thirdparty/SDL3/include/SDL3/SDL_init.h"
#include "../thirdparty/SDL3/include/SDL3/SDL_error.h"
#include "../shared.h"

int main(void)
{
    int result = 0;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        return_defer(1);
    }

    printf("everything is working fine");

defer:
    if (result)
        printf("everything is not working fine");
    SDL_Quit();
    return result;
}

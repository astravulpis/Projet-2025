#include "../thirdparty/SDL3/include/SDL3/SDL.h"
#include "../thirdparty/SDL3/include/SDL3/SDL_init.h"
#include "../thirdparty/SDL3/include/SDL3/SDL_error.h"
#include <SDL3/SDL_log.h>
#include <stdbool.h>

#define NOB_IMPLEMENTATION
#include "../thirdparty/nob.h"

#define FLAG_IMPLEMENTATION
#include "../thirdparty/flag.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char *argv[])
{
    bool *autoQuit = flag_bool("-auto-quit", false, "Auto quits the application, useful for testing");
    int result = 0;
    if (!flag_parse(argc, argv)) {
        flag_print_error(stderr);
        return_defer(1);
    }

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        nob_log(ERROR, "SDL failed to initialize. See: %s", SDL_GetError());
        return_defer(1);
    }

defer:
    SDL_Quit();
    return result;
}

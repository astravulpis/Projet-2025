#include <stdbool.h>

#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "thirdparty/nob.h"

#define FLAG_IMPLEMENTATION
#include "thirdparty/flag.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER "src/"

void usage(FILE *stream)
{
    fprintf(stream, "Usage: ./nob [OPTIONS]\n");
    fprintf(stream, "OPTIONS:\n");
    flag_print_options(stream);
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_Cmd cmd = {0};

    bool *help   = flag_bool("help", false, "Print this help");
    bool *clean  = flag_bool("clean", false, "Does a clean build (i.e. rebuilds the build folder)");
    bool *run    = flag_bool("run", false, "run the program");
    bool *debug  = flag_bool("debug", false, "run in debug mode");

    if (!flag_parse(argc, argv)) {
        usage(stderr);
        flag_print_error(stderr);
        return 1;
    }

    if (*help) {
        usage(stdout);
        return 0;
    }

    if (*clean) {
        nob_cmd_append(&cmd, "rm", "-rf", BUILD_FOLDER);
        if (!nob_cmd_run(&cmd)) return 1;
    }

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    if (nob_needs_rebuild1(BUILD_FOLDER"main", SRC_FOLDER"main.c")) {
        nob_cmd_append(&cmd, "cc",
                             "-Wall",
                             "-Wextra",
                             "-o", BUILD_FOLDER"main",
                             SRC_FOLDER"main.c",
                             "-Ithirdparty/SDL2/include",
                             "-Lthirdparty/SDL2/",
                             "-l:libSDL2.a",
                             "-lm");
        if (*debug)
            nob_cmd_append(&cmd, "-ggdb");
        if (!nob_cmd_run(&cmd)) return 1;
    }

    if (*debug) {
        nob_cmd_append(&cmd, "gf2", "./"BUILD_FOLDER"main");
        if (!nob_cmd_run(&cmd)) return 1;
    }
    if (*run && !(*debug)) {
        nob_cmd_append(&cmd, "./"BUILD_FOLDER"main");
        if (!nob_cmd_run(&cmd)) return 1;
    }
    return 0;
}

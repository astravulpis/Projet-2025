#include <stdbool.h>

#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "thirdparty/nob.h"

#define FLAG_IMPLEMENTATION
#include "thirdparty/flag.h"

#define VENDOR_FOLDER "thirdparty/"
#define BUILD_FOLDER "build/"
#define BINARIES_FOLDER BUILD_FOLDER "bin/"
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
    int result = 0;
    size_t mark = nob_temp_save();

    bool *help   = flag_bool("help", false, "Print this help");
    bool *clean  = flag_bool("clean", false, "Does a clean build (i.e. rebuilds the build folder)");
    bool *run    = flag_bool("run", false, "run the program");
    bool *debug  = flag_bool("debug", false, "run in debug mode");

    if (!flag_parse(argc, argv)) {
        usage(stderr);
        flag_print_error(stderr);
        return_defer(1);
    }

    if (*help) {
        usage(stdout);
        return 0;
    }

    if (*clean) {
        nob_cmd_append(&cmd, "rm", "-rf", BUILD_FOLDER);
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

    minimal_log_level = ERROR;
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return_defer(1);
    if (!nob_mkdir_if_not_exists(BINARIES_FOLDER)) return_defer(1);
    minimal_log_level = INFO;

    // Binary compiling
    if (nob_needs_rebuild1(BINARIES_FOLDER "main", SRC_FOLDER "main.c")) {
        nob_cmd_append(&cmd, "cc");
        nob_cmd_append(&cmd, "-Wall");
        nob_cmd_append(&cmd, "-Wextra");
        nob_cmd_append(&cmd, "-o", temp_sprintf("%smain", BINARIES_FOLDER));
        nob_cmd_append(&cmd, temp_sprintf("%smain.c",     SRC_FOLDER));
        nob_cmd_append(&cmd, temp_sprintf("-I%s/include", VENDOR_FOLDER "SDL3"));
        nob_cmd_append(&cmd, temp_sprintf("-L%s/lib",     VENDOR_FOLDER "SDL3"));
        nob_cmd_append(&cmd, "-lSDL3");
        nob_cmd_append(&cmd, "-lm");
        if (*debug) nob_cmd_append(&cmd, "-ggdb");
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

    if (*debug) {
        nob_cmd_append(&cmd, "gf2", "./" BINARIES_FOLDER "main");
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }
    if (*run && !(*debug)) {
        nob_cmd_append(&cmd, "./" BINARIES_FOLDER "main");
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

defer:
    free(cmd.items);
    temp_rewind(mark);

    return result;
}

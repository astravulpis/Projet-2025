#include "shared.h"

// #if !defined(_WIN32)
// #  if defined(nob_cc)
// #    undef nob_cc
// #    define nob_cc(cmd) nob_cmd_append(cmd, "gcc")
// #  endif
// #endif

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

    bool *help    = flag_bool("-help", false, "Print this help");
    bool *clean   = flag_bool("-clean", false, "Does a clean build (i.e. rebuilds the build folder)");
    bool *run     = flag_bool("-run", false, "run the program");
    bool *debug   = flag_bool("-debug", false, "run in debug mode");
    bool *debugui = flag_bool("-debugui", false, "run in debug mode using gf2 (YOU NEED TO HAVE GF2 IN YOUR PATH)");
    bool *tests   = flag_bool("-tests", false, "builds and run the tests, works as a standalone");
    bool *rec     = flag_bool("-test-rec", false, "builds, run, record the output of tests");

    if (!flag_parse(argc, argv)) {
        usage(stderr);
        flag_print_error(stderr);
        return_defer(1);
    }

    if (*debugui) *debug = true;

    if (*help) {
        usage(stdout);
        return 0;
    }

    if (*clean) {
        if (file_exists(BUILD_FOLDER)) {
            nob_cmd_append(&cmd, "rm", "-rf", BUILD_FOLDER);
            if (!nob_cmd_run(&cmd)) return_defer(1);
        }
    }

    if (!(*clean)) minimal_log_level = WARNING;
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return_defer(1);
    if (!nob_mkdir_if_not_exists(BINARIES_FOLDER)) return_defer(1);
    minimal_log_level = INFO;

    // IMPORTANT: `Tests` cannot be run with other commands.
    if (*tests || *rec) {
        set_current_dir("./tests/");
        nob_log(INFO, "CMD: cd ./tests/");
        if (nob_needs_rebuild1("nob", "nob.c")) {
            nob_cmd_append(&cmd, "cc", "-x", "c", "-o", "nob", "nob.c");
            if (!nob_cmd_run(&cmd)) return_defer(1);
        }

        if (*rec) nob_cmd_append(&cmd, "./nob", "-record");
        else nob_cmd_append(&cmd, "./nob");
        if (!nob_cmd_run(&cmd)) return_defer(1);
        return 0;
    }

    // Binary compiling
    if (nob_needs_rebuild1(BINARIES_FOLDER "main", SRC_FOLDER "main.c") || debug) {
        nob_cmd_append(&cmd, "cc");
        nob_cmd_append(&cmd, "-Wall");
        nob_cmd_append(&cmd, "-Wextra");
        nob_cmd_append(&cmd, "-o", temp_sprintf("%smain", BINARIES_FOLDER));
        nob_cmd_append(&cmd, temp_sprintf("%smain.c",     SRC_FOLDER));
        nob_cmd_append(&cmd, temp_sprintf("-I%s/include", VENDOR_FOLDER "SDL3"));
        nob_cmd_append(&cmd, temp_sprintf("-L%s/lib",     VENDOR_FOLDER "SDL3"));
        nob_cmd_append(&cmd, "-lSDL3");
        nob_cmd_append(&cmd, temp_sprintf("-Wl,-rpath,%s/lib", VENDOR_FOLDER "SDL3"));
        nob_cmd_append(&cmd, "-lm");
        if (*debug) nob_cmd_append(&cmd, "-ggdb");
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

    if (*debugui) {
        nob_cmd_append(&cmd, "gf2", "./" BINARIES_FOLDER "main");
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

    if (*run && !(*debugui)) {
        nob_cmd_append(&cmd, "./" BINARIES_FOLDER "main");
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

defer:
    free(cmd.items);
    temp_rewind(mark);

    return result;
}

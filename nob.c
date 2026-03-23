/**
 * @file nob.c
 * @brief Compiling tool script using nob.h single-file header library.
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2025-12-12
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 **/

#include "shared.h"
#include <string.h>

// #if !defined(_WIN32)
// #  if defined(nob_cc)
// #    undef nob_cc
// #    define nob_cc(cmd) cmd_append(cmd, "gcc")
// #  endif
// #endif

typedef struct submodules {
    const char **items;
    size_t count;
    size_t capacity;
} submodules;

#define da_get(da, idx) (da)->items[i]
#define add_sdl_libraries(cmd)                                                                                               \
    do {                                                                                                                     \
        cmd_append((cmd), temp_sprintf("-I%sinclude", VENDOR_FOLDER SDL_FOLDER));                                            \
        cmd_append((cmd), temp_sprintf("-L%slib", VENDOR_FOLDER SDL_FOLDER));                                                \
        cmd_append((cmd), "-lSDL3");                                                                                         \
        cmd_append((cmd), temp_sprintf("-I%sinclude", VENDOR_FOLDER "SDL_Image/"));                                          \
        cmd_append((cmd), temp_sprintf("-L%slib", VENDOR_FOLDER "SDL_Image/"));                                              \
        cmd_append((cmd), "-lSDL3_image");                                                                                   \
        cmd_append((cmd), temp_sprintf("-I%sinclude", VENDOR_FOLDER "SDL_ttf/"));                                            \
        cmd_append((cmd), temp_sprintf("-L%slib", VENDOR_FOLDER "SDL_ttf/"));                                                \
        cmd_append((cmd), "-lSDL3_ttf");                                                                                     \
        cmd_append((cmd), temp_sprintf("-Wl,-rpath,%slib:%slib:%slib", VENDOR_FOLDER SDL_FOLDER, VENDOR_FOLDER "SDL_Image/", \
                                       VENDOR_FOLDER "SDL_ttf/"));                                                           \
        cmd_append((cmd), "-lm");                                                                                            \
    } while (0)

bool debug = false;

void usage(FILE *stream)
{
    fprintf(stream, "Usage: ./nob [OPTIONS]\n");
    fprintf(stream, "OPTIONS:\n");
    flag_print_options(stream);
}

void compile_command(Cmd *cmd, const char *input_path, const char *output_path, bool linking)
{
    cmd_append(cmd, "cc");
    cmd_append(cmd, "-Wall");
    cmd_append(cmd, "-Wextra");
    if (debug) cmd_append(cmd, "-fsanitize=address");
    if (debug) cmd_append(cmd, "-g");
    if (debug) cmd_append(cmd, "-ggdb");
    cmd_append(cmd, "-o", output_path);
    if (!linking) cmd_append(cmd, "-c", input_path);
    else {
        cmd_append(cmd, input_path);
        cmd_append(cmd, LIBPATH);
    }
    add_sdl_libraries(cmd);
}

bool compile_submodules(submodules *modules)
{
    Nob_Cmd cmd = {0};
    bool result = true;

    size_t mark = temp_save();
    for (size_t i = 0; i < modules->count; ++i) {
        nob_temp_rewind(mark);
        char *input = nob_temp_sprintf("%s%s.c", SRC_FOLDER, da_get(modules, i));
        char *output = nob_temp_sprintf("%s%s.o", BUILD_FOLDER, da_get(modules, i));
        if (nob_needs_rebuild1(output, input) || debug) {
            printf("-------------\n");
            printf("Input path: %s\nOutput path: %s\n", input, output);
            compile_command(&cmd, input, output, false); // No linking yet
            if (!cmd_run(&cmd)) return_defer(false);
            printf("-------------\n");
        }
    }

defer:
    free(cmd.items);
    return result;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_Cmd cmd = {0};
    submodules modules = {0};
    int result = 0;
    size_t mark = nob_temp_save();

    flag_bool_var(&debug, "-debug", false, "run in debug mode");
    bool *help    = flag_bool("-help", false, "Print this help");
    bool *clean   = flag_bool("-clean", false, "Does a clean build (i.e. rebuilds the build folder)");
    bool *run     = flag_bool("-run", false, "run the program");
    bool *debugui = flag_bool("-debugui", false, "run in debug mode using gf2");
    bool *tests   = flag_bool("-tests", false, "builds and run the tests, works as a standalone");
    bool *rec     = flag_bool("-test-rec", false, "builds, run, record the output of tests");
    char **level   = flag_str("-level-path", false, "path to the level file, accessed from the root");

    if (!flag_parse(argc, argv)) {
        usage(stderr);
        flag_print_error(stderr);
        return_defer(1);
    }

    if (*debugui) debug = true;

    if (*help) {
        usage(stdout);
        return 0;
    }

    if (*clean) {
        if (file_exists(BUILD_FOLDER)) {
            cmd_append(&cmd, "rm", "-rf", BUILD_FOLDER);
            if (!nob_cmd_run(&cmd)) return_defer(1);
        }
    }

    if (!(*clean)) minimal_log_level = WARNING;
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return_defer(1);
    if (!nob_mkdir_if_not_exists(BINARIES_FOLDER)) return_defer(1);
    if (file_exists(LIBPATH) && (*clean))
        if (!delete_file(LIBPATH)) return_defer(1);
    // if (!nob_mkdir_if_not_exists(BINARIES_FOLDER "assets/")) return_defer(1);
    // if (!copy_directory_recursively("./assets/img", "./build/bin/assets/img")) return_defer(1);

    // Add more submodules here
    da_append(&modules, "event");
    da_append(&modules, "sdl_ctx");
    da_append(&modules, "sdl_helpers");
    da_append(&modules, "player");
    da_append(&modules, "file_parsing");
    da_append(&modules, "buttons");
    da_append(&modules, "gui");
    if (!compile_submodules(&modules)) return_defer(1);

    cmd_append(&cmd, "ar", "rcs");
    cmd_append(&cmd, LIBPATH);
    da_foreach(const char *, it, &modules)
    {
        cmd_append(&cmd, temp_sprintf("%s%s.o", BUILD_FOLDER, *it));
    }
    if (!cmd_run(&cmd)) return_defer(1);

    minimal_log_level = INFO;
    // IMPORTANT: `Tests` cannot be run with other commands.
    if (*tests || *rec) {
        set_current_dir("./tests/");
        nob_log(INFO, "CMD: cd ./tests/");
        if (nob_needs_rebuild1("nob", "nob.c")) {
            cmd_append(&cmd, "cc", "-x", "c", "-o", "nob", "nob.c");
            if (!nob_cmd_run(&cmd)) return_defer(1);
        }

        if (*rec) cmd_append(&cmd, "./nob", "-record");
        else
            cmd_append(&cmd, "./nob");
        if (!nob_cmd_run(&cmd)) return_defer(1);
        return 0;
    }

    // Binary compiling
    if (nob_needs_rebuild1(BINARIES_FOLDER "main", SRC_FOLDER "main.c") || debug) {
        compile_command(&cmd, SRC_FOLDER "main.c", BINARIES_FOLDER "main", true);
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

    const char *level_path = (level[0] != NULL ? temp_sprintf("--level-path=%s", level[0]) : NULL);

    if (*debugui) {
        cmd_append(&cmd, "gf2", "./" BINARIES_FOLDER "main");
        if (level_path != NULL) cmd_append(&cmd, level_path);
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

    if (*run && !(*debugui)) {
        cmd_append(&cmd, "./" BINARIES_FOLDER "main");
        if (level_path != NULL) cmd_append(&cmd, level_path);
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

defer:
    free(cmd.items);
    temp_rewind(mark);

    return result;
}

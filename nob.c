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
 * Rossignol François <francois_rossignol@outlook.fr>
 * Reeves Guillaume <greeves2306@gmail.com>
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

typedef struct {
    const char *include;
    const char *lib;
    const char *linking;
} library;

typedef struct {
    library *items;
    size_t count;
    size_t capacity;

    String_Builder rpath;
} SDL_Libraries;

bool debug = false;

void usage(FILE *stream)
{
    fprintf(stream, "Usage: ./nob [OPTIONS]\n");
    fprintf(stream, "OPTIONS:\n");
    flag_print_options(stream);
}

void add_library(SDL_Libraries *libs, const char *path, const char *linkingName)
{
    library lib = {.include = temp_sprintf("-I%s%sinclude", VENDOR_FOLDER, path),
                   .lib = temp_sprintf("-L%s%slib", VENDOR_FOLDER, path),
                   .linking = temp_sprintf("-l%s", linkingName)};

    sb_appendf(&libs->rpath, "%s%slib:", VENDOR_FOLDER, path);
    da_append(libs, lib);
}

void compile_command(Cmd *cmd, SDL_Libraries *libs, const char *input_path, const char *output_path, bool linking)
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

    da_foreach(library, lib, libs) {
        cmd_append(cmd, lib->include);
        cmd_append(cmd, lib->lib);
        cmd_append(cmd, lib->linking);
    }
    cmd_append(cmd, temp_sprintf("-Wl,-rpath,%s", libs->rpath.items));
    cmd_append(cmd, "-lm");
}

bool compile_submodules(SDL_Libraries *libs, submodules *modules, bool *needsRecompile)
{
    Cmd cmd = {0};
    Procs procs = {0};
    bool result = true;

    size_t mark = temp_save();
    for (size_t i = 0; i < modules->count; ++i) {
        nob_temp_rewind(mark);
        char *input = nob_temp_sprintf("%s%s.c", SRC_FOLDER, da_get(modules, i));
        char *output = nob_temp_sprintf("%s%s.o", BUILD_FOLDER, da_get(modules, i));
        if (nob_needs_rebuild1(output, input) || debug) {
            *needsRecompile = true;
            printf("-------------\n");
            printf("Input path: %s\nOutput path: %s\n", input, output);
            compile_command(&cmd, libs, input, output, false); // No linking yet
            if (!cmd_run(&cmd, .async = &procs)) return_defer(false);
            printf("-------------\n");
        }
    }

    if (!procs_flush(&procs)) return_defer(false);

    if (*needsRecompile) {
        if (file_exists(LIBPATH)) delete_file(LIBPATH);

        nob_log(INFO, "Recreating the archive to hold the modules...");
        cmd_append(&cmd, "ar", "rcs");
        cmd_append(&cmd, LIBPATH);
        da_foreach(const char *, it, modules)
        {
            cmd_append(&cmd, temp_sprintf("%s%s.o", BUILD_FOLDER, *it));
        }
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

defer:
    free(cmd.items);
    free(procs.items);
    return result;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_Cmd cmd = {0};
    submodules modules = {0};
    SDL_Libraries libs = {0};
    int result = 0;
    size_t mark = nob_temp_save();
    bool needsRecompile = false;

    flag_bool_var(&debug, "-debug", false, "run in debug mode");
    bool *help = flag_bool("-help", false, "Print this help");
    bool *clean = flag_bool("-clean", false, "Does a clean build (i.e. rebuilds the build folder)");
    bool *run = flag_bool("-run", false, "run the program");
    bool *debugui = flag_bool("-debugui", false, "run in debug mode using gf2");
    bool *tests = flag_bool("-tests", false, "builds and run the tests, works as a standalone");
    bool *rec = flag_bool("-test-rec", false, "builds, run, record the output of tests");
    char **level = flag_str("-level-path", false, "path to the level file, accessed from the root");

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
    if (!(*clean)) minimal_log_level = INFO;

    // Add more SDL libraries here
    add_library(&libs, SDL_FOLDER, "SDL3");
    add_library(&libs, "SDL_Image/", "SDL3_image");
    add_library(&libs, "SDL_ttf/", "SDL3_ttf");
    add_library(&libs, "SDL_Mixer/", "SDL3_mixer");

    libs.rpath.count--; // Remove the last ':'
    sb_append_null(&libs.rpath); // To finalize the rpath with a '\0'

    // Add more submodules here
    nob_log(INFO, "compiling modules...");
    da_append(&modules, "event");
    da_append(&modules, "sdl_ctx");
    da_append(&modules, "sdl_helpers");
    da_append(&modules, "player");
    da_append(&modules, "file_parsing");
    da_append(&modules, "buttons");
    da_append(&modules, "gui");
    da_append(&modules, "bullets");
    da_append(&modules, "entity");
    da_append(&modules, "music");
    da_append(&modules, "bars");
    da_append(&modules, "sliders");
    da_append(&modules, "checkboxes");
    da_append(&modules, "level");
    da_append(&modules, "player_animation");
    da_append(&modules, "guns");
    da_append(&modules, "triggers");
    da_append(&modules, "game");
    if (!compile_submodules(&libs, &modules, &needsRecompile)) return_defer(1);

    // IMPORTANT: `Tests` cannot be run with other commands.
    if (*tests || *rec) {
        nob_log(INFO, "Running tests...");
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
    if (needsRecompile && file_exists(BINARIES_FOLDER "main")) {
        delete_file(BINARIES_FOLDER "main");
    }
    if (nob_needs_rebuild1(BINARIES_FOLDER "main", SRC_FOLDER "main.c") || debug) {
        compile_command(&cmd, &libs, SRC_FOLDER "main.c", BINARIES_FOLDER "main", true);
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

    const char *level_path = NULL;
    if (level[0]) level_path = temp_sprintf("--level-path=%s", level[0]);

    if (*debugui) {
        nob_log(INFO, "Running main in debug mode");
        cmd_append(&cmd, "gf2", "./" BINARIES_FOLDER "main");
        if (level_path != NULL) cmd_append(&cmd, level_path);
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

    if (*run && !(*debugui)) {
        nob_log(INFO, "Running main in normal mode");
        cmd_append(&cmd, "./" BINARIES_FOLDER "main");
        if (level_path != NULL) cmd_append(&cmd, level_path);
        if (!nob_cmd_run(&cmd)) return_defer(1);
    }

defer:
    free(cmd.items);
    free(modules.items);
    temp_rewind(mark);

    return result;
}

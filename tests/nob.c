/**
 * @file tests/nob.c
 * @brief Compiling script which handles every test unit, and test them one by one
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-02-03
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 **/

#include "../shared.h"

typedef struct target {
    const char *name;
    const char *args;
} target;

typedef struct {
    target *items;
    size_t count;
    size_t capacity;
} targets;

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

void add_library(SDL_Libraries *libs, const char *path, const char *linkingName)
{
    library lib = {.include = temp_sprintf("-I%s%sinclude", VENDOR_FOLDER, path),
                   .lib = temp_sprintf("-L%s%slib", VENDOR_FOLDER, path),
                   .linking = temp_sprintf("-l%s", linkingName)};

    sb_appendf(&libs->rpath, "%s%slib:", VENDOR_FOLDER, path);
    da_append(libs, lib);
}

bool delete_walk_entry(Walk_Entry entry)
{
    return delete_file(entry.path);
}

bool delete_directory_recursively(const char *dir_path)
{
    return walk_dir(dir_path, delete_walk_entry, .post_order = true);
}

bool compile(const char *test_name, SDL_Libraries *libs)
{

    bool result = true;
    Cmd cmd = {0};
    char *src_path = temp_sprintf("%s%s.c", TEST_FOLDER, test_name);
    char *bin_path = temp_sprintf("%s%s", BUILD_FOLDER TEST_FOLDER, test_name);

    nob_log(INFO, "------ Testing: `%s` ------", test_name);

    // Compile the test
    cmd_append(&cmd, "cc");
    cmd_append(&cmd, "-Wall");
    cmd_append(&cmd, "-Wextra");
    cmd_append(&cmd, "-fsanitize=address");
    cmd_append(&cmd, "-g");
    cmd_append(&cmd, "-ggdb");
    cmd_append(&cmd, "-o", bin_path);
    cmd_append(&cmd, src_path);
    cmd_append(&cmd, LIBPATH);

    da_foreach (library, lib, libs) {
        cmd_append(&cmd, lib->include);
        cmd_append(&cmd, lib->lib);
        cmd_append(&cmd, lib->linking);
    }
    cmd_append(&cmd, temp_sprintf("-Wl,-rpath,%s", libs->rpath.items));
    cmd_append(&cmd, "-lm");
    if (!nob_cmd_run(&cmd)) return_defer(1);

defer:
    free(cmd.items);
    return result;
}

bool run_test(target *test, bool record)
{
    bool result = true;

    Cmd cmd = {0};

    String_Builder src = {0};
    String_Builder dst = {0};
    String_View sv_src = {0};
    String_View sv_dst = {0};

    char *bin_path = temp_sprintf("%s%s", BUILD_FOLDER TEST_FOLDER, test->name);
    char *bin_stdout_path = temp_sprintf("%s%s_stdout.txt", BUILD_FOLDER TEST_FOLDER, test->name);
    char *test_output_path = temp_sprintf("%s%s.txt", TEST_FOLDER, test->name);

    cmd_append(&cmd, temp_sprintf("./%s", bin_path));
    if (test->args) { // If any argument is passed
        String_View sv = sv_from_cstr(test->args);
        while (sv.count > 0) {
            String_View arg = sv_chop_by_delim(&sv, ' ');
            cmd_append(&cmd, temp_sprintf(SV_Fmt, SV_Arg(arg)));
        }
    }
    cmd_run(&cmd, .stdout_path = bin_stdout_path);

    if (record) {
        copy_file(bin_stdout_path, test_output_path);
    } else {
        if (!read_entire_file(bin_stdout_path, &src)) return_defer(false);
        if (!read_entire_file(test_output_path, &dst)) return_defer(false);

        sv_src = sb_to_sv(src);
        sv_dst = sb_to_sv(dst);

        // Error output in git's diff style
        if (!sv_eq(sv_src, sv_dst)) {
            nob_log(ERROR, "got an \x1b[31mUNEXPECTED OUTPUT\x1b[0m");
            fprintf(stderr, "<<<<<<< EXPECTED\n");
            fprintf(stderr, SV_Fmt, SV_Arg(sv_dst));
            fprintf(stderr, "================\n");
            fprintf(stderr, SV_Fmt, SV_Arg(sv_src));
            fprintf(stderr, ">>>>>>> CURRENT\n");
            return_defer(false);
        } else {
            nob_log(INFO, "got the \x1b[32mEXPECTED OUTPUT\x1b[0m");
        }
    }

    nob_log(INFO, "------ `%s` is finished ------\n", test->name);

defer:
    free(cmd.items);
    free(src.items);
    free(dst.items);
    return result;
}

int main(int argc, char *argv[])
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    int result = 0;
    size_t mark = nob_temp_save();
    bool record = false;
    UNUSED(shift(argv, argc));
    targets test_targets = {0};
    SDL_Libraries libs = {0};

    if (argc > 0) {
        char *flag = shift(argv, argc);
        if (strcmp(flag, "-record") == 0) record = true;
    }

    minimal_log_level = WARNING;
    set_current_dir("../");

    if (file_exists(BUILD_FOLDER TEST_FOLDER)) {
        if (!delete_directory_recursively(BUILD_FOLDER TEST_FOLDER)) return_defer(1);
    }

    if (!mkdir_if_not_exists(BUILD_FOLDER TEST_FOLDER)) return_defer(1);
    minimal_log_level = INFO;
    add_library(&libs, SDL_FOLDER, "SDL3");
    add_library(&libs, "SDL_Image/", "SDL3_image");
    add_library(&libs, "SDL_ttf/", "SDL3_ttf");
    add_library(&libs, "SDL_Mixer/", "SDL3_mixer");

    libs.rpath.count--; // Remove the last ':'
    sb_append_null(&libs.rpath); // To finalize the rpath with a '\0'

    da_append(&test_targets, ((target){.name = "load_sdl3"}));
    da_append(&test_targets, ((target){.name = "is_Player_inbounds"}));
    da_append(&test_targets, ((target){.name = "parseFile", .args = "--level-path=./tests/parseFile_test_level.txt"}));
    da_append(&test_targets, ((target){.name = "createLevel", .args = "--level-path=./tests/createLevel_test_level.txt"}));
    da_foreach (target, it, &test_targets) {
        mark = nob_temp_save();
        if (!compile(it->name, &libs)) return_defer(1);
        if (!run_test(it, record)) return_defer(1);
        temp_rewind(mark);
    }

defer:
    temp_rewind(mark);

    return result;
}

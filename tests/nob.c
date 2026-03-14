#include "../shared.h"

const char *test_targets[] = {
    "load_sdl3",
    "is_Player_inbounds",
};

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

bool delete_walk_entry(Walk_Entry entry)
{
    return delete_file(entry.path);
}

bool delete_directory_recursively(const char *dir_path)
{
    return walk_dir(dir_path, delete_walk_entry, .post_order = true);
}

bool compile(const char *test_name)
{
    bool result = true;
    Cmd cmd = {0};
    char *src_path = temp_sprintf("%s%s.c", TEST_FOLDER, test_name);
    char *bin_path = temp_sprintf("%s%s", BUILD_FOLDER TEST_FOLDER, test_name);

    nob_log(INFO, "------ Testing: `%s` ------", test_name);

    // Compile the test
    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, bin_path);
    nob_cc_inputs(&cmd, src_path);
    cmd_append(&cmd, LIBPATH);
    add_sdl_libraries(&cmd);
    cmd_append(&cmd, "-lm");
    cmd_append(&cmd, "-ggdb");
    if (!nob_cmd_run(&cmd)) return_defer(1);

defer:
    free(cmd.items);
    return result;
}

bool run_test(const char *test_name, bool record)
{
    bool result = true;

    Cmd cmd = {0};

    String_Builder src = {0};
    String_Builder dst = {0};
    String_View sv_src = {0};
    String_View sv_dst = {0};

    char *bin_path = temp_sprintf("%s%s", BUILD_FOLDER TEST_FOLDER, test_name);
    char *bin_output_path = temp_sprintf("%s%s.txt", BUILD_FOLDER TEST_FOLDER, test_name);
    char *test_output_path = temp_sprintf("%s%s.txt", TEST_FOLDER, test_name);

    cmd_append(&cmd, temp_sprintf("./%s", bin_path));
    cmd_run(&cmd, .stdout_path = bin_output_path, .stderr_path = bin_output_path);

    if (record) {
        copy_file(bin_output_path, test_output_path);
    } else {
        if (!read_entire_file(bin_output_path, &src)) return_defer(false);
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

    nob_log(INFO, "------ `%s` is finished ------\n", test_name);

defer:
    free(cmd.items);
    free(src.items);
    free(dst.items);
    return result;
}

int main(int argc, char *argv[])
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_Cmd cmd = {0};
    int result = 0;
    bool record = false;
    UNUSED(shift(argv, argc));
    size_t arrSize = ARRAY_LEN(test_targets);
    size_t mark = nob_temp_save();

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

    // set_current_dir(curr_cwd);
    for (size_t idx = 0; idx < arrSize; ++idx) {
        mark = nob_temp_save();
        if (!compile(test_targets[idx])) return_defer(1);
        if (!run_test(test_targets[idx], record)) return_defer(1);
        temp_rewind(mark);
    }

defer:
    free(cmd.items);
    temp_rewind(mark);

    return result;
}

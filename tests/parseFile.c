#include "../src/common.h"
// #include "../src/file_parsing.h"
#include "../shared.h"
#include <stdbool.h>

bool parseFlag(int xs_sz, char **xs);
bool parseFile(const char *path);

bool parseFlag(int xs_sz, char **xs)
{
    char *path = NULL;
    flag_str_var(&path, "-level-path", NULL, "Defines the path to the file where a level or room is stored at");

    if (!flag_parse(xs_sz, xs)) {
        flag_print_error(stderr);
        return false;
    }

    if (!parseFile(path)) return false;

    return true;
}

bool parseFile(const char *path)
{
    if (path == NULL) {
        printf("%s:%d: No path provided. Fallback to default debug level\n", __FILE__, __LINE__);
        return true;
    }

    if (!file_exists(path)) {
        printf("%s:%d: %s is not a file or does not exist.\n", __FILE__, __LINE__, path);
        return false;
    }

    String_Builder sb = {0};
    if (!read_entire_file(path, &sb)) return false;

    String_View sv = sb_to_sv(sb);
    size_t mark = temp_save();
    while (sv.count > 0) {
        String_View line = sv_chop_by_delim(&sv, '\n');
        while (line.count > 0) {
            String_View header = sv_chop_by_delim(&line, ' ');

            // If line header is "obj"
            if (sv_eq(header, sv_from_cstr("obj"))) {
                // Path to image
                String_View temp = sv_chop_by_delim(&line, ' ');
                if (!sv_eq(temp, sv_from_cstr("")) && !sv_eq(temp, sv_from_cstr("NULL"))) {
                    sv_chop_left(&temp, 1);
                    sv_chop_right(&temp, 1);
                }
                const char *path = nob_temp_sv_to_cstr(temp);

                // The rectangle's position, width and height
                float rect[4] = {0};
                for (int i = 0; i < 4; ++i) {
                    float val = atof(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
                    rect[i] = val;
                }

                // Removing unwanted values
                if (line.count > 0) {
                    while (line.count > 0) {
                        String_View unwanted = sv_chop_by_delim(&line, ' ');
                        printf("%s:%d: |" SV_Fmt "| <-- This should be empty\n", __FILE__, __LINE__, SV_Arg(unwanted));
                    }
                    continue;
                }

                // Creating the object into the level itself
                printf("%s, %.2f, %.2f, %.2f, %.2f\n", path, rect[0], rect[1], rect[2], rect[3]);
            } else {
                printf("%s:%d: Type \"" SV_Fmt "\" is not yet supported\n", __FILE__, __LINE__, SV_Arg(header));
                break;
            }
        }
    }

    temp_rewind(mark);
    free(sb.items);
    return true;
}

int main(int argc, char **argv)
{
    if (!parseFlag(argc, argv)) {
        printf("%s:%d: the parsing of flags went badly. please see error backtrace\n", __FILE__, __LINE__);
        return 1;
    }

    printf("%s:%d: the parsing of flags went smoothly. No error to report. Exiting...\n", __FILE__, __LINE__);
    return 0;
}

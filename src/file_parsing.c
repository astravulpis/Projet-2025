#include "file_parsing.h"
#include "common.h"

bool parseFlag(int xs_sz, char **xs, objs *level)
{
    char *path = NULL;
    flag_str_var(&path, "-level-path", NULL, "Defines the path to the file where a level or room is stored at");

    if (!flag_parse(xs_sz, xs)) {
        flag_print_error(stderr);
        return false;
    }

    if (!parseFile(path, level)) return false;

    return true;
}

bool parseFile(const char *path, objs *level)
{
    if (path == NULL) {
        nob_log(WARNING, "%s:%d: No path provided. Fallback to default debug level", __FILE__, __LINE__);
        return true;
    }

    if (!file_exists(path)) {
        nob_log(ERROR, "%s:%d: %s is not a file or does not exist.", __FILE__, __LINE__, path);
        return false;
    }

    String_Builder sb = {0};
    if (!read_entire_file(path, &sb)) return false;

    String_View sv = sb_to_sv(sb);
    while (sv.count > 0) {
        String_View line = sv_chop_by_delim(&sv, '\n');
        printf("|"SV_Fmt"|\n", SV_Arg(line));
    }

    free(sb.items);
    return true;
}

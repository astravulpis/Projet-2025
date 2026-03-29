/**
 * @file file_parsing.c
 * @brief Implementation of the different file parsing function
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-26
 * Date: 2026-03-21
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Reeves Guillaume <greeves2306@gmail.com>
 **/

#include "file_parsing.h"
#include "common.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "music.h"

bool parseFlag(int xs_sz, char **xs, sdl_ctx_t *ctx, objs *level)
{
    char *path = NULL;
    flag_str_var(&path, "-level-path", NULL, "Defines the path to the file where a level or room is stored at");

    if (!flag_parse(xs_sz, xs)) {
        flag_print_error(stderr);
        return false;
    }

    if (!parseFile(path, ctx, level)) return false;
    return true;
}

bool parseFile(char *path, sdl_ctx_t *ctx, objs *level)
{
    String_Builder sb = {0};
    bool result = true;
    bool usingDefault = false;
    size_t mark = temp_save();
    char *realPath = NULL;

    if (path == NULL) {
        nob_log(WARNING, "%s:%d: No path provided. Fallback to default debug level", __FILE__, __LINE__);
        usingDefault = true;
    }

    if (!usingDefault) realPath = temp_sprintf("./assets/level/%s.txt", path);
    else {
        realPath = temp_sprintf("./assets/level/level_debug.txt");
        nob_log(WARNING, "using default debug level: %s", realPath);
    }

    if (!file_exists(realPath)) {
        nob_log(ERROR, "%s:%d: %s is not a file or does not exist.", __FILE__, __LINE__, realPath);
        return_defer(false);
    }

    if (!read_entire_file(realPath, &sb)) return false;

    String_View sv = sb_to_sv(sb);

    float rect[4] = {0};
    while (sv.count > 0) {
        temp_rewind(mark);
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

                // The rectangle's position, width and height based on screen/FHD ratio
                for (int i = 0; i < 4; ++i) {
                    float val = atof(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
                    rect[i] = val * ctx->screenRatio;
                }

                // Removing unwanted values
                if (line.count > 0) {
                    while (line.count > 0) {
                        String_View unwanted = sv_chop_by_delim(&line, ' ');
                        nob_log(WARNING, "%s:%d: |" SV_Fmt "| <-- This should be empty", __FILE__, __LINE__, SV_Arg(unwanted));
                    }
                    continue;
                }

                // Creating the object into the level itself
                obj_create(level, ctx, path, rect[0], rect[1], rect[2], rect[3]);
            }
            else if (sv_eq(header, sv_from_cstr("bg"))) {
                String_View bgTemp = sv_chop_by_delim(&line, ' ');
                sv_chop_left(&bgTemp, 1);
#ifdef _WIN32
                sv_chop_right(&bgTemp, 2);
#else
                sv_chop_right(&bgTemp, 1);
#endif
                const char *path = nob_temp_sv_to_cstr(bgTemp);
                if (!loadBackgroundImage(ctx, path)) return false;
            }
            else if (sv_eq(header, sv_from_cstr("mus"))) {
                String_View bgTemp = sv_chop_by_delim(&line, ' ');
                sv_chop_left(&bgTemp, 1);
                sv_chop_right(&bgTemp, 1);
                const char *path = nob_temp_sv_to_cstr(bgTemp);
                printf("%s\n", path);
                if (!Mix_Init(path, ctx)) return false;
            }
            else {
                nob_log(ERROR, "%s:%d: Type \"" SV_Fmt "\" is not yet supported", __FILE__, __LINE__, SV_Arg(header));
                break;
            }
        }
    }

defer:
    temp_rewind(mark);
    free(sb.items);
    if (usingDefault) {
        free(path);
        path = NULL;
    }
    return result;
}

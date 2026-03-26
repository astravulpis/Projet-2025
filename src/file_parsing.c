#include "file_parsing.h"
#include "sdl_ctx.h"
#include "sdl_helpers.h"
#include "common.h"


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
    bool def = false;
    if (path == NULL) {
        nob_log(WARNING, "%s:%d: No path provided. Fallback to default debug level", __FILE__, __LINE__);
        path = strdup("./assets/level/level-debug.txt");
        def = true;
    }

    if (!file_exists(path)) {
        nob_log(ERROR, "%s:%d: %s is not a file or does not exist.", __FILE__, __LINE__, path);
        if (def) free(path);
        return false;
    }

    String_Builder sb = {0};
    if (!read_entire_file(path, &sb)) return false;

    //recuperation de la resolution de l'ecran pour pouvoir adapter les coordone des textures/bounding box
    int num_displays;
    SDL_DisplayID *displays = SDL_GetDisplays(&num_displays);
    const SDL_DisplayMode * screenInformation;
    screenInformation = SDL_GetCurrentDisplayMode(*displays);
    float ratio = (float)screenInformation->h / (float)WINDOW_HEIGHT;
    String_View sv = sb_to_sv(sb);
    size_t mark = temp_save();

    float rect[4];
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

                // The rectangle's position, width and height based on screen/FHD ratio
                for (int i = 0; i < 4; ++i) {
                    float val = atof(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
                    rect[i] = val*ratio;
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
            } else if (sv_eq(header, sv_from_cstr("bg"))) {
                String_View bgTemp = sv_chop_by_delim(&line, ' ');
                sv_chop_left(&bgTemp, 1);
                sv_chop_right(&bgTemp, 1);
                const char *path = nob_temp_sv_to_cstr(bgTemp);
                loadBackgroundImage(ctx, path);
            } else {
                nob_log(ERROR, "%s:%d: Type \"" SV_Fmt "\" is not yet supported", __FILE__, __LINE__, SV_Arg(header));
                break;
            }
        }
    }

    temp_rewind(mark);
    free(sb.items);
    if (def) free(path);
    return true;
}

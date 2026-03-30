/**
 * @file load_sdl3.c
 * @brief First unit test to make sure SDL3 was correctly included into the project
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-30
 * Date: 2026-02-30
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 **/

#include "../shared.h"
#include "../src/common.h"
#include "../src/level.h"

bool parseFile(char *path, level_t **level);

bool parseFlag(int xs_sz, char **xs, level_t **level)
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

bool parseFile(char *path, level_t **level)
{
    String_Builder sb = {0};
    bool result = true;
    size_t mark = temp_save();

    if (path == NULL) {
        printf("%s:%d: No path provided.\n", __FILE__, __LINE__);
        return_defer(false);
    }

    if (!file_exists(path)) {
        printf("%s:%d: %s is not a file or does not exist.\n", __FILE__, __LINE__, path);
        return_defer(false);
    }

    if (!read_entire_file(path, &sb)) return_defer(false);

    String_View sv = sb_to_sv(sb);

    float rect[4] = {0};

    // Its own context
    {
        String_View line = sv_chop_by_delim(&sv, '\n');
        String_View header = sv_chop_by_delim(&line, ' ');
        if (sv_eq(header, sv_from_cstr("info"))) {
            // header title         level_index
            // info   "debug level" 0
            (void)sv_chop_by_delim(&line, '"'); // Remove the left most '"'
            String_View title = sv_chop_by_delim(&line, '"');

            int level_id = atoi(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
            *level = createLevel((char *)nob_temp_sv_to_cstr(title), level_id);
        } else {
            printf("INFO LABEL IS NOT SET. FILE IS INVALID\n");
            return_defer(false);
        }
        temp_rewind(mark); // Title was strdup'd in the creation of the level
    }

    room_t *room = NULL;

    while (sv.count > 0) {
        temp_rewind(mark);
        String_View line = sv_chop_by_delim(&sv, '\n');
        while (line.count > 0) {
            String_View header = sv_chop_by_delim(&line, ' ');

            // Create the room
            if (sv_eq(header, sv_from_cstr("room"))) {
                if (room != NULL) {
                    printf("Assigning room no.%d to level %d\n", room->roomID, (*level)->levelID);
                    assignRoomToLevel((*level), room);
                    room = NULL;
                }
                int room_id = atoi(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
                room = createRoom(room_id);
                printf("Created room id: %d\n", room_id);

            // Insert objects in the room
            } else if (sv_eq(header, sv_from_cstr("obj"))) {

                // Path to image
                String_View temp = sv_chop_by_delim(&line, ' ');
                if (!sv_eq(temp, sv_from_cstr("")) && !sv_eq(temp, sv_from_cstr("NULL"))) {
                    sv_chop_left(&temp, 1);
                    sv_chop_right(&temp, 1);
                }
                (void)nob_temp_sv_to_cstr(temp);

                // The rectangle's position, width and height based on screen/FHD ratio
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
                assignObject_Ex(room, (obj){createRect((rect[0]), (rect[1]), (rect[2]), (rect[3])), NULL});

            // Choose a background image for the level/room
            } else if (sv_eq(header, sv_from_cstr("bg"))) {
                (void)sv_chop_by_delim(&line, ' ');
                nob_log(INFO, "Background loaded");
            } else if (sv_eq(header, sv_from_cstr("entity"))) { // See TODO(2026-03-30 08:08:45)
                TODO("entity parsing");
            } else {
                printf("%s:%d: Type \"" SV_Fmt "\" is not yet supported\n", __FILE__, __LINE__, SV_Arg(header));
                break;
            }
        }
    }
    if (room != NULL) {
        printf("Assigning room no.%d to level %d\n", room->roomID, (*level)->levelID);
        assignRoomToLevel((*level), room);
        loadRoom((*level), (*level)->items[0]->roomID);
    }

defer:
    temp_rewind(mark);
    free(sb.items);
    return result;
}

int main(int argc, char *argv[])
{
    level_t *level = NULL;
    parseFlag(argc, argv, &level);
    destroyLevel(&level);

    printf("everything went well\n");
    return 0;
}

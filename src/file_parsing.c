/**
 * @file file_parsing.c
 * @brief Implementation of the different file parsing function
 *
 * @author Liam B. <liam.berge72@gmail.com>
 * @date 2026-03-21
 * @remark last modified: 2026-03-26
 *
 * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Reeves Guillaume <greeves2306@gmail.com>
 **/

#include "file_parsing.h"
#include "common.h"
#include "level.h"
#include "sdl_ctx.h"
#include "triggers.h"
#include <string.h>

bool parseFlag(int xs_sz, char **xs, sdl_ctx_t **ctx, level_t **level)
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

bool parseFile(char *path, sdl_ctx_t **ctx, level_t **level)
{
    String_Builder sb = {0};
    bool result = true;
    bool usingDefault = false;
    size_t mark = temp_save();
    char *realPath = NULL;

    if (path == NULL) {
        // nob_log(WARNING, "%s:%d: No path provided. Fallback to default debug level", __FILE__, __LINE__);
        usingDefault = true;
    }

    if (!usingDefault && sv_starts_with(sv_from_cstr(path), sv_from_cstr("./"))) {
        realPath = path;
    } else if (!usingDefault) {
        realPath = temp_sprintf("./assets/level/%s.txt", path);
    } else {
        realPath = temp_sprintf("./assets/level/level_debug.txt");
    }

    if (!file_exists(realPath)) {
        nob_log(ERROR, "%s:%d: %s is not a file or does not exist.", __FILE__, __LINE__, realPath);
        return_defer(false);
    }

    if (!read_entire_file(realPath, &sb)) return false;

    String_View sv = sb_to_sv(sb);

    float rect[4] = {0};
    float trigger_rect[4] = {0};

    // Its own context
    {
        String_View line = sv_chop_by_delim(&sv, '\n');
        String_View header = sv_chop_by_delim(&line, ' ');
        if (sv_eq(header, sv_from_cstr("info"))) {
            // info "[LEVEL_NAME]" [LEVEL_INDEX]
            (void)sv_chop_by_delim(&line, '"'); // Remove the left most '"'
            String_View title = sv_chop_by_delim(&line, '"');

            int level_id = atoi(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
            *level = createLevel((char *)nob_temp_sv_to_cstr(title), level_id);
        } else {
            nob_log(ERROR, "INFO LABEL IS NOT SET. EXITING...");
            exit(1);
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

                // room [INDEX]
                // When at least one room has been parsed
                if (room != NULL) {
                    assignRoomToLevel((*level), room);
                    room = NULL;
                }

                int room_id = atoi(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
                room = createRoom(room_id);

            // Insert objects in the room
            } else if (sv_eq(header, sv_from_cstr("obj"))) {

                // obj "[PATH_TO_IMAGE]" [X_POS] [Y_POS] [WIDTH] [HEIGHT]
                String_View temp = sv_chop_by_delim(&line, ' ');
                if (!sv_eq(temp, sv_from_cstr("")) && !sv_eq(temp, sv_from_cstr("NULL"))) {
                    sv_chop_left(&temp, 1);
                    sv_chop_right(&temp, 1);
                }
                const char *path = nob_temp_sv_to_cstr(temp);

                // The rectangle's position, width and height based on screen/FHD ratio
                for (int i = 0; i < 4; ++i) {
                    float val = atof(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
                    rect[i] = val * (*ctx)->screenRatio;
                }

                // Removing unwanted values
                if (line.count > 0) {
                    while (line.count > 0) {
                        String_View unwanted = sv_chop_by_delim(&line, ' ');
                        nob_log(WARNING, "%s:%d: |" SV_Fmt "| <-- This should be empty", __FILE__, __LINE__, SV_Arg(unwanted));
                    }
                    continue;
                }
                assignObject(room, (*ctx), path, rect[0], rect[1], rect[2], rect[3]);

            } else if (sv_eq(header, sv_from_cstr("bg"))) {

                // bg "[PATH_TO_IMG]"
                String_View bgTemp = sv_chop_by_delim(&line, ' ');
                sv_chop_left(&bgTemp, 1);

                sv_chop_right(&bgTemp, 1);
                if (bgTemp.data[bgTemp.count - 1] == '"') sv_chop_right(&bgTemp, 1);

                const char *path = nob_temp_sv_to_cstr(bgTemp);
                if (!loadBackgroundImage((*ctx), path)) return false;

            } else if (sv_eq(header, sv_from_cstr("player"))) {

                // player [X_POS] [Y_POS]
                int x_pos = atoi(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' '))) * (*ctx)->screenRatio;
                int y_pos = atoi(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' '))) * (*ctx)->screenRatio;
                room->startPos = (V2f){x_pos, y_pos};

            // entities
            } else if (sv_eq(header, sv_from_cstr("entity"))) {
                // entity [TYPE] [X_POS] [Y_POS] [WAVE_INDEX]
                String_View type = sv_chop_by_delim(&line, ' ');
                float x_pos = atof(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' '))) * (*ctx)->screenRatio;
                float y_pos = atof(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' '))) * (*ctx)->screenRatio;
                int waveIdx = atoi(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));

                // Might consider to just abs(waveIdx) but the fact that a waveIdx can be <0 is just wrong
                assert(waveIdx >= 0 && "the wave index should be zero or positive ");
                assignEntityToWave(room, ctx, type, (V2f){x_pos, y_pos}, waveIdx);

            } else if (sv_eq(header, sv_from_cstr("trigger"))) {

                // trigger [X_POS] [Y_POS] [WIDTH] [HEIGHT] [TRIGGER_KIND] [WAVE_IDX]
                // trigger [X_POS] [Y_POS] [WIDTH] [HEIGHT] [TRIGGER_KIND] [ROOM_DEST] [NEW_X_POS] [NEW_Y_POS]

                // the trigger's position adapted to the screen ratio
                for (int i = 0; i < 4; ++i) {
                    float val = atof(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
                    trigger_rect[i] = val * (*ctx)->screenRatio;
                }

                String_View type = sv_chop_by_delim(&line, ' ');
                trigger_kind kind = getTriggerKindFromSV(type);

                trigger_t *trigger = createTrigger(trigger_rect[0], trigger_rect[1], trigger_rect[2], trigger_rect[3], kind);

                switch (kind) {
                case PORTAL: {
                    trigger->room_dst = atoi(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
                    V2f new_pos = {0};
                    new_pos.x = atoi(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
                    new_pos.y = atoi(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
                    trigger->newPos = new_pos;
                } break;
                case SPAWNER: {
                    trigger->waveId = atoi(nob_temp_sv_to_cstr(sv_chop_by_delim(&line, ' ')));
                } break;
                case ONESHOT:
                    break;
                default:
                    UNREACHABLE("trigger kind");
                }

                // Removing unwanted values
                if (line.count > 0) {
                    while (line.count > 0) {
                        String_View unwanted = sv_chop_by_delim(&line, ' ');
                        nob_log(WARNING, "%s:%d: |" SV_Fmt "| <-- This should be empty", __FILE__, __LINE__, SV_Arg(unwanted));
                    }
                    continue;
                }

                assignTriggerToRoom(room, trigger);

            // get the music to play in the background
            } else if (sv_eq(header, sv_from_cstr("mus"))) {
                String_View temp = sv_chop_by_delim(&line, ' ');
                sv_chop_left(&temp, 1);
                sv_chop_right(&temp, 1);

                if (temp.data[temp.count - 1] == '"') sv_chop_right(&temp, 1);

                (*level)->BGM_path = strdup((char *)nob_temp_sv_to_cstr(temp));
            } else {
                nob_log(ERROR, "%s:%d: Type \"" SV_Fmt "\" is not yet supported", __FILE__, __LINE__, SV_Arg(header));
                break;
            }
        }
    }
    if (room != NULL) {
        assignRoomToLevel((*level), room);
        loadRoom((*level), (*level)->items[0]->roomID);
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

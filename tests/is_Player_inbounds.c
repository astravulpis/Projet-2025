#include "../shared.h"
#include "../src/event.h"
#include "../src/sdl_helpers.h"
#include "SDL3/SDL_rect.h"
#include <assert.h>
#include <stdbool.h>

typedef struct {
    float x, y;
} V2f;
V2f expectedCoords = {0};

bool is_at_expected_coords(SDL_FRect *r, V2f expected_coords)
{
    return (r->x == expected_coords.x && r->y == expected_coords.y);
}

void move_rect(SDL_FRect *r, V2f new_pos)
{
    r->x = new_pos.x;
    r->y = new_pos.y;
}
void resize_rect(SDL_FRect *r, V2f new_size)
{
    r->w = new_size.x;
    r->h = new_size.y;
}

int main(void)
{
    SDL_FRect *rect = NULL;
    int result = 0;

    // Creating a rectangle
    rect = createRect(0, 0, 0, 0);
    if (rect == NULL) {
        nob_log(ERROR, "%s:%d: Rect failed to be created", __FILE__, __LINE__);
        return_defer(1);
    }

    if (rect->w != 1 && rect->h != 1) {
        nob_log(ERROR, "%s:%d: Rect->w = %.2f, Rect->h = %.2f - Values are not set correctly.", __FILE__, __LINE__, rect->w,
                rect->y);
        return_defer(1);
    }
    nob_log(INFO, "Rectangle successfully created");

    // Setting it in bounds
    keep_player_inbound(rect, 0.0f, 0.0f, 1920.0f, 1080.0f);
    if (!is_at_expected_coords(rect, expectedCoords)) {
        nob_log(ERROR, "%s:%d: Expected: (%.2f, %.2f), got: (%.2f, %.2f)", __FILE__, __LINE__, expectedCoords.x,
                expectedCoords.y, rect->x, rect->y);
        return_defer(1);
    }
    nob_log(INFO, "Got expected coords");

    // Moving it oobe
    move_rect(rect, (V2f){9999.0f, 9999.0f});
    expectedCoords.x = WINDOW_WIDTH - rect->w;
    expectedCoords.y = WINDOW_HEIGHT - rect->h;

    // Setting it back in bounds
    keep_player_inbound(rect, 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!is_at_expected_coords(rect, expectedCoords)) {
        nob_log(ERROR, "%s:%d: Expected: (%.2f, %.2f), got: (%.2f, %.2f)", __FILE__, __LINE__, expectedCoords.x,
                expectedCoords.y, rect->x, rect->y);
        return_defer(1);
    }
    nob_log(INFO, "Got expected coords after transformation of position");

    // Mofiying the rect's size to check for correct displacement
    resize_rect(rect, (V2f){128.0f, 256.0f});
    expectedCoords.x = WINDOW_WIDTH - rect->w;
    expectedCoords.y = WINDOW_HEIGHT - rect->h;

    // Setting it back in bounds
    keep_player_inbound(rect, 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!is_at_expected_coords(rect, expectedCoords)) {
        nob_log(ERROR, "%s:%d: Expected: (%.2f, %.2f), got: (%.2f, %.2f)", __FILE__, __LINE__, expectedCoords.x,
                expectedCoords.y, rect->x, rect->y);
        return_defer(1);
    }
    nob_log(INFO, "Got expected coords after transformation of size");

    // Moving and resizing
    move_rect(rect, (V2f){9999.0f, -100.0f});
    resize_rect(rect, (V2f){200.0f, 200.0f});
    expectedCoords.x = WINDOW_WIDTH - rect->w;
    expectedCoords.y = 0;

    keep_player_inbound(rect, 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!is_at_expected_coords(rect, expectedCoords)) {
        nob_log(ERROR, "%s:%d: Expected: (%.2f, %.2f), got: (%.2f, %.2f)", __FILE__, __LINE__, expectedCoords.x,
                expectedCoords.y, rect->x, rect->y);
        return_defer(1);
    }
    nob_log(INFO, "Got expected coords after transformation of size and position");

    nob_log(INFO, "Every test case for `keep_player_inbound` passed successfully");

defer:
    if (result) nob_log(ERROR, "Things failed");
    free(rect);
    rect = NULL;
    return result;
}

/**
 * @file is_player_inbounds.c
 * @brief keep player inbounds of the window display test unit
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-03-10
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Reeves Guillaume <greeves2306@gmail.com>
 **/

#include "../shared.h"
#include "SDL3/SDL_rect.h"
#include "sdl_helpers.h"
#include <assert.h>
#include <stdbool.h>

V2f expectedCoords = {0};

bool isAtExpectedCoords(SDL_FRect *r, V2f expected_coords)
{
    return (r->x == expected_coords.x && r->y == expected_coords.y);
}

void moveRect(SDL_FRect *r, V2f new_pos)
{
    r->x = new_pos.x;
    r->y = new_pos.y;
}
void resizeRect(SDL_FRect *r, V2f new_size)
{
    r->w = new_size.x;
    r->h = new_size.y;
}

void keepPlayerInbound(SDL_FRect *r, float minX, float minY, float maxX, float maxY)
{
    if (r == NULL) return;
    if (r->x < minX) r->x = minX;               // Left
    if (r->y < minY) r->y = minY;               // Down
    if (r->x + r->w > maxX) r->x = maxX - r->w; // Right
    if (r->y + r->h > maxY) r->y = maxY - r->h; // Up
}

int main(void)
{
    SDL_FRect *rect = NULL;
    int result = 0;

    // Creating a rectangle
    rect = createRect(0, 0, 0, 0);
    if (rect == NULL) {
        printf("%s:%d: Rect failed to be created\n", __FILE__, __LINE__);
        return_defer(1);
    }

    if (rect->w != 1 && rect->h != 1) {
        printf("%s:%d: Rect->w = %.2f, Rect->h = %.2f - Values are not set correctly.\n", __FILE__, __LINE__, rect->w, rect->y);
        return_defer(1);
    }
    printf("Rectangle successfully created\n");

    // Setting it in bounds
    keepPlayerInbound(rect, 0.0f, 0.0f, 1920.0f, 1080.0f);
    if (!isAtExpectedCoords(rect, expectedCoords)) {
        printf("%s:%d: Expected: (%.2f, %.2f), got: (%.2f, %.2f)\n", __FILE__, __LINE__, expectedCoords.x, expectedCoords.y,
               rect->x, rect->y);
        return_defer(1);
    }
    printf("Got expected coords\n");

    // Moving it oobe
    moveRect(rect, (V2f){9999.0f, 9999.0f});
    expectedCoords.x = WINDOW_WIDTH - rect->w;
    expectedCoords.y = WINDOW_HEIGHT - rect->h;

    // Setting it back in bounds
    keepPlayerInbound(rect, 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!isAtExpectedCoords(rect, expectedCoords)) {
        printf("%s:%d: Expected: (%.2f, %.2f), got: (%.2f, %.2f)\n", __FILE__, __LINE__, expectedCoords.x, expectedCoords.y,
               rect->x, rect->y);
        return_defer(1);
    }
    printf("Got expected coords after transformation of position\n");

    // Mofiying the rect's size to check for correct displacement
    resizeRect(rect, (V2f){128.0f, 256.0f});
    expectedCoords.x = WINDOW_WIDTH - rect->w;
    expectedCoords.y = WINDOW_HEIGHT - rect->h;

    // Setting it back in bounds
    keepPlayerInbound(rect, 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!isAtExpectedCoords(rect, expectedCoords)) {
        printf("%s:%d: Expected: (%.2f, %.2f), got: (%.2f, %.2f)\n", __FILE__, __LINE__, expectedCoords.x, expectedCoords.y,
               rect->x, rect->y);
        return_defer(1);
    }
    printf("Got expected coords after transformation of size\n");

    // Moving and resizing
    moveRect(rect, (V2f){9999.0f, -100.0f});
    resizeRect(rect, (V2f){200.0f, 200.0f});
    expectedCoords.x = WINDOW_WIDTH - rect->w;
    expectedCoords.y = 0;

    keepPlayerInbound(rect, 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!isAtExpectedCoords(rect, expectedCoords)) {
        printf("%s:%d: Expected: (%.2f, %.2f), got: (%.2f, %.2f)\n", __FILE__, __LINE__, expectedCoords.x, expectedCoords.y,
               rect->x, rect->y);
        return_defer(1);
    }
    printf("Got expected coords after transformation of size and position\n");

    printf("Every test case for `keep_player_inbound` passed successfully\n");

defer:
    if (result) printf("Things failed\n");
    free(rect);
    rect = NULL;
    return result;
}

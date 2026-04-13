/**
 * @file main.c
 * @brief Main translation unit to initialise and run the core loop of the game
 *
 * @author Liam B. <liam.berge72@gmail.com>
 * @date 2026-02-02
 * @remark last modified: 2026-03-18
 *
 * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Rossignol François <francois_rossignol@outlook.fr>
 * Reeves Guillaume <greeves2306@gmail.com>
 **/

#include "../shared.h"
#include "game.h"

/**
 * @file main.c
 * @brief The file to execute the game's main loop
 */

int main(int argc, char **argv)
{
    gameContext *ctx = NULL;
    if (initGameContext(&ctx, argc, argv)) gameLoop(ctx);
    closeGame(&ctx);
    return 0;
}

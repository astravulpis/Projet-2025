/**
 * @file main.c
 * @brief Main translation unit to initialise and run the core loop of the game
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-02-02
 *
 * * Contributors:
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
    gameContext ctx = {0};
    gameLoop(&ctx, argc, argv);
    closeGame(&ctx);
    return 0;
}

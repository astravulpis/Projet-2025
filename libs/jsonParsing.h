#include "common.h"
#include "player.h"
#include "cJSON.h"

/**
 * @fn readJSON(player_t * player)
 * @param[in] player pointer pointer to the player itself
 * @brief reads the player's data from a JSON file
 *
 * loads the player's data from a JSON file and initializes it into the player structure
 */
bool readJSON(player_t * player);

/**
 * @fn bool writeJSON(player_t * player)
 * @param[in] player pointer pointer to the player itself
 * @brief updates the json file
 *
 * changes the score value in the JSON file to the player's current score at the end of the game
 */
bool writeJSON(player_t * player);
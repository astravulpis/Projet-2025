/**
 * @file file_parsing.h
 * @brief Declarations to parse a file
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-26
 * Date: 2026-03-21
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 * Reeves Guillaume <greeves2306@gmail.com>
 **/

#ifndef FILE_PARSING_H
#define FILE_PARSING_H

#include "common.h"

/**
 * @fn parseFlag(int xs_sz, char **xs, sdl_ctx_t *ctx, objs *level)
 * @param[in] xs_sz Amount of elements in the list of elements `xs`
 * @param[in] xs List of elements
 * @param[in] ctx our own sdl context variable
 * @param[in] level level object
 * @brief Parse the arguments given when calling the main function.
 */
bool parseFlag(int xs_sz, char **xs, sdl_ctx_t *ctx, objs *level);

/**
 * @fn parseFile(const char *path, sdl_ctx_t *ctx, objs *level)
 * @param[in] path The path to the file to parse
 * @param[in] ctx our own sdl context variable
 * @param[in] level Array of objects that will be filled with the level's objects
 * @brief Parse a level file to get each object out and constructed into the array `level`
 */
bool parseFile(char *path, sdl_ctx_t *ctx, objs *level);

#endif // FILE_PARSING_H

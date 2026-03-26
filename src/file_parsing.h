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
 * @param[in] xs_sz argument size
 * @param[in] xs arguments
 * @param[in] ctx our own sdl context variable
 * @param[in] level level object
 * @brief parse l'appel du programme pour recuperer les flags
 */
bool parseFlag(int xs_sz, char **xs, sdl_ctx_t *ctx, objs *level);

/**
 * @fn parseFile(const char *path, sdl_ctx_t *ctx, objs *level)
 * @param[in] char chemin d'acces du fichier des niveaux
 * @param[in] ctx our own sdl context variable
 * @param[in] level level object
 * @brief parse le fichier pour recuperer les objects data
 *
 * Parcourt le fichier donne pour recuperer tout les objects et dans le meme temps viens
 * les adapter a la taille de la fenetre
 */
bool parseFile(char *path, sdl_ctx_t *ctx, objs *level);

#endif // FILE_PARSING_H

#ifndef FILE_PARSING_H
#define FILE_PARSING_H

#include "common.h"

bool parseFlag(int xs_sz, char **xs, sdl_ctx_t *ctx, objs *level);
bool parseFile(const char *path, sdl_ctx_t *ctx, objs *level);

#endif // FILE_PARSING_H

#ifndef FILE_PARSING_H
#define FILE_PARSING_H

#include "common.h"

bool parseFlag(int xs_sz, char **xs, objs *level);
bool parseFile(const char *path, objs *level);

#endif // FILE_PARSING_H

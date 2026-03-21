#include "../src/common.h"
#include "../src/file_parsing.h"
#include <stdbool.h>
#include "../shared.h"

int main(int argc, char **argv)
{
    objs level = {0};
    if (!parseFlag(argc, argv, &level)) {
        printf("%s:%d: the parsing of flags went badly. please see error backtrace\n", __FILE__, __LINE__);
        return 1;
    }

    printf("%s:%d: the parsing of flags went smoothly. No error to report. Exiting...\n", __FILE__, __LINE__);
    free(level.items);
    return 0;
}

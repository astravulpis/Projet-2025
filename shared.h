/**
 * @file shared.h
 * @brief All shared defines across all files, not just src/ files.
 *
 * Author: Liam B. <liam.berge72@gmail.com>
 * Last Modified: 2026-03-18
 * Date: 2026-02-03
 *
 * * Contributors:
 * Liam B. <liam.berge72@gmail.com>
 **/

#include <stdbool.h>

#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "thirdparty/nob.h"

#define FLAG_IMPLEMENTATION
#include "thirdparty/flag.h"

#define VENDOR_FOLDER "thirdparty/"
#define BUILD_FOLDER "build/"
#define BINARIES_FOLDER BUILD_FOLDER "bin/"
#define TEST_FOLDER "tests/"
#define SRC_FOLDER "src/"

#define SDL_FOLDER "SDL3/"

#define LIBPATH BUILD_FOLDER "libModules.a"

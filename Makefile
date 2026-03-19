OUTPUT    := main
SRC_DIR   := src
BUILD_DIR := build
# Matching your nob.c "VENDOR_FOLDER" logic
VENDOR    := thirdparty

CC        := gcc
CFLAGS    := -Wall -Wextra -g -ggdb -fsanitize=address

# You must include the VENDOR folder itself to find "nob.h"
INCLUDES  := -Isrc \
             -I$(VENDOR) \
             -I$(VENDOR)/SDL3/include \
             -I$(VENDOR)/SDL_Image/include \
             -I$(VENDOR)/SDL_ttf/include

# Matching your nob.c add_sdl_libraries() logic
LDFLAGS   := -L$(VENDOR)/SDL3/lib \
             -L$(VENDOR)/SDL_Image/lib \
             -L$(VENDOR)/SDL_ttf/lib
LIBS      := -lSDL3 -lSDL3_image -lSDL3_ttf -lm
RPATH     := -Wl,-rpath,$(VENDOR)/SDL3/lib:$(VENDOR)/SDL_Image/lib:$(VENDOR)/SDL_ttf/lib

SRC       := event.c sdl_helpers.c sdl_ctx.c player.c
OBJS      := $(SRC:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/$(OUTPUT)

# Link the final binary (includes main.c and the objects)
$(BUILD_DIR)/$(OUTPUT): $(SRC_DIR)/main.c $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LIBS) $(RPATH)

# Compile submodules (event.o, sdl_helpers.o)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(BUILD_DIR)/$(OUTPUT)

.PHONY: all run clean

OUTPUT    := main
SRC_DIR   := src
BUILD_DIR := build
BIN_DIR := build/bin
# Matching your nob.c "VENDOR_FOLDER" logic
VENDOR    := thirdparty

CC        := gcc
CFLAGS    := -Wall -Wextra -g -ggdb

# You must include the VENDOR folder itself to find "nob.h"
INCLUDES  := -I./src/ \
             -I./libs/ \
             -I$(VENDOR) \
             -I$(VENDOR)/SDL3/include \
             -I$(VENDOR)/SDL_Image/include \
             -I$(VENDOR)/SDL_ttf/include \
             -I$(VENDOR)/SDL_Mixer/include

# Matching your nob.c add_sdl_libraries() logic
LDFLAGS   := -L$(VENDOR)/SDL3/lib \
             -L$(VENDOR)/SDL_Image/lib \
             -L$(VENDOR)/SDL_ttf/lib \
             -L$(VENDOR)/SDL_Mixer/lib
LIBS      := -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer -lm
RPATH     := -Wl,-rpath,$(VENDOR)/SDL3/lib:$(VENDOR)/SDL_Image/lib:$(VENDOR)/SDL_ttf/lib:$(VENDOR)/SDL_Mixer/lib

SRC       := event.c sdl_ctx.c sdl_helpers.c player.c file_parsing.c buttons.c gui.c bullets.c entity.c music.c bars.c sliders.c checkboxes.c level.c player_animation.c guns.c triggers.c game.c
OBJS      := $(SRC:%.c=$(BUILD_DIR)/%.o)


all: $(BUILD_DIR)/$(OUTPUT)

# Link the final binary (includes main.c and the objects)
$(BUILD_DIR)/$(OUTPUT): $(SRC_DIR)/main.c $(BUILD_DIR)/libModules.a | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LIBS) $(RPATH)

# Compile submodules (event.o, sdl_helpers.o)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/libModules.a: $(OBJS) | $(BUILD_DIR)
	ar rcs $@ $^

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(BUILD_DIR)/$(OUTPUT)

.PHONY: all run clean

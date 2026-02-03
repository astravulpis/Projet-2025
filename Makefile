OUTPUT := main
SRC_DIR := src
BUILD_DIR := build
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

CXX := cc
CXXFLAGS := -Wall -Wextra -ggdb
INCLUDES := -Isrc -Ithirdparty/SDL3/include
LIBS :=  -Lthirdparty/SDL3/ -lSDL3 -lm
OPT := -Wl,-rpath,thirdparty/SDL3/lib

all: $(BUILD_DIR)/$(OUTPUT)

# Compile the executable
$(BUILD_DIR)/$(OUTPUT): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS) $(OPT)

# Compile each c file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create build directory (not needed for subdirs, see above mkdir -p)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(BUILD_DIR)/$(OUTPUT)

.PHONY: all run clean


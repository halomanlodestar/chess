CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra $(shell sdl2-config --cflags) $(shell pkg-config --cflags SDL2_image)
LDFLAGS  := $(shell sdl2-config --libs) $(shell pkg-config --libs SDL2_image)

SRC_DIR  := src
BUILD_DIR := build
TARGET   := $(BUILD_DIR)/chess

SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

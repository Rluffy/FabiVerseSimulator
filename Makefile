CXX ?= g++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra

SOURCES := $(wildcard src/*.cpp src/configs/*.cpp src/handlers/*.cpp src/monitors/*.cpp src/objects/*.cpp src/pugixml/*.cpp)
BUILD_DIR := build
TARGET := $(BUILD_DIR)/simulator

.PHONY: build check run clean

build: $(TARGET)

$(TARGET): $(SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

# Compiles every source without writing binaries
check:
	$(CXX) $(CXXFLAGS) -fsyntax-only $(SOURCES)

# The config path is relative to src/, so the simulator runs from there
run: build
	cd src && ../$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

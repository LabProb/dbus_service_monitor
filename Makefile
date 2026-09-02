.PHONY: all configure build clean rebuild

BUILD_DIR=build

all: configure build

configure:
	cmake -S . -B $(BUILD_DIR)

build:
	cmake --build $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

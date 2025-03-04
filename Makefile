CC=gcc
CFLAGS=-Wall
BUILD_DIR=./build
SRC_DIR=./src
SRC=$(shell ls $(SRC_DIR)/*.c)

.PHONY: build build_dir clean

build: build_dir
	$(CC) $(CFLAGS) $(SRC) -o $(BUILD_DIR)/a.out
	$(BUILD_DIR)/a.out

build_dir:
	@if [ -e "$(BUILD_DIR)/" ]; then \
		echo "Directory '$(BUILD_DIR)' exists."; \
	else \
		mkdir $(BUILD_DIR)/; \
	fi

clean:
	@if [ -e "$(BUILD_DIR)/" ]; then \
		rm -f $(BUILD_DIR)/* \
	else \
		echo "Directory '$(BUILD_DIR)/' doesn't exists."; \
	fi

SRC_DIR = src
BIN_DIR = bin
INC_DIR = include
LIB_DIR = lib

CC = gcc
C_VERSION = gnu99
CFLAGS = -Wall -Wextra -Wshadow -pedantic-errors -std=$(C_VERSION) -O3 -I$(INC_DIR) -L$(LIB_DIR) -ljcrl

$(BIN_DIR)/list_append: $(SRC_DIR)/list_append.c
	$(CC) $^ $(CFLAGS) -o $@

$(BIN_DIR)/queue_push: $(SRC_DIR)/queue_push.c
	$(CC) $^ $(CFLAGS) -o $@

.PHONY: all
all: $(BIN_DIR)/list_append $(BIN_DIR)/queue_push

.PHONY: clean
clean:
	rm $(BIN_DIR)/*


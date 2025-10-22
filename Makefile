CC = gcc
ifeq ($(shell which $(CC)),)
    CC := clang
endif
CFLAGS = -g -O0 -Wall -std=c99 -Iinclude
LDFLAGS = -lncurses

SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = cart

PREFIX = $(HOME)/.local
BIN_DIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	@echo "Installing cart to $(BIN_DIR) ..."
	@mkdir -p $(BIN_DIR)
	@install $(TARGET) $(BIN_DIR)/$(TARGET)
	@echo "cart installed successfully!"

uninstall: 
	@rm -rf $(BIN_DIR)/$(TARGET)
	@echo "cart uninstalled successfully!"

clean:
	@rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all install uninstall clean

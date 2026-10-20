CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinc
LDFLAGS =

SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin

# Main application sources (exclude main.c from tests)
APP_SOURCES = $(filter-out $(SRC_DIR)/main.c,$(shell find $(SRC_DIR) -type f -name '*.c'))
APP_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(APP_SOURCES))
APP_TARGET = $(BIN_DIR)/web-server

# Test sources
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/test_%.o,$(TEST_SOURCES))
TEST_TARGET = $(BIN_DIR)/test-runner

all: $(APP_TARGET)

$(APP_TARGET): $(APP_OBJECTS) $(OBJ_DIR)/main.o | $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

tests: $(TEST_TARGET)

$(TEST_TARGET): $(APP_OBJECTS) $(TEST_OBJECTS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)	
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: $(APP_TARGET)
	./$(APP_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

rebuild: clean all

.PHONY: all clean run rebuild test tests

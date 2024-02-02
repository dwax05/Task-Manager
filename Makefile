CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC_DIR = src
OUT_DIR = out
BIN_DIR = $(OUT_DIR)/bin

# List of source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate object file names
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OUT_DIR)/%.o, $(SRCS))

# Main executable name
EXE = $(BIN_DIR)/tskmngr

all: $(EXE)

$(EXE): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OUT_DIR)

.PHONY: all clean

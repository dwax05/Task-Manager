CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC = main.c tasks.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = tskmngr

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(EXECUTABLE) $(OBJ)

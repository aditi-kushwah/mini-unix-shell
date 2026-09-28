CC = gcc
CFLAGS = -Wall -Wextra

TARGET = mini-shell

SRC = main.c src/builtins.c src/parser.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
CC = gcc
CFLAGS = -Wall -I./include
SRC = src/main.c src/file_manager.c src/display.c src/cursor_text.c src/search_misc.c src/input_handler.c src/deque.c
OBJ = $(SRC:.c=.o)
TARGET = editor

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(CFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

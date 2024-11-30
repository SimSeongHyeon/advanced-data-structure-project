# �����Ϸ��� �÷��� ����
CC = gcc
CFLAGS = -Wall -I./lib/wincon -I./lib/common -I./src

# ���̺귯�� ����
ifeq ($(OS),Windows_NT)
    LDFLAGS = -L./lib/wincon -l:pdcurses.a
else
    LDFLAGS = -lncurses
endif

# �ҽ� ���� �� Ÿ�� ����
SRC = src/main.c src/cursor_text.c src/deque.c src/display.c src/file_manager.c src/input.c src/search.c src/line_node.c
OBJ = $(SRC:src/%.c=src/%.o)
TARGET = viva

# �⺻ ���� ���
all: $(TARGET)

# Ÿ�� ����
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(CFLAGS) $(LDFLAGS)

# ���� ������Ʈ ���� ����
src/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Ŭ�� ���
clean:
	rm -f src/*.o $(TARGET)
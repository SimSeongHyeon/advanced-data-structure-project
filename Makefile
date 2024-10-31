# �����Ϸ��� �÷��� ����
CC = gcc
CFLAGS = -Wall -I.

# �ҽ� ���� �� Ÿ�� ����
SRC = main.c deque.c file_manager.c display.c cursor_text.c input.c search.c
OBJ = $(SRC:.c=.o)
TARGET = editor

# �⺻ ���� ���
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(CFLAGS)

# ���� ������Ʈ ���� ����
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Ŭ�� ��� (������Ʈ ���� �� ���� ���� ����)
clean:
	rm -f $(OBJ) $(TARGET)

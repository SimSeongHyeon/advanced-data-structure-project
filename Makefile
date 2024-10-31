# 컴파일러와 플래그 설정
CC = gcc
CFLAGS = -Wall -I.

# 소스 파일 및 타겟 설정
SRC = main.c deque.c file_manager.c display.c cursor_text.c input.c search.c
OBJ = $(SRC:.c=.o)
TARGET = editor

# 기본 빌드 명령
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(CFLAGS)

# 개별 오브젝트 파일 빌드
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# 클린 명령 (오브젝트 파일 및 실행 파일 삭제)
clean:
	rm -f $(OBJ) $(TARGET)

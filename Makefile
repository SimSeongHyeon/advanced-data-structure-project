# 컴파일러와 플래그 설정
CC = gcc
CFLAGS = -Wall -I./lib/wincon -I./lib/common -I./src

# 라이브러리 설정
ifeq ($(OS),Windows_NT)
    LDFLAGS = -L./lib/wincon -l:pdcurses.a
else
    LDFLAGS = -lncurses
endif

# 소스 파일 및 타겟 설정
SRC = src/main.c src/cursor_text.c src/deque.c src/display.c src/file_manager.c src/input.c src/search.c src/line_node.c
OBJ = $(SRC:src/%.c=src/%.o)
TARGET = viva

# 기본 빌드 명령
all: $(TARGET)

# 타겟 빌드
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(CFLAGS) $(LDFLAGS)

# 개별 오브젝트 파일 빌드
src/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# 클린 명령
clean:
	rm -f src/*.o $(TARGET)
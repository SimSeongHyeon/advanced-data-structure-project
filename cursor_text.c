#include "cursor_text.h"
#include <stdlib.h>
#include <string.h>

Cursor cursor = { 0, 0 }; // 초기 커서 위치 (0,0)

// 텍스트 삽입 함수
void insertText(Deque* deque, const char* text, int row, int col) {
    Node* current = deque->head;
    for (int i = 0; i < row && current != NULL; i++) {
        current = current->next;
    }
    if (current == NULL) return;

    size_t newSize = strlen(current->line) + strlen(text) + 1;
    char* newLine = (char*)malloc(newSize);

    if (newLine != NULL) {  // 메모리 할당 성공 여부 확인
        strncpy_s(newLine, newSize, current->line, col);  // 첫 부분 복사
        strcat_s(newLine, newSize, text);                 // 삽입할 텍스트 추가
        strcat_s(newLine, newSize, current->line + col);  // 나머지 텍스트 추가

        free(current->line);
        current->line = newLine;
    }
}

// 커서 이동 함수
void moveCursor(int row, int col) {
    cursor.row = row;
    cursor.col = col;
}

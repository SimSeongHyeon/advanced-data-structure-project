#ifndef CURSOR_TEXT_H
#define CURSOR_TEXT_H

#include "deque.h"

// Cursor 구조체 선언
typedef struct Cursor {
    int row;
    int col;
} Cursor;

// 커서 변수 외부 선언
extern Cursor cursor;

void insertText(Deque* deque, const char* text, int row, int col);
void moveCursor(int row, int col);

#endif // CURSOR_TEXT_H

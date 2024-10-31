#ifndef CURSOR_TEXT_H
#define CURSOR_TEXT_H

#include "deque.h"

// Cursor ����ü ����
typedef struct Cursor {
    int row;
    int col;
} Cursor;

// Ŀ�� ���� �ܺ� ����
extern Cursor cursor;

void insertText(Deque* deque, const char* text, int row, int col);
void moveCursor(int row, int col);

#endif // CURSOR_TEXT_H

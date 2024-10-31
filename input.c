#include <stdio.h>
#include "input.h"
#include "cursor_text.h"     // Ŀ�� ����ü�� �ܺ� ���� cursor ��� ����

void handleInput(char input, Deque* deque) {
    switch (input) {
    case 'w':  // ���� �̵�
        if (cursor.row > 0) moveCursor(cursor.row - 1, cursor.col);
        break;
    case 's':  // �Ʒ��� �̵�
        moveCursor(cursor.row + 1, cursor.col);
        break;
    case 'a':  // �������� �̵�
        if (cursor.col > 0) moveCursor(cursor.row, cursor.col - 1);
        break;
    case 'd':  // ���������� �̵�
        moveCursor(cursor.row, cursor.col + 1);
        break;
    default:
        printf("Unrecognized input.\n");
        break;
    }
}

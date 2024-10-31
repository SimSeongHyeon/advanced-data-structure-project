#include <stdio.h>
#include "input.h"
#include "cursor_text.h"     // 커서 구조체와 외부 변수 cursor 사용 가능

void handleInput(char input, Deque* deque) {
    switch (input) {
    case 'w':  // 위로 이동
        if (cursor.row > 0) moveCursor(cursor.row - 1, cursor.col);
        break;
    case 's':  // 아래로 이동
        moveCursor(cursor.row + 1, cursor.col);
        break;
    case 'a':  // 왼쪽으로 이동
        if (cursor.col > 0) moveCursor(cursor.row, cursor.col - 1);
        break;
    case 'd':  // 오른쪽으로 이동
        moveCursor(cursor.row, cursor.col + 1);
        break;
    default:
        printf("Unrecognized input.\n");
        break;
    }
}

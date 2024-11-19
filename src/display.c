#include "display.h"
#include "os_terminal.h"
#include <stdio.h>

void display_text(Deque* deque) {
    Node* current = deque->head;
    int y = 0;
    while (current) {
        mvprintw(y++, 0, "%s", current->line);
        current = current->next;
    }
    refresh();
}

void update_status_bar(const char* filename, Deque* deque) {
    int total_lines = deque->size;
    Cursor cursor = get_cursor_position();
    int max_y, max_x;

    // 터미널 크기를 가져옴
    getmaxyx(stdscr, max_y, max_x);

    // 상태바에 표시할 문자열 생성
    char status[256];
    snprintf(status, sizeof(status), "File: %s | Lines: %d | Cursor: (%d, %d)",
        filename, total_lines, cursor.y, cursor.x);

    // 반전 색 활성화 후 상태 바 업데이트
    attron(A_REVERSE);
    mvprintw(max_y - 2, 0, "%-*s", max_x, status);  // 상태 바 내용 출력
    attroff(A_REVERSE);
    refresh();
}

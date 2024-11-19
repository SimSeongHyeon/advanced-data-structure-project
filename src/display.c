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

    // �͹̳� ũ�⸦ ������
    getmaxyx(stdscr, max_y, max_x);

    // ���¹ٿ� ǥ���� ���ڿ� ����
    char status[256];
    snprintf(status, sizeof(status), "File: %s | Lines: %d | Cursor: (%d, %d)",
        filename, total_lines, cursor.y, cursor.x);

    // ���� �� Ȱ��ȭ �� ���� �� ������Ʈ
    attron(A_REVERSE);
    mvprintw(max_y - 2, 0, "%-*s", max_x, status);  // ���� �� ���� ���
    attroff(A_REVERSE);
    refresh();
}

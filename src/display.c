#include "display.h"
#include "line_node.h"
#include "os_terminal.h"
#include <stdio.h>
#include <string.h>

void display_text(LineList* line_list) {
    LineNode* current = line_list->head;
    int y = 0;
    while (current && y < LINES - 2) { // ���¹ٿ� �޽����ٸ� ������ ����
        Node* left_current = current->left_deque->head;
        int x = 0; // x ��ġ �ʱ�ȭ
        while (left_current) {
            mvprintw(y, x++, "%c", left_current->data); // �� ���ڸ� ���
            left_current = left_current->next;
        }
        y++;
        current = current->next;
    }
    refresh();
}

void update_status_bar(const char* filename, LineList* line_list, int cursor_x, int cursor_y) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    if (max_y < 2) return; // �͹̳� ���̰� ������� ������ ���� �� ǥ�� ����

    char status[256];
    const char* display_filename = (filename && strlen(filename) > 0) ? filename : "No Name";

    snprintf(status, sizeof(status), "[%s] - %d lines | Cursor: %d,%d",
        display_filename, line_list->total_lines, cursor_y + 1, cursor_x + 1);

    attron(A_REVERSE);
    mvprintw(max_y - 2, 0, "%-*s", max_x, status); // ���� �� ���
    attroff(A_REVERSE);
    refresh();
}

void update_message_bar(const char* message) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    if (max_y < 1) return; // �͹̳� ���̰� ������� ������ �޽��� �� ǥ�� ����

    mvprintw(max_y - 1, 0, "%-*s", max_x, message ? message : ""); // �޽��� ���
    refresh();
}

void display_help_bar() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // ���� â ũ�� ��������
    
    mvprintw(max_y - 1, 0, "HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");
    for (int i = strlen("HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find"); i < max_x; i++) {
        mvaddch(max_y - 1, i, ' '); // ������ ������ ä���
    }
    refresh();
}

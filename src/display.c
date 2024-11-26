#include "display.h"
#include "line_node.h"
#include "os_terminal.h"
#include <stdio.h>

void display_text(LineList* line_list) {
    clear(); // ȭ�� �ʱ�ȭ

    // �ؽ�Ʈ ���
    LineNode* current = line_list->head;
    int y = 0;

    while (current) {
        Node* left_current = current->left_deque->head;
        int x = 0;

        while (left_current) {
            mvprintw(y, x++, "%c", left_current->data);
            left_current = left_current->next;
        }
        y++;
        current = current->next;
    }
    refresh(); // ȭ�� ���ΰ�ħ
}

void update_status_bar(const char* filename, LineList* line_list, int cursor_x, int cursor_y) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    if (max_y < 2) return; // �͹̳� ���̰� ������� ������ ���� �� ǥ�� ����

    char status[256];
    snprintf(status, sizeof(status), "[%s] - %d lines | Cursor: %d,%d",
        filename ? filename : "No Name",
        line_list ? line_list->total_lines : 0,
        cursor_y + 1, cursor_x + 1);

    attron(A_REVERSE); // ������ Ȱ��ȭ
    mvprintw(max_y - 2, 0, "%-*s", max_x, status); // ���� �� ���
    attroff(A_REVERSE); // ������ ��Ȱ��ȭ
}


void update_message_bar(const char* message) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    if (max_y < 1) return; // �͹̳� ���̰� ������� ������ �޽��� �� ǥ�� ����

    mvprintw(max_y - 1, 0, "%-*s", max_x, message ? message : ""); // �޽��� ���
    refresh();
}

#include "display.h"
#include "line_node.h"
#include "os_terminal.h"
#include <stdio.h>
#include <string.h>

void display_text(LineList* line_list, Cursor* cursor) {
    LineNode* current = line_list->head;
    int y = 0;

    while (current && y < LINES - 2) { // ���� �ٿ� �޽��� �ٸ� ������ ����
        Node* left_current = current->left_deque->head;
        int x = 0;

        // left_deque ���
        while (left_current) {
            if (y == cursor->y && x == cursor->x) {
                attron(A_REVERSE);
                mvprintw(y, x, "%c", left_current->data);
                attroff(A_REVERSE);
            }
            else {
                mvprintw(y, x, "%c", left_current->data);
            }
            left_current = left_current->next;
            x++;
        }

        // right_deque ���
        Node* right_current = current->right_deque->head;
        while (right_current) {
            if (y == cursor->y && x == cursor->x) {
                attron(A_REVERSE);
                mvprintw(y, x, "%c", right_current->data);
                attroff(A_REVERSE);
            }
            else {
                mvprintw(y, x, "%c", right_current->data);
            }
            right_current = right_current->next;
            x++;
        }

        // �� ������ Ŀ�� ���
        if (y == cursor->y && x == cursor->x) {
            attron(A_REVERSE);
            mvprintw(y, x, " ");
            attroff(A_REVERSE);
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
    mvprintw(max_y - 2, 0, "%-*s", max_x, status); // ���� �ٴ� �׻� �Ʒ����� �� ��° ���ο� ����
    attroff(A_REVERSE);
    refresh();
}

void update_message_bar(const char* message) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    move(max_y - 1, 0);  // ���� �Ʒ� �������� �̵�
    clrtoeol();  // ���� �޽��� ����
    mvprintw(max_y - 1, 0, "%-*s", max_x, message); // �޽����� ȭ�鿡 �°� ���
    refresh();

    napms(2000);  // 2�� ���� �޽��� ����

    // �޽��� ����
    move(max_y - 1, 0);
    clrtoeol();  // �޽��� ����
    refresh();
}


void display_help_bar() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    move(max_y - 1, 0);
    clrtoeol();

    // max_x�� Ȱ���� ���� �޽����� ȭ�� �ʺ� ���� ���
    char help_message[256];
    snprintf(help_message, sizeof(help_message), "HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");
    mvprintw(max_y - 1, 0, "%-*s", max_x, help_message);

    refresh();
}
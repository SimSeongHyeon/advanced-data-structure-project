#include "display.h"
#include "line_node.h"
#include "os_terminal.h"
#include <stdio.h>
#include <string.h>

void display_text(LineList* line_list) {
    LineNode* current = line_list->head;
    int y = 0;
    while (current && y < LINES - 2) { // 상태바와 메시지바를 제외한 영역
        Node* left_current = current->left_deque->head;
        int x = 0; // x 위치 초기화
        while (left_current) {
            mvprintw(y, x++, "%c", left_current->data); // 각 문자를 출력
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

    if (max_y < 2) return; // 터미널 높이가 충분하지 않으면 상태 바 표시 생략

    char status[256];
    const char* display_filename = (filename && strlen(filename) > 0) ? filename : "No Name";

    snprintf(status, sizeof(status), "[%s] - %d lines | Cursor: %d,%d",
        display_filename, line_list->total_lines, cursor_y + 1, cursor_x + 1);

    attron(A_REVERSE);
    mvprintw(max_y - 2, 0, "%-*s", max_x, status); // 상태 바 출력
    attroff(A_REVERSE);
    refresh();
}

void update_message_bar(const char* message) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    if (max_y < 1) return; // 터미널 높이가 충분하지 않으면 메시지 바 표시 생략

    mvprintw(max_y - 1, 0, "%-*s", max_x, message ? message : ""); // 메시지 출력
    refresh();
}

void display_help_bar() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // 현재 창 크기 가져오기
    
    mvprintw(max_y - 1, 0, "HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");
    for (int i = strlen("HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find"); i < max_x; i++) {
        mvaddch(max_y - 1, i, ' '); // 나머지 공간을 채우기
    }
    refresh();
}

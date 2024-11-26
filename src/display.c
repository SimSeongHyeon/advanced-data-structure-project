#include "display.h"
#include "line_node.h"
#include "os_terminal.h"
#include <stdio.h>

void display_text(LineList* line_list) {
    clear(); // 화면 초기화

    // 텍스트 출력
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
    refresh(); // 화면 새로고침
}

void update_status_bar(const char* filename, LineList* line_list, int cursor_x, int cursor_y) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    if (max_y < 2) return; // 터미널 높이가 충분하지 않으면 상태 바 표시 생략

    char status[256];
    snprintf(status, sizeof(status), "[%s] - %d lines | Cursor: %d,%d",
        filename ? filename : "No Name",
        line_list ? line_list->total_lines : 0,
        cursor_y + 1, cursor_x + 1);

    attron(A_REVERSE); // 반전색 활성화
    mvprintw(max_y - 2, 0, "%-*s", max_x, status); // 상태 바 출력
    attroff(A_REVERSE); // 반전색 비활성화
}


void update_message_bar(const char* message) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    if (max_y < 1) return; // 터미널 높이가 충분하지 않으면 메시지 바 표시 생략

    mvprintw(max_y - 1, 0, "%-*s", max_x, message ? message : ""); // 메시지 출력
    refresh();
}

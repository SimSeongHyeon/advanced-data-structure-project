#include "display.h"
#include "line_node.h"
#include "os_terminal.h"
#include "search.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void display_text(LineList* line_list, Cursor* cursor) {
    LineNode* current = line_list->head;
    int y = 0;

    while (current && y < LINES - 2) { // 상태 바와 메시지 바를 제외한 영역
        Node* left_current = current->left_deque->head;
        int x = 0;

        // left_deque 출력
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

        // right_deque 출력
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

        // 빈 공간에 커서 출력
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

    if (max_y < 2) return; // 터미널 높이가 충분하지 않으면 상태 바 표시 생략

    char status[256];
    const char* display_filename = (filename && strlen(filename) > 0) ? filename : "Untitled";

    snprintf(status, sizeof(status), "[%s] - %d lines | Cursor: %d,%d",
        display_filename, line_list->total_lines, cursor_y + 1, cursor_x + 1);

    attron(A_REVERSE);
    mvprintw(max_y - 2, 0, "%-*s", max_x, status); // 상태 바는 항상 아래에서 두 번째 라인에 유지
    attroff(A_REVERSE);
    refresh();
}

void update_message_bar(const char* message) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    move(max_y - 1, 0);  // 가장 아래 라인으로 이동
    clrtoeol();  // 기존 메시지 삭제
    mvprintw(max_y - 1, 0, "%-*s", max_x, message); // 메시지를 화면에 맞게 출력
    refresh();

    // 메시지 유지: 삭제는 명시적인 호출로만 수행
}

void display_help_bar() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    move(max_y - 1, 0);
    clrtoeol();

    // max_x를 활용해 도움말 메시지를 화면 너비에 맞춰 출력
    char help_message[256];
    snprintf(help_message, sizeof(help_message), "HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");
    mvprintw(max_y - 1, 0, "%-*s", max_x, help_message);

    refresh();
}

void highlight_search_result(LineList* line_list, Cursor* cursor, const char* search_term) {
    LineNode* current = line_list->head;
    int y = 0;

    while (current && y < LINES - 2) { // 상태 바와 메시지 바를 제외한 영역
        char* line_text = dequeToString(current->left_deque);
        if (!line_text) {
            current = current->next;
            y++;
            continue;
        }

        const char* match = strstr(line_text, search_term); // 검색어 찾기
        int x = 0;

        // 라인의 모든 매칭 위치를 하이라이트
        while (match) {
            x = match - line_text;

            // 매칭된 텍스트 하이라이트
            for (int i = 0; i < strlen(search_term); i++) {
                if (y == cursor->y && (x + i) == cursor->x) {
                    // 커서와 겹치면 커서의 반전 우선
                    attron(A_REVERSE);
                    mvprintw(y, x + i, "%c", line_text[x + i]);
                    attroff(A_REVERSE);
                }
                else {
                    // 일반 하이라이트
                    attron(A_STANDOUT);
                    mvprintw(y, x + i, "%c", line_text[x + i]);
                    attroff(A_STANDOUT);
                }
            }

            match = strstr(match + 1, search_term); // 다음 매칭 검색
        }

        free(line_text);
        current = current->next;
        y++;
    }
    refresh();
}

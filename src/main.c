#include <stdio.h>
#include <string.h>
#include "line_node.h"
#include "file_manager.h"
#include "display.h"
#include "input.h"
#include "os_terminal.h"

// to-do
// back-space 구현
// ctrl-f 구현

// 전역 변수 선언
char filename[256] = { '\0' }; // 프로그램 전체에서 사용할 파일 이름

int main(int argc, char* argv[]) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    LineList* line_list = NULL;

    if (argc == 1) { // 새로운 파일 생성
        line_list = init_line_list();
        if (!line_list) {
            endwin();
            fprintf(stderr, "Failed to initialize a new file.\n");
            return 1;
        }

        // 새로운 파일에 기본 빈 라인 추가
        LineNode* new_line = create_line_node();
        if (!new_line) {
            free_line_list(line_list);
            endwin();
            fprintf(stderr, "Failed to initialize a new line.\n");
            return 1;
        }
        append_line(line_list, new_line); // 빈 라인을 리스트에 추가
        update_message_bar("New file created. Press Ctrl+S to save.");
    }
    else if (argc == 2) { // 기존 파일 열기
        strncpy(filename, argv[1], sizeof(filename)); // 전역 변수에 파일 이름 저장
        line_list = load_file(filename);
        if (!line_list) {
            endwin();
            fprintf(stderr, "Error: File '%s' not found.\n", filename);
            return 1;
        }
    }
    else { // 잘못된 명령어 사용
        endwin();
        fprintf(stderr, "Usage: ./viva [filename]\n");
        return 1;
    }

    int cursor_x = 0, cursor_y = 0;

    // 기존 파일 내용 표시
    clear();
    display_text(line_list);
    update_status_bar(filename, line_list, cursor_x, cursor_y);
    display_help_bar();
    refresh();

    // 입력 처리
    handle_input(line_list, line_list->head, &cursor_x, &cursor_y);

    // 메모리 정리 및 종료
    free_line_list(line_list);
    endwin();
    return 0;
}
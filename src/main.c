#include <stdio.h>
#include <string.h>
#include "line_node.h"
#include "file_manager.h"
#include "display.h"
#include "input.h"
#include "os_terminal.h"

// to do
// 커서 구현
// 방향키, 백스페이스 구현
// 저장 안 하고 종료시 경고 메시지 유지
// 저장 시 기존 파일 있으면 이미 있다는 경고 출력


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

        LineNode* new_line = create_line_node();
        if (!new_line) {
            free_line_list(line_list);
            endwin();
            fprintf(stderr, "Failed to initialize a new line.\n");
            return 1;
        }
        append_line(line_list, new_line);
    }
    else if (argc == 2) { // 기존 파일 열기
        strncpy(filename, argv[1], sizeof(filename));
        line_list = load_file(filename);
        if (!line_list) {
            endwin();
            fprintf(stderr, "Error: File '%s' not found.\n", filename);
            return 1;
        }
    }
    else {
        endwin();
        fprintf(stderr, "Usage: ./viva [filename]\n");
        return 1;
    }

    int cursor_x = 0, cursor_y = 0;

    // 화면 초기화
    clear();
    display_text(line_list);
    update_status_bar(filename, line_list, cursor_x, cursor_y);
    display_help_bar(); // Help Bar 추가
    refresh();

    handle_input(line_list, line_list->head, &cursor_x, &cursor_y, filename);

    free_line_list(line_list);
    endwin();
    return 0;
}

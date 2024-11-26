#include <stdio.h>
#include <string.h>
#include "line_node.h"
#include "file_manager.h"
#include "display.h"
#include "input.h"
#include "os_terminal.h"

int main(int argc, char* argv[]) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    char filename[256];
    strncpy(filename, (argc >= 2) ? argv[1] : "untitled.txt", sizeof(filename));
    filename[sizeof(filename) - 1] = '\0';

    LineList* line_list = load_file(filename);
    if (!line_list || !line_list->head) {
        endwin();
        return 1;
    }

    int cursor_x = 0, cursor_y = 0;

    // 입력 처리
    handle_input(line_list, line_list->head, &cursor_x, &cursor_y, filename);

    // 메모리 정리 및 종료
    free_line_list(line_list);
    endwin();
    return 0;
}


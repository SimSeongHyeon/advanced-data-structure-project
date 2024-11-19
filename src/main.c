#include <stdio.h>
#include "file_manager.h"
#include "display.h"
#include "input.h"
#include "cursor_text.h"
#include "deque.h"
#include <string.h>
#include "os_terminal.h"

int main(int argc, char* argv[]) {
    char filename[256];

    if (argc >= 2) {
        strncpy(filename, argv[1], sizeof(filename) - 1);  // 명령줄 인수로 파일이 제공된 경우 사용
        filename[sizeof(filename) - 1] = '\0';
    }
    else {
        strncpy(filename, "untitled.txt", sizeof(filename) - 1);
        filename[sizeof(filename) - 1] = '\0';
        generate_unique_filename(filename);  // 중복되지 않는 이름 생성
    }

    Deque* deque = load_file(filename);
    if (!deque) {
        printf("Error loading file: %s\n", filename);
        return 1;
    }

    initscr();           // PDCurses 초기화
    curs_set(1);         // 커서 표시
    keypad(stdscr, TRUE);
    noecho();

    display_text(deque);
    update_status_bar(filename, deque);  // 상태 바 초기 표시

    handle_input();      // 입력 처리

    save_file(filename, deque);
    free_deque(deque);

    endwin();            // PDCurses 종료
    return 0;
}

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
        strncpy(filename, argv[1], sizeof(filename) - 1);  // ����� �μ��� ������ ������ ��� ���
        filename[sizeof(filename) - 1] = '\0';
    }
    else {
        strncpy(filename, "untitled.txt", sizeof(filename) - 1);
        filename[sizeof(filename) - 1] = '\0';
        generate_unique_filename(filename);  // �ߺ����� �ʴ� �̸� ����
    }

    Deque* deque = load_file(filename);
    if (!deque) {
        printf("Error loading file: %s\n", filename);
        return 1;
    }

    initscr();           // PDCurses �ʱ�ȭ
    curs_set(1);         // Ŀ�� ǥ��
    keypad(stdscr, TRUE);
    noecho();

    display_text(deque);
    update_status_bar(filename, deque);  // ���� �� �ʱ� ǥ��

    handle_input();      // �Է� ó��

    save_file(filename, deque);
    free_deque(deque);

    endwin();            // PDCurses ����
    return 0;
}

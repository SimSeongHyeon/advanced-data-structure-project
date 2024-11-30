#include <stdio.h>
#include <string.h>
#include "line_node.h"
#include "file_manager.h"
#include "display.h"
#include "input.h"
#include "os_terminal.h"

// to do
// Ŀ�� ����
// ����Ű, �齺���̽� ����
// ���� �� �ϰ� ����� ��� �޽��� ����
// ���� �� ���� ���� ������ �̹� �ִٴ� ��� ���
// ctrl-q��  ��� ��� ���ֱ�


// ���� ���� ����
char filename[256] = { '\0' }; // ���α׷� ��ü���� ����� ���� �̸�

int main(int argc, char* argv[]) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    LineList* line_list = NULL;
    Cursor cursor = { 0, 0 }; // Ŀ�� �ʱ�ȭ

    if (argc == 1) { // ���ο� ���� ����
        line_list = init_line_list();
        if (!line_list) {
            endwin();
            fprintf(stderr, "Failed to initialize a new file.\n");
            return 1;
        }

        // ���ο� ���Ͽ� �⺻ �� ���� �߰�
        LineNode* new_line = create_line_node();
        if (!new_line) {
            free_line_list(line_list);
            endwin();
            fprintf(stderr, "Failed to initialize a new line.\n");
            return 1;
        }
        append_line(line_list, new_line); // �� ������ ����Ʈ�� �߰�
    }
    else if (argc == 2) { // ���� ���� ����
        strncpy(filename, argv[1], sizeof(filename));
        line_list = load_file(filename);
        if (!line_list) {
            endwin();
            fprintf(stderr, "Error: File '%s' not found.\n", filename);
            return 1;
        }
    }
    else { // �߸��� ��ɾ� ���
        endwin();
        fprintf(stderr, "Usage: ./viva [filename]\n");
        return 1;
    }
    int cursor_x = 0, cursor_y = 0;


    // ȭ�� �ʱ�ȭ
    clear();
    display_text(line_list, &cursor); // Ŀ�� ������ �Բ� ����
    update_status_bar(filename, line_list, cursor_x, cursor_y);
    display_help_bar(); // Help Bar �߰�
    refresh();

    handle_input(line_list, line_list->head, &cursor_x, &cursor_y, filename);

    free_line_list(line_list);
    endwin();
    return 0;
}

#include "input.h"
#include "line_node.h"
#include "cursor_text.h"
#include "os_terminal.h"
#include "display.h"
#include "file_manager.h"
#include "search.h"
#include "globals.h"
#include <string.h>

int confirm_exit(int* unsaved_changes) {
    if (*unsaved_changes) { // ������� ���� ���� ������ ���� ���
        update_message_bar("Unsaved changes! Press Ctrl+Q again to quit without saving.");
        int ch = getch(); // ����� �Է� ���
        if (ch == 17) {   // Ctrl+Q�� �ٽ� ������
            return 1;     // ���� ��ȣ ��ȯ
        }
        update_message_bar("Exit cancelled."); // �ٸ� Ű�� ������ ���
        return 0;         // ���� ���
    }
    return 1; // ����� ���¸� �ٷ� ����
}

void save_file_with_prompt(LineList* line_list) {
    // ���� filename�� ��� ������ ����ڷκ��� �Է¹ޱ�
    if (strlen(filename) == 0) {
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);

        // �޽��� �ٿ� "Enter filename to save: " ���
        move(max_y - 1, 0); // �޽��� �� ��ġ
        clrtoeol(); // ���� �޽��� ����
        mvprintw(max_y - 1, 0, "Enter filename to save: ");

        // max_x�� Ȱ���� Ŀ�� ��ġ ����
        mvprintw(max_y - 1, strlen("Enter filename to save: "), "%-*s", max_x - strlen("Enter filename to save: "), filename);
        refresh();

        echo();  // �Է��� ������ ȭ�鿡 ǥ��
        mvgetstr(max_y - 1, strlen("Enter filename to save: "), filename);  // �޽��� ������ �Է¹ޱ�
        noecho(); // �Է� �� �ٽ� �����
    }

    // ���� ����
    save_file(filename, line_list);  // ���� filename�� ����Ͽ� ���� ����

    // ���� ���� �޽��� ���
    update_message_bar("File saved successfully!");  // ���� ���� �޽��� ���
}

void handle_input(LineList* line_list, LineNode* current_line, int* cursor_x, int* cursor_y, char* filename) {
    int unsaved_changes = 0;
    int ch;

    while (1) {
        ch = getch();
        if (ch == ERR) {
            continue;
        }

        switch (ch) {
        case 19: // Ctrl+S
            save_file_with_prompt(line_list);
            unsaved_changes = 0;
            break;

        case 17: // Ctrl+Q
            if (confirm_exit(&unsaved_changes)) {
                update_message_bar("Exiting...");
                napms(1000);
                return;
            }
            break;

        case '\n':
        case KEY_ENTER: {
            LineNode* new_line = create_line_node();
            if (!new_line) {
                update_message_bar("Error: Unable to create new line.");
                continue;
            }
            while (current_line->right_deque->size > 0) {
                char c = pop_front(current_line->right_deque);
                push_back(new_line->left_deque, c);
            }
            new_line->prev = current_line;
            new_line->next = current_line->next;
            if (current_line->next) {
                current_line->next->prev = new_line;
            }
            current_line->next = new_line;

            current_line = new_line;
            *cursor_x = 0;
            *cursor_y += 1;
            unsaved_changes = 1;
        }
                      break;

        default:
            push_back(current_line->left_deque, ch);
            (*cursor_x)++;
            unsaved_changes = 1;
            break;
        }

        clear();
        display_text(line_list);
        update_status_bar(filename, line_list, *cursor_x, *cursor_y);
        display_help_bar();
        refresh();
    }
}

#include "input.h"
#include "line_node.h"
#include "cursor_text.h"
#include "os_terminal.h"
#include "display.h"
#include "file_manager.h"
#include "search.h"
#include "globals.h"
#include <string.h>
#include <stdlib.h>

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
    int unsaved_changes = 0; // ���� ���� ���� ����
    int ch;
    Cursor cursor = { *cursor_x, *cursor_y }; // �ʱ� Ŀ�� ��ġ

    while (1) {
        ch = getch(); // ����� �Է� ���

        if (ch == ERR) { // �Է��� ������ continue
            continue;
        }

        switch (ch) {
        case 19: // Ctrl+S (Save)
            save_file_with_prompt(line_list); // ���� ����
            unsaved_changes = 0; // ���� �� ���� ���� �ʱ�ȭ
            break;

        case 17: // Ctrl+Q (Quit)
            if (unsaved_changes) {
                if (confirm_exit(&unsaved_changes)) {
                    return; // ���α׷� ����
                }
            }
            else {
                return; // ���α׷� ����
            }
            break;

        case KEY_UP: // ���� �̵�
            move_cursor(0, -1, &current_line, &cursor, line_list);
            break;

        case KEY_DOWN: // �Ʒ��� �̵�
            move_cursor(0, 1, &current_line, &cursor, line_list);
            break;

        case KEY_LEFT: // �������� �̵�
            move_cursor(-1, 0, &current_line, &cursor, line_list);
            break;

        case KEY_RIGHT: // ���������� �̵�
            move_cursor(1, 0, &current_line, &cursor, line_list);
            break;

        case KEY_PPAGE:
        case 451: // Page Up ó��
            current_line = line_list->head;
            cursor.x = 0; // ù ��° �ؽ�Ʈ�� ����
            cursor.y = 0; // ù ��° ������ y ��ġ
            break;

        case KEY_NPAGE: // Page Down ó��
        case 457:
            current_line = line_list->tail;
            cursor.x = current_line->left_deque->size + current_line->right_deque->size; // ������ �ؽ�Ʈ ��
            cursor.y = line_list->total_lines - 1; // ������ ������ y ��ġ
            break;

        case KEY_BACKSPACE: // Backspace ó��
        case 127:          // ASCII DEL
        case 8: {          // ASCII Backspace
            if (cursor.x > 0) {
                char deleted = pop_back(current_line->left_deque);
                if (deleted) {
                    cursor.x--;
                    unsaved_changes = 1;
                }
            }
            else if (current_line->prev) {
                LineNode* prev_line = current_line->prev;
                cursor.x = prev_line->left_deque->size + prev_line->right_deque->size;

                while (current_line->left_deque->size > 0) {
                    push_back(prev_line->left_deque, pop_front(current_line->left_deque));
                }
                while (current_line->right_deque->size > 0) {
                    push_back(prev_line->left_deque, pop_front(current_line->right_deque));
                }

                prev_line->next = current_line->next;
                if (current_line->next) {
                    current_line->next->prev = prev_line;
                }
                free(current_line);
                current_line = prev_line;
                cursor.y--;
                unsaved_changes = 1;
            }
        }
              break;

        case '\n': { // �� �� ����
            LineNode* new_line = create_line_node();
            if (!new_line) {
                update_message_bar("Error: Unable to create new line.");
                continue;
            }
            while (current_line->right_deque->size > 0) {
                char c = pop_front(current_line->right_deque);
                push_back(new_line->left_deque, c);
            }
            current_line->next = new_line;
            new_line->prev = current_line;
            current_line = new_line;
            cursor.x = 0;
            cursor.y += 1;
            unsaved_changes = 1;
        }
                 break;

        default:
            push_back(current_line->left_deque, ch);
            cursor.x++;
            unsaved_changes = 1;
            break;
        }

        // ȭ�� ����
        clear();
        display_text(line_list, &cursor);
        update_status_bar(filename, line_list, cursor.x, cursor.y);
        display_help_bar(); // Help Bar�� �׻� ǥ��
        refresh();
    }
}

#include "input.h"
#include "line_node.h"
#include "cursor_text.h"
#include "os_terminal.h" // ncurses �Ǵ� PDCurses ����
#include "display.h"
#include "file_manager.h"
#include "search.h"
#include <string.h>

void save_file_with_prompt(LineList* line_list, const char* current_filename) {
    char filename[256];
    if (!current_filename || strlen(current_filename) == 0) {
        echo();
        update_message_bar("Enter file name to save: ");
        mvgetstr(LINES - 1, 25, filename); // �޽��� �ٿ��� ���� �̸� �Է�
        noecho();
    }
    else {
        strncpy(filename, current_filename, sizeof(filename));
    }

    save_file(filename, line_list);
    update_message_bar("File saved successfully! Press any key to continue.");
    getch(); // ����� �Է� ���
    update_message_bar(""); // �޽��� �� �ʱ�ȭ
}

void search_and_highlight(LineList* line_list, int* cursor_x, int* cursor_y) {
    char keyword[256];
    echo();
    update_message_bar("Enter search keyword: ");
    mvgetstr(LINES - 1, 21, keyword); // �޽��� �ٿ��� Ű���� �Է�
    noecho();

    int found_line = searchInDeque(line_list, keyword);
    if (found_line < 0) {
        update_message_bar("Keyword not found!");
        return;
    }

    LineNode* current_line = line_list->head;
    for (int i = 0; i < found_line; i++) {
        current_line = current_line->next;
    }

    *cursor_y = found_line;
    *cursor_x = 0; // �⺻������ ù ��ġ�� Ŀ�� �̵�

    update_message_bar("Use arrow keys to navigate, Enter to edit, ESC to cancel");

    while (1) {
        int ch = getch();
        switch (ch) {
        case KEY_DOWN:
            if (current_line->next) {
                current_line = current_line->next;
                (*cursor_y)++;
            }
            break;
        case KEY_UP:
            if (current_line->prev) {
                current_line = current_line->prev;
                (*cursor_y)--;
            }
            break;
        case 10: // Enter
            update_message_bar("Exiting search mode...");
            return;
        case 27: // ESC
            update_message_bar("Search cancelled");
            return;
        }
        clear();
        display_text(line_list); // �ؽ�Ʈ ǥ��
        mvprintw(*cursor_y, *cursor_x, "["); // �˻��� ��ġ ���̶���Ʈ
        refresh();
    }
}

int confirm_exit(int* unsaved_changes) {
    if (*unsaved_changes) {
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

void handle_input(LineList* line_list, LineNode* current_line, int* cursor_x, int* cursor_y, const char* filename) {
    int unsaved_changes = 0; // ���� ���� ���� ����
    int ch;
    while ((ch = getch())) {
        switch (ch) {
        case 19: // Ctrl+S (Save)
            save_file_with_prompt(line_list, filename);
            unsaved_changes = 0; // ���� ���� �ʱ�ȭ
            break;

        case 17: // Ctrl+Q (Quit)
            if (confirm_exit(&unsaved_changes)) {
                return; // ���α׷� ����
            }
            break;

        case KEY_LEFT:
            if (current_line->left_deque->size > 0) {
                char c = pop_back(current_line->left_deque);
                push_front(current_line->right_deque, c);
                (*cursor_x) = (*cursor_x > 0) ? *cursor_x - 1 : 0;
            }
            break;

        case KEY_RIGHT:
            if (current_line->right_deque->size > 0) {
                char c = pop_front(current_line->right_deque);
                push_back(current_line->left_deque, c);
                (*cursor_x) = (*cursor_x < COLS - 1) ? *cursor_x + 1 : COLS - 1;
            }
            break;

        case KEY_DOWN:
            if (current_line->next && *cursor_y < LINES - 3) {
                current_line = current_line->next;
                (*cursor_y)++;
            }
            break;

        case KEY_UP:
            if (current_line->prev && *cursor_y > 0) {
                current_line = current_line->prev;
                (*cursor_y)--;
            }
            break;

        default:
            push_back(current_line->left_deque, ch);
            (*cursor_x)++;
            unsaved_changes = 1; // ���� ���� ���
        }

        clear();
        display_text(line_list);
        update_status_bar(filename, line_list, *cursor_x, *cursor_y);
        refresh();
    }
}

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

int confirm_exit(int* unsaved_changes, LineList* line_list) {
    update_message_bar("Changes have not been saved. To exit without saving, press Ctrl-Q again.");

    while (1) {
        int ch = getch();
        switch (ch) {
        case 17: // Ctrl-Q
            return 1;

        case 19: // Ctrl-S
            save_file_with_prompt(line_list);
            update_message_bar("File saved successfully!");
            napms(2000);
            update_message_bar("");
            return 0;

        default:
            update_message_bar("");
            return 0;
        }
    }
}

void save_file_with_prompt(LineList* line_list) {
    if (strlen(filename) == 0) {
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);

        move(max_y - 1, 0);
        clrtoeol();
        mvprintw(max_y - 1, 0, "Enter filename to save: ");
        mvprintw(max_y - 1, strlen("Enter filename to save: "), "%-*s", max_x - strlen("Enter filename to save: "), filename);
        refresh();

        echo();
        mvgetstr(max_y - 1, strlen("Enter filename to save: "), filename);
        noecho();
    }

    save_file(filename, line_list);
    update_message_bar("File saved successfully!");
}

void handle_input(LineList* line_list, LineNode* current_line, int* cursor_x, int* cursor_y, char* filename) {
    int unsaved_changes = 0;
    int ch;
    Cursor cursor = { *cursor_x, *cursor_y };
    char search_term[256] = { 0 };
    int search_mode = 0;

    while (1) {
        ch = getch();

        if (ch == ERR) continue;

        if (search_mode) {
            switch (ch) {
            case '\n':
                search_mode = 0;
                update_message_bar("");
                break;

            case KEY_RIGHT:
                move_to_next_result(line_list, &cursor, search_term);
                break;

            case KEY_LEFT:
                move_to_previous_result(line_list, &cursor, search_term);
                break;

            case 27:
                search_mode = 0;
                update_message_bar("");
                if (cursor.y >= line_list->total_lines) {
                    cursor.y = line_list->total_lines - 1;
                }
                current_line = line_list->head;
                for (int i = 0; i < cursor.y; i++) {
                    if (current_line->next) {
                        current_line = current_line->next;
                    }
                }

                int line_length = current_line->left_deque->size + current_line->right_deque->size;
                if (cursor.x > line_length) {
                    cursor.x = line_length;
                }

                clear();
                display_text(line_list, &cursor);
                update_status_bar(filename, line_list, cursor.x, cursor.y);
                display_help_bar();
                refresh();
                break;

            default:
                break;
            }

            continue;
        }

        switch (ch) {
        case 19:
            save_file_with_prompt(line_list);
            unsaved_changes = 0;
            update_message_bar("File saved successfully!");
            napms(2000);
            update_message_bar("");
            break;

        case 17:
            if (unsaved_changes) {
                int exit_result = confirm_exit(&unsaved_changes, line_list);
                if (exit_result == 1) return;
            }
            else {
                return;
            }
            break;

        case 6:
            get_search_term(search_term);
            if (find_next(line_list, &cursor, search_term)) {
                search_mode = 1;
                update_message_bar("Search mode activated. Use arrows to navigate, Esc to exit.");
                clear();
                display_text(line_list, &cursor);
                update_status_bar(filename, line_list, cursor.x, cursor.y);
                refresh();
            }
            else {
                update_message_bar("No matches found.");
            }
            break;

        case KEY_UP:
            move_cursor(0, -1, &current_line, &cursor, line_list);
            break;

        case KEY_DOWN:
            move_cursor(0, 1, &current_line, &cursor, line_list);
            break;

        case KEY_LEFT:
            move_cursor(-1, 0, &current_line, &cursor, line_list);
            break;

        case KEY_RIGHT:
            move_cursor(1, 0, &current_line, &cursor, line_list);
            break;

        case KEY_PPAGE:
        case 451:
            current_line = line_list->head;
            cursor.x = 0;
            cursor.y = 0;
            break;

        case KEY_NPAGE:
        case 457:
            current_line = line_list->tail;
            cursor.x = current_line->left_deque->size + current_line->right_deque->size;
            cursor.y = line_list->total_lines - 1;
            break;

        case KEY_BACKSPACE:
        case 127:
        case 8:
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
            break;

        case '\n': {
            LineNode* new_line = create_line_node();
            if (!new_line) {
                update_message_bar("Error: Unable to create new line.");
                continue;
            }

            while (cursor.x < current_line->left_deque->size) {
                char c = pop_back(current_line->left_deque);
                push_front(current_line->right_deque, c);
            }

            while (current_line->right_deque->size > 0) {
                char c = pop_front(current_line->right_deque);
                push_back(new_line->left_deque, c);
            }

            new_line->next = current_line->next;
            new_line->prev = current_line;
            if (current_line->next) {
                current_line->next->prev = new_line;
            }
            current_line->next = new_line;

            if (line_list->tail == current_line) {
                line_list->tail = new_line;
            }

            current_line = new_line;
            cursor.x = 0;
            cursor.y++;
            line_list->total_lines++;
            unsaved_changes = 1;
        }
                 break;

        default:
            if (cursor.x == current_line->left_deque->size) {
                push_back(current_line->left_deque, ch);
            }
            else if (cursor.x < current_line->left_deque->size) {
                while (cursor.x < current_line->left_deque->size) {
                    char temp = pop_back(current_line->left_deque);
                    push_front(current_line->right_deque, temp);
                }
                push_back(current_line->left_deque, ch);
            }
            else {
                int right_offset = cursor.x - current_line->left_deque->size;
                while (right_offset-- > 0) {
                    char temp = pop_front(current_line->right_deque);
                    push_back(current_line->left_deque, temp);
                }
                push_back(current_line->left_deque, ch);
            }
            cursor.x++;
            unsaved_changes = 1;
            break;
        }

        clear();
        display_text(line_list, &cursor);
        update_status_bar(filename, line_list, cursor.x, cursor.y);
        display_help_bar();
        refresh();
    }
}

void get_search_term(char* search_term) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    mvprintw(max_y - 1, 0, "Enter search term: ");
    clrtoeol();
    refresh();
    echo();
    getnstr(search_term, max_x - 20);
    noecho();
}

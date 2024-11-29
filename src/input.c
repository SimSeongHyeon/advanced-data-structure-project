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
    if (*unsaved_changes) { // 저장되지 않은 변경 사항이 있을 경우
        update_message_bar("Unsaved changes! Press Ctrl+Q again to quit without saving.");
        int ch = getch(); // 사용자 입력 대기
        if (ch == 17) {   // Ctrl+Q가 다시 눌리면
            return 1;     // 종료 신호 반환
        }
        update_message_bar("Exit cancelled."); // 다른 키가 눌리면 취소
        return 0;         // 종료 취소
    }
    return 1; // 저장된 상태면 바로 종료
}

void save_file_with_prompt(LineList* line_list) {
    // 전역 filename이 비어 있으면 사용자로부터 입력받기
    if (strlen(filename) == 0) {
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);

        // 메시지 바에 "Enter filename to save: " 출력
        move(max_y - 1, 0); // 메시지 바 위치
        clrtoeol(); // 기존 메시지 삭제
        mvprintw(max_y - 1, 0, "Enter filename to save: ");

        // max_x를 활용해 커서 위치 설정
        mvprintw(max_y - 1, strlen("Enter filename to save: "), "%-*s", max_x - strlen("Enter filename to save: "), filename);
        refresh();

        echo();  // 입력한 내용을 화면에 표시
        mvgetstr(max_y - 1, strlen("Enter filename to save: "), filename);  // 메시지 옆에서 입력받기
        noecho(); // 입력 후 다시 숨기기
    }

    // 파일 저장
    save_file(filename, line_list);  // 전역 filename을 사용하여 파일 저장

    // 저장 성공 메시지 출력
    update_message_bar("File saved successfully!");  // 저장 성공 메시지 출력
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

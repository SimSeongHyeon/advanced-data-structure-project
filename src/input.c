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
    int unsaved_changes = 0; // 변경 사항 추적 변수
    int ch;
    Cursor cursor = { *cursor_x, *cursor_y }; // 초기 커서 위치

    while (1) {
        ch = getch(); // 사용자 입력 대기

        if (ch == ERR) { // 입력이 없으면 continue
            continue;
        }

        switch (ch) {
        case 19: // Ctrl+S (Save)
            save_file_with_prompt(line_list); // 파일 저장
            unsaved_changes = 0; // 저장 후 변경 상태 초기화
            break;

        case 17: // Ctrl+Q (Quit)
            if (unsaved_changes) {
                if (confirm_exit(&unsaved_changes)) {
                    return; // 프로그램 종료
                }
            }
            else {
                return; // 프로그램 종료
            }
            break;

        case KEY_UP: // 위로 이동
            move_cursor(0, -1, &current_line, &cursor, line_list);
            break;

        case KEY_DOWN: // 아래로 이동
            move_cursor(0, 1, &current_line, &cursor, line_list);
            break;

        case KEY_LEFT: // 왼쪽으로 이동
            move_cursor(-1, 0, &current_line, &cursor, line_list);
            break;

        case KEY_RIGHT: // 오른쪽으로 이동
            move_cursor(1, 0, &current_line, &cursor, line_list);
            break;

        case KEY_PPAGE:
        case 451: // Page Up 처리
            current_line = line_list->head;
            cursor.x = 0; // 첫 번째 텍스트의 시작
            cursor.y = 0; // 첫 번째 라인의 y 위치
            break;

        case KEY_NPAGE: // Page Down 처리
        case 457:
            current_line = line_list->tail;
            cursor.x = current_line->left_deque->size + current_line->right_deque->size; // 마지막 텍스트 끝
            cursor.y = line_list->total_lines - 1; // 마지막 라인의 y 위치
            break;

        case KEY_BACKSPACE: // Backspace 처리
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

        case '\n': { // 새 줄 생성
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

        // 화면 갱신
        clear();
        display_text(line_list, &cursor);
        update_status_bar(filename, line_list, cursor.x, cursor.y);
        display_help_bar(); // Help Bar를 항상 표시
        refresh();
    }
}

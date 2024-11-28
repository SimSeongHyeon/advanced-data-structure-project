#include "input.h"
#include "line_node.h"
#include "cursor_text.h"
#include "os_terminal.h" // ncurses 또는 PDCurses 포함
#include "display.h"
#include "file_manager.h"
#include "search.h"
#include "globals.h"
#include <string.h>

void save_file_with_prompt(LineList* line_list) {
    if (strlen(filename) == 0) { // 파일 이름이 없는 경우 입력받음
        echo();
        update_message_bar("Enter file name to save: ");
        mvgetstr(LINES - 1, 25, filename); // 메시지 바에서 파일 이름 입력
        noecho();
    }

    save_file(filename, line_list);
    update_message_bar("File saved successfully! Press any key to continue.");
    getch(); // 사용자 입력 대기
    update_message_bar(""); // 메시지 바 초기화
}

void search_and_highlight(LineList* line_list, int* cursor_x, int* cursor_y) {
    char keyword[256];
    echo();
    update_message_bar("Enter search keyword: ");
    mvgetstr(LINES - 1, 21, keyword); // 메시지 바에서 키워드 입력
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
    *cursor_x = 0; // 기본적으로 첫 위치로 커서 이동

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
        display_text(line_list); // 텍스트 표시
        mvprintw(*cursor_y, *cursor_x, "["); // 검색된 위치 하이라이트
        refresh();
    }
}

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

void handle_input(LineList* line_list, LineNode* current_line, int* cursor_x, int* cursor_y) {
    int unsaved_changes = 0; // 변경 사항 추적 변수
    int ch;

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
            if (confirm_exit(&unsaved_changes)) {
                return; // 프로그램 종료
            }
            break;

        case '\n': // 줄바꿈 처리
        case KEY_ENTER: {
            // 새로운 줄 생성
            LineNode* new_line = create_line_node();
            if (!new_line) {
                update_message_bar("Error: Unable to create new line.");
                continue;
            }

            // 현재 줄의 오른쪽 덱 내용을 새 줄로 이동
            while (current_line->right_deque->size > 0) {
                char c = pop_front(current_line->right_deque);
                push_back(new_line->left_deque, c);
            }

            // 새로운 줄을 현재 줄 다음에 삽입
            new_line->prev = current_line;
            new_line->next = current_line->next;
            if (current_line->next) {
                current_line->next->prev = new_line;
            }
            current_line->next = new_line;

            // 커서를 새 줄로 이동
            current_line = new_line;
            *cursor_x = 0;  // 새 줄의 시작 위치
            *cursor_y += 1; // 한 줄 아래로 이동
            unsaved_changes = 1; // 변경 사항 기록
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
            unsaved_changes = 1; // 변경 사항 기록
            break;
        }

        clear();
        display_text(line_list);
        update_status_bar(filename, line_list, *cursor_x, *cursor_y); // 상태바 갱신
        refresh();
    }
}
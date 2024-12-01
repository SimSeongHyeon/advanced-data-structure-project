#include "search.h"
#include "line_node.h" // LineList를 정의한 헤더 파일
#include "cursor_text.h"
#include "display.h"
#include "os_terminal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Helper function to convert Deque to string
char* dequeToString(Deque* deque) {
    int length = deque->size;
    char* result = (char*)malloc((length + 1) * sizeof(char));
    if (!result) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    Node* current = deque->head;
    int index = 0;
    while (current) {
        result[index++] = current->data;
        current = current->next;
    }
    result[index] = '\0'; // Null-terminate the string
    return result;
}

// Helper function: Find last occurrence of substring
char* strrstr(const char* haystack, const char* needle) {
    if (!haystack || !needle) return NULL;

    char* last_found = NULL;
    char* current_found = strstr(haystack, needle);

    while (current_found) {
        last_found = current_found;
        current_found = strstr(current_found + 1, needle);
    }
    return last_found;
}

// 텍스트 검색 함수
int searchInDeque(LineList* line_list, const char* keyword) {
    LineNode* current_line = line_list->head;
    int row = 0;

    while (current_line != NULL) {
        char* line_text = dequeToString(current_line->left_deque); // Deque를 문자열로 변환
        if (!line_text) {
            printf("Error converting deque to string.\n");
            return -1;
        }

        if (strstr(line_text, keyword) != NULL) {
            free(line_text); // 동적 할당된 문자열 해제
            return row; // 첫 번째로 발견된 줄의 인덱스 반환
        }

        free(line_text); // 동적 할당된 문자열 해제
        current_line = current_line->next;
        row++;
    }
    return -1; // 발견되지 않음
}

int find_next(LineList* line_list, Cursor* cursor, const char* search_term) {
    LineNode* current = line_list->head;
    int row = 0;

    // 커서의 현재 줄로 이동
    for (int i = 0; i < cursor->y; i++) {
        if (current->next) {
            current = current->next;
        }
        else {
            return 0; // 더 이상 줄이 없으면 검색 실패
        }
    }

    // 현재 줄과 이후 줄 검색
    while (current) {
        char* line_text = dequeToString(current->left_deque); // Deque를 문자열로 변환
        if (!line_text) return 0; // 메모리 할당 실패

        const char* search_start = (row == cursor->y) ? line_text + cursor->x : line_text;
        char* found = strstr(search_start, search_term);

        if (found) {
            cursor->y = row;
            cursor->x = found - line_text; // 검색된 위치로 커서 이동
            free(line_text);
            return 1; // 검색 성공
        }

        free(line_text);
        current = current->next;
        row++;
        cursor->x = 0; // 이후 줄은 처음부터 검색
    }

    return 0; // 검색 실패
}

int find_previous(LineList* line_list, Cursor* cursor, const char* search_term) {
    LineNode* current = line_list->head;
    LineNode* prev_line = NULL;
    int row = 0;

    // 커서의 현재 줄로 이동
    for (int i = 0; i < cursor->y; i++) {
        if (current->next) {
            prev_line = current;
            current = current->next;
        }
        else {
            return 0; // 더 이상 이전 줄이 없으면 검색 실패
        }
    }

    // 현재 줄 역순 검색
    while (current) {
        char* line_text = dequeToString(current->left_deque); // Deque를 문자열로 변환
        if (!line_text) return 0;

        char* found = strrstr(line_text, search_term); // 마지막으로 검색어가 나타난 위치
        if (found && (row != cursor->y || found < line_text + cursor->x)) {
            cursor->y = row;
            cursor->x = found - line_text; // 검색된 위치로 커서 이동
            free(line_text);
            return 1; // 검색 성공
        }

        free(line_text);
        current = prev_line; // 이전 줄로 이동
        row--;
        if (current) prev_line = current->prev;
        cursor->x = INT_MAX; // 이전 줄은 끝에서부터 검색
    }

    return 0; // 검색 실패
}


int move_to_next_result(LineList* line_list, Cursor* cursor, const char* search_term) {
    if (!find_next(line_list, cursor, search_term)) {
        update_message_bar("No more matches found.");
        return 0; // 실패
    }
    else {
        clear();
        display_text(line_list, cursor);
        update_status_bar(NULL, line_list, cursor->x, cursor->y);
        refresh();
        return 1; // 성공
    }
}

int move_to_previous_result(LineList* line_list, Cursor* cursor, const char* search_term) {
    if (!find_previous(line_list, cursor, search_term)) {
        update_message_bar("No previous matches found.");
        return 0; // 실패
    }
    else {
        clear();
        display_text(line_list, cursor);
        update_status_bar(NULL, line_list, cursor->x, cursor->y);
        refresh();
        return 1; // 성공
    }
}

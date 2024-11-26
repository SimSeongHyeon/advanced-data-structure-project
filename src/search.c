#include "search.h"
#include "line_node.h" // LineList를 정의한 헤더 파일
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
            printf("Keyword found in row %d: %s\n", row, line_text);
            free(line_text); // 동적 할당된 문자열 해제
            return row; // 첫 번째로 발견된 줄의 인덱스 반환
        }

        free(line_text); // 동적 할당된 문자열 해제
        current_line = current_line->next;
        row++;
    }

    printf("Keyword not found.\n");
    return -1; // 발견되지 않음
}

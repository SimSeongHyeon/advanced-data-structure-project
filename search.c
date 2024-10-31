#include "search.h"
#include <stdio.h>
#include <string.h>

// 텍스트 검색 함수
int searchInDeque(Deque* deque, const char* keyword) {
    Node* current = deque->head;
    int row = 0;
    while (current != NULL) {
        if (strstr(current->line, keyword) != NULL) {
            printf("Keyword found in row %d: %s\n", row, current->line);
            return row;  // 첫 번째로 발견된 줄의 인덱스 반환
        }
        current = current->next;
        row++;
    }
    printf("Keyword not found.\n");
    return -1;  // 발견되지 않음
}

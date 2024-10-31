#include "display.h"
#include <stdio.h>

// 텍스트 화면에 출력 함수
void displayText(Deque* deque) {
    Node* current = deque->head;
    int lineNumber = 1;

    while (current != NULL) {
        printf("%d: %s\n", lineNumber, current->line);  // 각 줄의 번호와 내용을 출력
        current = current->next;
        lineNumber++;
    }
    printf("\n");
}

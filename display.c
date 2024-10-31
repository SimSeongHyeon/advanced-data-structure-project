#include "display.h"
#include <stdio.h>

// �ؽ�Ʈ ȭ�鿡 ��� �Լ�
void displayText(Deque* deque) {
    Node* current = deque->head;
    int lineNumber = 1;

    while (current != NULL) {
        printf("%d: %s\n", lineNumber, current->line);  // �� ���� ��ȣ�� ������ ���
        current = current->next;
        lineNumber++;
    }
    printf("\n");
}

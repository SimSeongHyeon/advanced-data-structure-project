#include "search.h"
#include <stdio.h>
#include <string.h>

// �ؽ�Ʈ �˻� �Լ�
int searchInDeque(Deque* deque, const char* keyword) {
    Node* current = deque->head;
    int row = 0;
    while (current != NULL) {
        if (strstr(current->line, keyword) != NULL) {
            printf("Keyword found in row %d: %s\n", row, current->line);
            return row;  // ù ��°�� �߰ߵ� ���� �ε��� ��ȯ
        }
        current = current->next;
        row++;
    }
    printf("Keyword not found.\n");
    return -1;  // �߰ߵ��� ����
}

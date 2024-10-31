#include "cursor_text.h"
#include <stdlib.h>
#include <string.h>

Cursor cursor = { 0, 0 }; // �ʱ� Ŀ�� ��ġ (0,0)

// �ؽ�Ʈ ���� �Լ�
void insertText(Deque* deque, const char* text, int row, int col) {
    Node* current = deque->head;
    for (int i = 0; i < row && current != NULL; i++) {
        current = current->next;
    }
    if (current == NULL) return;

    size_t newSize = strlen(current->line) + strlen(text) + 1;
    char* newLine = (char*)malloc(newSize);

    if (newLine != NULL) {  // �޸� �Ҵ� ���� ���� Ȯ��
        strncpy_s(newLine, newSize, current->line, col);  // ù �κ� ����
        strcat_s(newLine, newSize, text);                 // ������ �ؽ�Ʈ �߰�
        strcat_s(newLine, newSize, current->line + col);  // ������ �ؽ�Ʈ �߰�

        free(current->line);
        current->line = newLine;
    }
}

// Ŀ�� �̵� �Լ�
void moveCursor(int row, int col) {
    cursor.row = row;
    cursor.col = col;
}

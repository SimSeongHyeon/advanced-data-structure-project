#include "search.h"
#include "line_node.h" // LineList�� ������ ��� ����
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

// �ؽ�Ʈ �˻� �Լ�
int searchInDeque(LineList* line_list, const char* keyword) {
    LineNode* current_line = line_list->head;
    int row = 0;

    while (current_line != NULL) {
        char* line_text = dequeToString(current_line->left_deque); // Deque�� ���ڿ��� ��ȯ
        if (!line_text) {
            printf("Error converting deque to string.\n");
            return -1;
        }

        if (strstr(line_text, keyword) != NULL) {
            printf("Keyword found in row %d: %s\n", row, line_text);
            free(line_text); // ���� �Ҵ�� ���ڿ� ����
            return row; // ù ��°�� �߰ߵ� ���� �ε��� ��ȯ
        }

        free(line_text); // ���� �Ҵ�� ���ڿ� ����
        current_line = current_line->next;
        row++;
    }

    printf("Keyword not found.\n");
    return -1; // �߰ߵ��� ����
}

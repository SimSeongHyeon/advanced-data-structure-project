#include "file_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ���� ���� �Լ�
int openFile(Deque* deque, const char* filename) {
    FILE* file;

    // "r+" ���� ���� ���� (������ ���� ��� ���� ���� ���� ����)
    if (fopen_s(&file, filename, "r+") != 0) {
        if (fopen_s(&file, filename, "w+") != 0) {
            perror("Could not open file");
            return -1;
        }
    }

    // ���� ������ ���� �о����
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';  // �ٹٲ� ����
        appendNode(deque, buffer);             // ���� ���� �߰�
    }

    fclose(file);
    return 0;
}

// ���� ���� �Լ�
int saveFile(Deque* deque, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Could not open file for writing");
        return -1;
    }

    Node* current = deque->head;
    while (current != NULL) {
        fprintf(file, "%s\n", current->line);  // ���� �� ���� ���Ͽ� ���
        current = current->next;
    }

    fclose(file);
    return 0;
}

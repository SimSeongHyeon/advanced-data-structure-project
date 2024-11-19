#include "file_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ������ �����ϴ��� Ȯ���ϴ� �Լ�
int file_exists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1; // ������ ����
    }
    return 0; // ������ �������� ����
}

// �ߺ����� �ʴ� ���� �̸��� �����ϴ� �Լ�
void generate_unique_filename(char* filename) {
    int count = 1;
    while (file_exists(filename)) {
        sprintf(filename, "untitled(%d).txt", count++);
    }
}

Deque* load_file(const char* filename) {
    Deque* deque = init_deque();
    char line[256];

    FILE* file = fopen(filename, "r");
    if (file) {
        // ������ �����ϴ� ���, ������ �ҷ���
        while (fgets(line, sizeof(line), file)) {
            append_node(deque, line);
        }
        fclose(file);
    }
    else {
        // ������ �������� ������ ���� ����
        file = fopen(filename, "w");
        if (file) {
            fclose(file);
        }
        else {
            printf("Error creating file: %s\n", filename);
            free_deque(deque);
            return NULL;
        }
    }

    return deque;
}

void save_file(const char* filename, Deque* deque) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error saving file: %s\n", filename);
        return;
    }

    Node* current = deque->head;
    while (current) {
        fputs(current->line, file);
        current = current->next;
    }

    fclose(file);
}

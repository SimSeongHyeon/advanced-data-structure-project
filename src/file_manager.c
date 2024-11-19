#include "file_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 파일이 존재하는지 확인하는 함수
int file_exists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1; // 파일이 존재
    }
    return 0; // 파일이 존재하지 않음
}

// 중복되지 않는 파일 이름을 생성하는 함수
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
        // 파일이 존재하는 경우, 내용을 불러옴
        while (fgets(line, sizeof(line), file)) {
            append_node(deque, line);
        }
        fclose(file);
    }
    else {
        // 파일이 존재하지 않으면 새로 생성
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

#include "file_manager.h"
#include "line_node.h"
#include "deque.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LineList* load_file(const char* filename) {

    fprintf(stderr, "Loading file: %s\n", filename); // 파일 로드 시작 로그

    FILE* file = fopen(filename, "r");
    if (!file) {
        // 파일이 없으면 NULL 반환
        return NULL;
    }

    LineList* line_list = init_line_list();
    if (!line_list) {
        fclose(file);
        fprintf(stderr, "Failed to initialize LineList.\n");
        return NULL;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        LineNode* new_line = create_line_node();
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '\n') continue; // \n 제외
            push_back(new_line->left_deque, line[i]);
        }
        append_line(line_list, new_line);
    }
    fclose(file);

    fprintf(stderr, "File '%s' loaded successfully. Total lines: %d\n", filename, line_list->total_lines); // 파일 로드 완료 로그

    return line_list;
}

void save_file(const char* filename, LineList* line_list) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error saving file: %s\n", filename);
        return;
    }

    LineNode* current = line_list->head;
    while (current) {
        Node* left_node = current->left_deque->head;
        while (left_node) {
            fputc(left_node->data, file);
            left_node = left_node->next;
        }
        fputc('\n', file);  // 각 라인의 끝에 \n 추가
        current = current->next;
    }
    fclose(file);
}
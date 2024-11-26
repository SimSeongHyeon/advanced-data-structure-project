#include "file_manager.h"
#include "line_node.h"
#include "deque.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LineList* load_file(const char* filename) {
    fprintf(stderr, "Attempting to open file: %s\n", filename);

    FILE* file = fopen(filename, "r");
    LineList* line_list = init_line_list();

    if (!line_list) {
        fprintf(stderr, "Failed to initialize line_list\n");
        return NULL;
    }

    if (!file) {
        fprintf(stderr, "File not found. Creating new file: %s\n", filename);
        file = fopen(filename, "w");
        if (!file) {
            fprintf(stderr, "Error creating file: %s\n", filename);
            free(line_list);
            return NULL;
        }
        fclose(file);

        // 새 파일 생성 시 기본 라인 추가
        LineNode* new_line = create_line_node();
        if (!new_line) {
            fprintf(stderr, "Failed to create initial line node\n");
            free(line_list);
            return NULL;
        }
        append_line(line_list, new_line);
        fprintf(stderr, "Initial line added to new file\n");

        return line_list;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        LineNode* new_line = create_line_node();
        if (!new_line) {
            fprintf(stderr, "Failed to create new line node\n");
            fclose(file);
            free_line_list(line_list);
            return NULL;
        }

        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '\n') continue;  // 줄 바꿈 문자 제외
            push_back(new_line->left_deque, line[i]);
        }
        append_line(line_list, new_line);
    }
    fclose(file);

    if (!line_list->head) {
        // 기존 파일이 비어있는 경우 기본 라인 추가
        fprintf(stderr, "File is empty, adding an initial line\n");
        LineNode* new_line = create_line_node();
        if (!new_line) {
            fprintf(stderr, "Failed to create initial line node for empty file\n");
            free_line_list(line_list);
            return NULL;
        }
        append_line(line_list, new_line);
    }

    fprintf(stderr, "File loaded successfully\n");
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
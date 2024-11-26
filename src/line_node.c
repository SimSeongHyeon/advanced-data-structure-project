#include "line_node.h"
#include <stdlib.h>
#include <stdio.h>

LineList* init_line_list() {
    LineList* list = (LineList*)malloc(sizeof(LineList));
    if (!list) {
        // 메모리 할당 실패 시 처리
        printf("Error: Memory allocation for LineList failed.\n");
        return NULL;
    }

    list->head = list->tail = NULL;
    list->total_lines = 0;
    return list;
}

LineNode* create_line_node() {
    LineNode* node = (LineNode*)malloc(sizeof(LineNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed for LineNode\n");
        return NULL;
    }

    node->left_deque = init_deque();
    node->right_deque = init_deque();

    if (!node->left_deque || !node->right_deque) {
        fprintf(stderr, "Deque initialization failed\n");
        if (node->left_deque) free_deque(node->left_deque);
        if (node->right_deque) free_deque(node->right_deque);
        free(node);
        return NULL;
    }

    node->prev = node->next = NULL;
    return node;
}


void append_line(LineList* list, LineNode* line_node) {
    if (!list || !line_node) {
        fprintf(stderr, "append_line: list or line_node is NULL\n");
        return;
    }

    if (!list->head) {
        list->head = list->tail = line_node;
    }
    else {
        list->tail->next = line_node;
        line_node->prev = list->tail;
        list->tail = line_node;
    }

    list->total_lines++;
    fprintf(stderr, "Line appended: total_lines = %d\n", list->total_lines);
}


void free_line_list(LineList* list) {
    if (!list) return;

    LineNode* current = list->head;
    while (current) {
        LineNode* next = current->next;
        free_deque(current->left_deque);
        free_deque(current->right_deque);
        free(current);
        current = next;
    }
    free(list);
}


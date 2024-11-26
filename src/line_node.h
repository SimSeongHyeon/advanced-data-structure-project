#ifndef LINE_NODE_H
#define LINE_NODE_H

#include "deque.h"

typedef struct LineNode {
    Deque* left_deque;
    Deque* right_deque;
    struct LineNode* prev;
    struct LineNode* next;
} LineNode;

typedef struct {
    LineNode* head;
    LineNode* tail;
    int total_lines;
} LineList;

LineList* init_line_list();
LineNode* create_line_node();
void append_line(LineList* list, LineNode* line_node);
void free_line_list(LineList* list);

#endif // LINE_NODE_H

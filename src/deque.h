#ifndef DEQUE_H
#define DEQUE_H

typedef struct Node {
    char* line;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int size;
} Deque;

Deque* init_deque();
void append_node(Deque* deque, const char* line);
void free_deque(Deque* deque);

#endif // DEQUE_H

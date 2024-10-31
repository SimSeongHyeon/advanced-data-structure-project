#ifndef DEQUE_H
#define DEQUE_H

typedef struct Node {
    char* line;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct Deque {
    Node* head;
    Node* tail;
    int size;
} Deque;

// 덱 조작 함수들
Deque* initDeque();
void appendNode(Deque* deque, const char* line);
void freeDeque(Deque* deque);

#endif // DEQUE_H

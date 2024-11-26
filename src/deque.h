#ifndef DEQUE_H
#define DEQUE_H

typedef struct Node {
    char data;                  // 각 노드에 저장될 문자 데이터
    struct Node* prev;          // 이전 노드 포인터
    struct Node* next;          // 다음 노드 포인터
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int size;
} Deque;

Deque* init_deque();
void append_node(Deque* deque, const char* line);
void free_deque(Deque* deque);

// 추가된 함수 선언
void push_back(Deque* deque, char data);
char pop_back(Deque* deque);
void push_front(Deque* deque, char data);
char pop_front(Deque* deque);

#endif // DEQUE_H

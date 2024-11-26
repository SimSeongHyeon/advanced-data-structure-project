#ifndef DEQUE_H
#define DEQUE_H

typedef struct Node {
    char data;                  // �� ��忡 ����� ���� ������
    struct Node* prev;          // ���� ��� ������
    struct Node* next;          // ���� ��� ������
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int size;
} Deque;

Deque* init_deque();
void append_node(Deque* deque, const char* line);
void free_deque(Deque* deque);

// �߰��� �Լ� ����
void push_back(Deque* deque, char data);
char pop_back(Deque* deque);
void push_front(Deque* deque, char data);
char pop_front(Deque* deque);

#endif // DEQUE_H

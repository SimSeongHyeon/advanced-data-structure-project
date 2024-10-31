#include "deque.h"
#include <stdlib.h>
#include <string.h>

// �� �ʱ�ȭ �Լ�
Deque* initDeque() {
    Deque* deque = (Deque*)malloc(sizeof(Deque));
    if (deque != NULL) {
        deque->head = NULL;
        deque->tail = NULL;
        deque->size = 0;
    }
    return deque;
}

// ��� ���� �Լ�
Node* createNode(const char* line) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->line = _strdup(line);  // Visual Studio���� _strdup ���
        newNode->prev = NULL;
        newNode->next = NULL;
    }
    return newNode;
}

// ���� ���� ��� �߰�
void appendNode(Deque* deque, const char* line) {
    if (deque == NULL) return;

    Node* newNode = createNode(line);
    if (newNode == NULL) return;

    if (deque->tail == NULL) {
        deque->head = deque->tail = newNode;
    }
    else {
        deque->tail->next = newNode;
        newNode->prev = deque->tail;
        deque->tail = newNode;
    }
    deque->size++;
}

// �� �޸� ����
void freeDeque(Deque* deque) {
    if (deque == NULL) return;

    Node* current = deque->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp->line);
        free(temp);
    }
    free(deque);
}

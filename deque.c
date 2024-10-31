#include "deque.h"
#include <stdlib.h>
#include <string.h>

// 덱 초기화 함수
Deque* initDeque() {
    Deque* deque = (Deque*)malloc(sizeof(Deque));
    if (deque != NULL) {
        deque->head = NULL;
        deque->tail = NULL;
        deque->size = 0;
    }
    return deque;
}

// 노드 생성 함수
Node* createNode(const char* line) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->line = _strdup(line);  // Visual Studio에서 _strdup 사용
        newNode->prev = NULL;
        newNode->next = NULL;
    }
    return newNode;
}

// 덱의 끝에 노드 추가
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

// 덱 메모리 해제
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

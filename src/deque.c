#include "deque.h"
#include <stdlib.h>

Deque* init_deque() {
    Deque* deque = (Deque*)malloc(sizeof(Deque));
    deque->head = deque->tail = NULL;
    deque->size = 0;
    return deque;
}

void push_back(Deque* deque, char data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->prev = deque->tail;
    new_node->next = NULL;

    if (deque->tail) deque->tail->next = new_node;
    deque->tail = new_node;
    if (!deque->head) deque->head = new_node;

    deque->size++;
}

char pop_back(Deque* deque) {
    if (!deque || !deque->tail) return '\0'; // 后 郸 贸府

    Node* tail = deque->tail;
    char data = tail->data;

    deque->tail = tail->prev;
    if (deque->tail) {
        deque->tail->next = NULL;
    }
    else {
        deque->head = NULL;
    }

    free(tail);
    deque->size--;
    return data;
}

void push_front(Deque* deque, char data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = deque->head;
    new_node->prev = NULL;

    if (deque->head) deque->head->prev = new_node;
    deque->head = new_node;
    if (!deque->tail) deque->tail = new_node;

    deque->size++;
}

char pop_front(Deque* deque) {
    if (!deque->head) return '\0'; // 后 郸 贸府

    Node* head = deque->head;
    char data = head->data;

    deque->head = head->next;
    if (deque->head) {
        deque->head->prev = NULL;
    }
    else {
        deque->tail = NULL;
    }

    free(head);
    deque->size--;
    return data;
}

void free_deque(Deque* deque) {
    Node* current = deque->head;
    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(deque);
}

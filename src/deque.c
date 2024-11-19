#include "deque.h"
#include <stdlib.h>
#include <string.h>

Deque* init_deque() {
    Deque* deque = (Deque*)malloc(sizeof(Deque));
    deque->head = deque->tail = NULL;
    deque->size = 0;
    return deque;
}

void append_node(Deque* deque, const char* line) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->line = strdup(line);
    new_node->prev = deque->tail;
    new_node->next = NULL;

    if (deque->tail) deque->tail->next = new_node;
    deque->tail = new_node;
    if (!deque->head) deque->head = new_node;

    deque->size++;
}

void free_deque(Deque* deque) {
    Node* current = deque->head;
    while (current) {
        Node* next = current->next;
        free(current->line);
        free(current);
        current = next;
    }
    free(deque);
}

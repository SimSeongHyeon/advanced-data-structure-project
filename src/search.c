#include "search.h"
#include "line_node.h" // LineList�� ������ ��� ����
#include "cursor_text.h"
#include "display.h"
#include "os_terminal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Helper function to convert Deque to string
char* dequeToString(Deque* deque) {
    int length = deque->size;
    char* result = (char*)malloc((length + 1) * sizeof(char));
    if (!result) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    Node* current = deque->head;
    int index = 0;
    while (current) {
        result[index++] = current->data;
        current = current->next;
    }
    result[index] = '\0'; // Null-terminate the string
    return result;
}

// Helper function: Find last occurrence of substring
char* strrstr(const char* haystack, const char* needle) {
    if (!haystack || !needle) return NULL;

    char* last_found = NULL;
    char* current_found = strstr(haystack, needle);

    while (current_found) {
        last_found = current_found;
        current_found = strstr(current_found + 1, needle);
    }
    return last_found;
}

// �ؽ�Ʈ �˻� �Լ�
int searchInDeque(LineList* line_list, const char* keyword) {
    LineNode* current_line = line_list->head;
    int row = 0;

    while (current_line != NULL) {
        char* line_text = dequeToString(current_line->left_deque); // Deque�� ���ڿ��� ��ȯ
        if (!line_text) {
            printf("Error converting deque to string.\n");
            return -1;
        }

        if (strstr(line_text, keyword) != NULL) {
            free(line_text); // ���� �Ҵ�� ���ڿ� ����
            return row; // ù ��°�� �߰ߵ� ���� �ε��� ��ȯ
        }

        free(line_text); // ���� �Ҵ�� ���ڿ� ����
        current_line = current_line->next;
        row++;
    }
    return -1; // �߰ߵ��� ����
}

int find_next(LineList* line_list, Cursor* cursor, const char* search_term) {
    LineNode* current = line_list->head;
    int row = 0;

    // Ŀ���� ���� �ٷ� �̵�
    for (int i = 0; i < cursor->y; i++) {
        if (current->next) {
            current = current->next;
        }
        else {
            return 0; // �� �̻� ���� ������ �˻� ����
        }
    }

    // ���� �ٰ� ���� �� �˻�
    while (current) {
        char* line_text = dequeToString(current->left_deque); // Deque�� ���ڿ��� ��ȯ
        if (!line_text) return 0; // �޸� �Ҵ� ����

        const char* search_start = (row == cursor->y) ? line_text + cursor->x : line_text;
        char* found = strstr(search_start, search_term);

        if (found) {
            cursor->y = row;
            cursor->x = found - line_text; // �˻��� ��ġ�� Ŀ�� �̵�
            free(line_text);
            return 1; // �˻� ����
        }

        free(line_text);
        current = current->next;
        row++;
        cursor->x = 0; // ���� ���� ó������ �˻�
    }

    return 0; // �˻� ����
}

int find_previous(LineList* line_list, Cursor* cursor, const char* search_term) {
    LineNode* current = line_list->head;
    LineNode* prev_line = NULL;
    int row = 0;

    // Ŀ���� ���� �ٷ� �̵�
    for (int i = 0; i < cursor->y; i++) {
        if (current->next) {
            prev_line = current;
            current = current->next;
        }
        else {
            return 0; // �� �̻� ���� ���� ������ �˻� ����
        }
    }

    // ���� �� ���� �˻�
    while (current) {
        char* line_text = dequeToString(current->left_deque); // Deque�� ���ڿ��� ��ȯ
        if (!line_text) return 0;

        char* found = strrstr(line_text, search_term); // ���������� �˻�� ��Ÿ�� ��ġ
        if (found && (row != cursor->y || found < line_text + cursor->x)) {
            cursor->y = row;
            cursor->x = found - line_text; // �˻��� ��ġ�� Ŀ�� �̵�
            free(line_text);
            return 1; // �˻� ����
        }

        free(line_text);
        current = prev_line; // ���� �ٷ� �̵�
        row--;
        if (current) prev_line = current->prev;
        cursor->x = INT_MAX; // ���� ���� ���������� �˻�
    }

    return 0; // �˻� ����
}


int move_to_next_result(LineList* line_list, Cursor* cursor, const char* search_term) {
    if (!find_next(line_list, cursor, search_term)) {
        update_message_bar("No more matches found.");
        return 0; // ����
    }
    else {
        clear();
        display_text(line_list, cursor);
        update_status_bar(NULL, line_list, cursor->x, cursor->y);
        refresh();
        return 1; // ����
    }
}

int move_to_previous_result(LineList* line_list, Cursor* cursor, const char* search_term) {
    if (!find_previous(line_list, cursor, search_term)) {
        update_message_bar("No previous matches found.");
        return 0; // ����
    }
    else {
        clear();
        display_text(line_list, cursor);
        update_status_bar(NULL, line_list, cursor->x, cursor->y);
        refresh();
        return 1; // ����
    }
}

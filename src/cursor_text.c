#include "cursor_text.h"
#include "line_node.h"
#include <stdio.h>

static Cursor cursor = { 0, 0 };

void move_cursor(int dx, int dy, LineNode** current_line, Cursor* cursor, LineList* line_list) {
    // ���� ������ �ؽ�Ʈ ���� + �� ���� ����
    int line_length_with_space = (*current_line)->left_deque->size + (*current_line)->right_deque->size + 1;

    // �¿� �̵� ó��
    cursor->x += dx;

    // ���� ���� ����� ��
    if (cursor->x < 0) {
        if ((*current_line)->prev) { // ���� ������ �ִ� ���
            *current_line = (*current_line)->prev;
            cursor->x = (*current_line)->left_deque->size + (*current_line)->right_deque->size; // ���� ������ ������ �̵�
            cursor->y--;
        }
        else {
            cursor->x = 0; // ù ��° ������ ���ۿ��� ����
        }
    }
    // ������ ���� ����� ��
    else if (cursor->x >= line_length_with_space) {
        if ((*current_line)->next) { // ���� ������ �ִ� ���
            *current_line = (*current_line)->next;
            cursor->x = 0; // ���� ������ �������� �̵�
            cursor->y++;
        }
        else {
            cursor->x = line_length_with_space - 1; // ���� ������ ������ ����
        }
    }

    // ���� �̵� ó��
    if (dy < 0 && (*current_line)->prev) { // ���� �̵�
        *current_line = (*current_line)->prev;
        cursor->y--;
        int new_line_length = (*current_line)->left_deque->size + (*current_line)->right_deque->size + 1;
        cursor->x = (cursor->x > new_line_length - 1) ? new_line_length - 1 : cursor->x;
    }
    else if (dy > 0 && (*current_line)->next) { // �Ʒ��� �̵�
        *current_line = (*current_line)->next;
        cursor->y++;
        int new_line_length = (*current_line)->left_deque->size + (*current_line)->right_deque->size + 1;
        cursor->x = (cursor->x > new_line_length - 1) ? new_line_length - 1 : cursor->x;
    }

    // Ŀ���� ������ ���� ������ �ִ� ���� + �� �������� ����
    line_length_with_space = (*current_line)->left_deque->size + (*current_line)->right_deque->size + 1;
    if (cursor->x < 0) cursor->x = 0;
    if (cursor->x >= line_length_with_space) cursor->x = line_length_with_space - 1;
}


void set_cursor_position(int x, int y, int max_x, int max_y) {
    cursor.x = x;
    cursor.y = y;

    // Ŀ�� ��ġ�� ȭ���� ����� �ʵ��� ����
    if (cursor.x < 0) cursor.x = 0;
    if (cursor.y < 0) cursor.y = 0;
    if (cursor.x >= max_x) cursor.x = max_x - 1;
    if (cursor.y >= max_y) cursor.y = max_y - 1;
}

Cursor get_cursor_position(void) {
    return cursor;
}

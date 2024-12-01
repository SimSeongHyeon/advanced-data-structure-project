#include "cursor_text.h"
#include "line_node.h"
#include <stdio.h>

static Cursor cursor = { 0, 0 };

void move_cursor(int dx, int dy, LineNode** current_line, Cursor* cursor, LineList* line_list) {
    // 현재 라인의 텍스트 길이 + 빈 공간 포함
    int line_length_with_space = (*current_line)->left_deque->size + (*current_line)->right_deque->size + 1;

    // 좌우 이동 처리
    cursor->x += dx;

    // 왼쪽 끝을 벗어났을 때
    if (cursor->x < 0) {
        if ((*current_line)->prev) { // 이전 라인이 있는 경우
            *current_line = (*current_line)->prev;
            cursor->x = (*current_line)->left_deque->size + (*current_line)->right_deque->size; // 이전 라인의 끝으로 이동
            cursor->y--;
        }
        else {
            cursor->x = 0; // 첫 번째 라인의 시작에서 멈춤
        }
    }
    // 오른쪽 끝을 벗어났을 때
    else if (cursor->x >= line_length_with_space) {
        if ((*current_line)->next) { // 다음 라인이 있는 경우
            *current_line = (*current_line)->next;
            cursor->x = 0; // 다음 라인의 시작으로 이동
            cursor->y++;
        }
        else {
            cursor->x = line_length_with_space - 1; // 현재 라인의 끝에서 멈춤
        }
    }

    // 상하 이동 처리
    if (dy < 0 && (*current_line)->prev) { // 위로 이동
        *current_line = (*current_line)->prev;
        cursor->y--;
        int new_line_length = (*current_line)->left_deque->size + (*current_line)->right_deque->size + 1;
        cursor->x = (cursor->x > new_line_length - 1) ? new_line_length - 1 : cursor->x;
    }
    else if (dy > 0 && (*current_line)->next) { // 아래로 이동
        *current_line = (*current_line)->next;
        cursor->y++;
        int new_line_length = (*current_line)->left_deque->size + (*current_line)->right_deque->size + 1;
        cursor->x = (cursor->x > new_line_length - 1) ? new_line_length - 1 : cursor->x;
    }

    // 커서의 범위를 현재 라인의 최대 길이 + 빈 공간으로 제한
    line_length_with_space = (*current_line)->left_deque->size + (*current_line)->right_deque->size + 1;
    if (cursor->x < 0) cursor->x = 0;
    if (cursor->x >= line_length_with_space) cursor->x = line_length_with_space - 1;
}


void set_cursor_position(int x, int y, int max_x, int max_y) {
    cursor.x = x;
    cursor.y = y;

    // 커서 위치가 화면을 벗어나지 않도록 제한
    if (cursor.x < 0) cursor.x = 0;
    if (cursor.y < 0) cursor.y = 0;
    if (cursor.x >= max_x) cursor.x = max_x - 1;
    if (cursor.y >= max_y) cursor.y = max_y - 1;
}

Cursor get_cursor_position(void) {
    return cursor;
}

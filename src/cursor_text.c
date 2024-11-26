#include "cursor_text.h"

static Cursor cursor = { 0, 0 };

void move_cursor(int dx, int dy, int max_x, int max_y) {
    cursor.x += dx;
    cursor.y += dy;

    // Ŀ�� ��ġ�� ȭ���� ����� �ʵ��� ����
    if (cursor.x < 0) cursor.x = 0;
    if (cursor.y < 0) cursor.y = 0;
    if (cursor.x >= max_x) cursor.x = max_x - 1;
    if (cursor.y >= max_y) cursor.y = max_y - 1;
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

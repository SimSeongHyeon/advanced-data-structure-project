#include "cursor_text.h"

static Cursor cursor = { 0, 0 };

void move_cursor(int dx, int dy) {
    cursor.x += dx;
    cursor.y += dy;
}

void set_cursor_position(int x, int y) {
    cursor.x = x;
    cursor.y = y;
}

Cursor get_cursor_position(void) {
    return cursor;
}

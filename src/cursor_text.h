#ifndef CURSOR_TEXT_H
#define CURSOR_TEXT_H

typedef struct {
    int x;
    int y;
} Cursor;

void move_cursor(int dx, int dy, int max_x, int max_y);
void set_cursor_position(int x, int y, int max_x, int max_y);
Cursor get_cursor_position(void);

#endif // CURSOR_TEXT_Hd

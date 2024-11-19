#ifndef DISPLAY_H
#define DISPLAY_H

#include "deque.h"
#include "cursor_text.h"

void display_text(Deque* deque);
void update_status_bar(const char* filename, Deque* deque);

#endif // DISPLAY_H

#ifndef DISPLAY_H
#define DISPLAY_H

#include "line_node.h"
#include "cursor_text.h"

void display_text(LineList* line_list);
void update_status_bar(const char* filename, LineList* line_list, int cursor_x, int cursor_y);
void update_message_bar(const char* message);
void display_help_bar();

#endif // DISPLAY_H

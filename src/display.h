#ifndef DISPLAY_H
#define DISPLAY_H

#include "line_node.h"
#include "cursor_text.h"

void display_text(LineList* line_list, Cursor* cursor);
void update_status_bar(const char* filename, LineList* line_list, int cursor_x, int cursor_y);
void update_message_bar(const char* message);
void display_help_bar();
void highlight_search_result(LineList* line_list, Cursor* cursor, const char* search_term);


#endif // DISPLAY_H

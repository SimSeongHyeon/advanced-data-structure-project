#ifndef INPUT_H
#define INPUT_H

#include "line_node.h"

void handle_input(LineList* line_list, LineNode* current_line, int* cursor_x, int* cursor_y, char* filename);
int confirm_exit(int* unsaved_changes, LineList* line_list);
void get_search_term(char* search_term);

#endif // INPUT_H
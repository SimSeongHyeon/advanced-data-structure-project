#ifndef INPUT_H
#define INPUT_H

#include "line_node.h"

void handle_input(LineList* line_list, LineNode* current_line, int* cursor_x, int* cursor_y, char* filename);
int confirm_exit(int* unsaved_changes);

#endif // INPUT_H
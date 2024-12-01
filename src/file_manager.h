#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "line_node.h"

LineList* load_file(const char* filename);
void save_file(const char* filename, LineList* line_list);
void save_file_with_prompt(LineList* line_list);

#endif // FILE_MANAGER_H

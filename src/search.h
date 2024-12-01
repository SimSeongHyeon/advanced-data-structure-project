#ifndef SEARCH_H
#define SEARCH_H

#include "line_node.h" // LineList�� ������ ��� ����
#include "cursor_text.h"

// �˻� �Լ� ����
char* dequeToString(Deque* deque);
int searchInDeque(LineList* line_list, const char* keyword);
int find_next(LineList* line_list, Cursor* cursor, const char* search_term);
int move_to_next_result(LineList* line_list, Cursor* cursor, const char* search_term);
int move_to_previous_result(LineList* line_list, Cursor* cursor, const char* search_term);


#endif // SEARCH_H

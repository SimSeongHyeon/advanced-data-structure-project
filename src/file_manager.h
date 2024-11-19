#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "deque.h"

Deque* load_file(const char* filename);
void save_file(const char* filename, Deque* deque);
void generate_unique_filename(char* filename);  // 함수 선언 추가

#endif // FILE_MANAGER_H

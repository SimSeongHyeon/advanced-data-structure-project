#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "deque.h"

int openFile(Deque* deque, const char* filename);
int saveFile(Deque* deque, const char* filename);

#endif // FILE_MANAGER_H

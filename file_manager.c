#include "file_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 파일 열기 함수
int openFile(Deque* deque, const char* filename) {
    FILE* file;

    // "r+" 모드로 파일 열기 (파일이 없을 경우 쓰기 모드로 새로 생성)
    if (fopen_s(&file, filename, "r+") != 0) {
        if (fopen_s(&file, filename, "w+") != 0) {
            perror("Could not open file");
            return -1;
        }
    }

    // 파일 내용을 덱에 읽어오기
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';  // 줄바꿈 제거
        appendNode(deque, buffer);             // 줄을 덱에 추가
    }

    fclose(file);
    return 0;
}

// 파일 저장 함수
int saveFile(Deque* deque, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Could not open file for writing");
        return -1;
    }

    Node* current = deque->head;
    while (current != NULL) {
        fprintf(file, "%s\n", current->line);  // 덱의 각 줄을 파일에 기록
        current = current->next;
    }

    fclose(file);
    return 0;
}

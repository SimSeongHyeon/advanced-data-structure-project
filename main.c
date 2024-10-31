#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"
#include "file_manager.h"
#include "display.h"
#include "cursor_text.h"
#include "input.h"

void startEditor(const char* filename) {
    Deque* deque = initDeque();

    // 파일 열기
    if (openFile(deque, filename) == -1) {
        printf("Failed to open file: %s\n", filename);
        freeDeque(deque);
        return;
    }

    // 기본 화면 출력
    displayText(deque);

    // 기본 명령어 안내
    printf("Commands:\n");
    printf("  - 'q' to quit\n");
    printf("  - 's' to save\n");
    printf("  - 'w', 'a', 's', 'd' to move cursor\n");

    char command;
    while (1) {
        printf("Enter command: ");
        command = getchar();
        getchar(); // Enter key handling

        // 명령어 처리
        if (command == 'q') { // 종료
            printf("Exiting editor.\n");
            break;
        }
        else if (command == 's') { // 저장
            if (saveFile(deque, filename) == 0) {
                printf("File saved successfully.\n");
            }
            else {
                printf("Failed to save file.\n");
            }
        }
        else {
            handleInput(command, deque); // 커서 이동 및 기타 명령어 처리
            displayText(deque); // 변경된 내용을 화면에 다시 표시
        }
    }

    freeDeque(deque); // 종료 시 메모리 해제
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    startEditor(argv[1]);
    return 0;
}

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

    // ���� ����
    if (openFile(deque, filename) == -1) {
        printf("Failed to open file: %s\n", filename);
        freeDeque(deque);
        return;
    }

    // �⺻ ȭ�� ���
    displayText(deque);

    // �⺻ ��ɾ� �ȳ�
    printf("Commands:\n");
    printf("  - 'q' to quit\n");
    printf("  - 's' to save\n");
    printf("  - 'w', 'a', 's', 'd' to move cursor\n");

    char command;
    while (1) {
        printf("Enter command: ");
        command = getchar();
        getchar(); // Enter key handling

        // ��ɾ� ó��
        if (command == 'q') { // ����
            printf("Exiting editor.\n");
            break;
        }
        else if (command == 's') { // ����
            if (saveFile(deque, filename) == 0) {
                printf("File saved successfully.\n");
            }
            else {
                printf("Failed to save file.\n");
            }
        }
        else {
            handleInput(command, deque); // Ŀ�� �̵� �� ��Ÿ ��ɾ� ó��
            displayText(deque); // ����� ������ ȭ�鿡 �ٽ� ǥ��
        }
    }

    freeDeque(deque); // ���� �� �޸� ����
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    startEditor(argv[1]);
    return 0;
}

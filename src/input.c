#include "input.h"
#include <stdio.h>

void handle_input() {
    int ch;
    printf("Press 'q' to quit.\n");
    while ((ch = getchar()) != 'q') {
        printf("You pressed: %c\n", ch);
    }
}

//#include <curses.h>
//
//int main() {
//    // ncurses �ʱ�ȭ
//    initscr();            // curses ���� ����
//    cbreak();             // ���� ���� ��� Ű �Է��� ����
//    noecho();             // Ű �Է� �� ȭ�鿡 ������� ����
//    keypad(stdscr, TRUE); // ȭ��ǥ Ű ��� ����
//    curs_set(0);          // Ŀ���� ������ �ʰ� ����
//
//    // �׽�Ʈ �޽��� ���
//    mvprintw(5, 10, "PDCurses �׽�Ʈ�Դϴ�.");
//    mvprintw(7, 10, "�ƹ� Ű�� ����������...");
//    mvprintw(9, 10, "ESC Ű�� ������ ����˴ϴ�.");
//    refresh(); // ȭ�� ����
//
//    int ch;
//    while ((ch = getch()) != 27) { // ESC Ű�� �ԷµǸ� ����
//        // �Էµ� Ű�� ȭ�鿡 ���
//        mvprintw(11, 10, "�Էµ� Ű �ڵ�: %d   ", ch);
//        refresh();
//    }
//
//    // curses ��� ����
//    endwin();
//    return 0;
//}

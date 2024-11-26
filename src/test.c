//#include <curses.h>
//
//int main() {
//    // ncurses 초기화
//    initscr();            // curses 모드로 진입
//    cbreak();             // 버퍼 없이 즉시 키 입력을 전달
//    noecho();             // 키 입력 시 화면에 출력하지 않음
//    keypad(stdscr, TRUE); // 화살표 키 사용 가능
//    curs_set(0);          // 커서를 보이지 않게 설정
//
//    // 테스트 메시지 출력
//    mvprintw(5, 10, "PDCurses 테스트입니다.");
//    mvprintw(7, 10, "아무 키나 눌러보세요...");
//    mvprintw(9, 10, "ESC 키를 누르면 종료됩니다.");
//    refresh(); // 화면 갱신
//
//    int ch;
//    while ((ch = getch()) != 27) { // ESC 키가 입력되면 종료
//        // 입력된 키를 화면에 출력
//        mvprintw(11, 10, "입력된 키 코드: %d   ", ch);
//        refresh();
//    }
//
//    // curses 모드 종료
//    endwin();
//    return 0;
//}

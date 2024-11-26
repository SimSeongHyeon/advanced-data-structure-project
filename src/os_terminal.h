#ifndef OS_TERMINAL_H
#define OS_TERMINAL_H

#ifdef _WIN32
#include "../lib/wincon/curses.h" // Windows������ PDCurses
#else
#include <ncurses.h>             // Linux/macOS������ ncurses
#endif

#endif // OS_TERMINAL_H

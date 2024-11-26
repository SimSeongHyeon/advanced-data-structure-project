#ifndef OS_TERMINAL_H
#define OS_TERMINAL_H

#ifdef _WIN32
#include "../lib/wincon/curses.h" // Windows에서는 PDCurses
#else
#include <ncurses.h>             // Linux/macOS에서는 ncurses
#endif

#endif // OS_TERMINAL_H

#ifndef OS_TERMINAL_H
#define OS_TERMINAL_H

#ifdef _WIN32
#include "../lib/wincon/curses.h"
#else
#include <ncurses.h>
#endif

#endif // OS_TERMINAL_H

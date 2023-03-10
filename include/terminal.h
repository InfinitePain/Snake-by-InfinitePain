/*
 ============================================================================
 Name        : terminal.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <ncurses.h>

void init_screen();
WINDOW* create_win(WINDOW* window);
void destroy_screen();

#endif /*TERMINAL_H_*/
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

#define INPUT1_WIN 0
#define INPUT2_WIN 1
#define GAME_WIN 2
#define MENU_WIN 3

extern WINDOW* appWindows[4];

void wmanual_box(WINDOW* win, int x, int y, int width, int height);
WINDOW* create_win(int width, int height, int start_y, int start_x);
void init_screen();
void create_app_windows();
void destroy_screen();

#endif /*TERMINAL_H_*/
/*
 ============================================================================
 Name        : terminal.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include "terminal.h" 
#include <setjmp.h>
#include "appdata.h"
#include "error_message.h"

extern jmp_buf jmp_buffer10;

void init_screen() {
	sleep(1);
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	clear();
	start_color();

	init_pair(1, COLOR_BLACK, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_BLUE);
	init_pair(3, COLOR_BLACK, COLOR_GREEN);
	init_pair(4, COLOR_BLACK, COLOR_CYAN);
	init_pair(5, COLOR_BLACK, COLOR_RED);
	init_pair(6, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(7, COLOR_BLACK, COLOR_YELLOW);
	init_pair(8, COLOR_BLACK, COLOR_WHITE);

	init_pair(10, COLOR_BLACK, COLOR_BLACK);
	init_pair(20, COLOR_BLUE, COLOR_BLACK);
	init_pair(30, COLOR_GREEN, COLOR_BLACK);
	init_pair(40, COLOR_CYAN, COLOR_BLACK);
	init_pair(50, COLOR_RED, COLOR_BLACK);
	init_pair(60, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(70, COLOR_YELLOW, COLOR_BLACK);
	init_pair(80, COLOR_WHITE, COLOR_BLACK);

	if (!has_colors()) {
		printw("Terminal does not support color output!!!!!");
	}
	clear();
	curs_set(0);
}

WINDOW* create_win(WINDOW* window, int width, int height, int start_y, int start_x) {
	window = newwin(width, height, start_y, start_x);
	if (window == NULL) {
		error_message("ERROR func create_win");
		longjmp(jmp_buffer10, 1);
	}
	return window;
}

void destroy_screen() {
    endwin();
}
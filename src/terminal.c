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
#include <string.h>

extern jmp_buf jmp_buffer10;

WINDOW* appWindows[4];

void wmanual_box(WINDOW* win, int x, int y, int width, int height) {
	mvwaddch(win, y, x, ACS_ULCORNER);
	mvwaddch(win, y, x + width - 1, ACS_URCORNER);
	mvwaddch(win, y + height - 1, x, ACS_LLCORNER);
	mvwaddch(win, y + height - 1, x + width - 1, ACS_LRCORNER);

	for (int i = 1; i < width - 1; i++) {
		mvwaddch(win, y, x + i, ACS_HLINE);
		mvwaddch(win, y + height - 1, x + i, ACS_HLINE);
	}

	for (int i = 1; i < height - 1; i++) {
		mvwaddch(win, y + i, x, ACS_VLINE);
		mvwaddch(win, y + i, x + width - 1, ACS_VLINE);
	}
}

WINDOW* create_win(int width, int height, int start_y, int start_x) {
	WINDOW* window = newwin(width, height, start_y, start_x);
	if (window == NULL) {
		error_message("ERROR: func create_win(): newwin() failed");
		longjmp(jmp_buffer10, 1);
	}
	return window;
}

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
	memset(appWindows, 0, sizeof(appWindows));
}

void create_app_windows() {
	appWindows[INPUT1_WIN] = create_win(1, 1, 1, 1);
	appWindows[INPUT2_WIN] = create_win(1, 1, 1, 1);
	appWindows[GAME_WIN] = create_win(appArgs.pConfig->configs[SCREEN_HEIGHT] - 6, appArgs.pConfig->configs[SCREEN_WIDTH] - 6, 3, 3);
	appWindows[MENU_WIN] = create_win(15, 30, (appArgs.pConfig->configs[SCREEN_HEIGHT] - 15) / 2, (appArgs.pConfig->configs[SCREEN_WIDTH] - 30) / 2);
}

void destroy_screen() {
	delwin(appWindows[INPUT1_WIN]);
	delwin(appWindows[INPUT2_WIN]);
	delwin(appWindows[GAME_WIN]);
	delwin(appWindows[MENU_WIN]);
	endwin();
}
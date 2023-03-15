/*
 ============================================================================
 Name        : mymenu.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>
#include <setjmp.h>
#include "appdata.h"
#include "thread.h"



#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

char* choices[] = {
					"Single Player",
					" Multiplayer ",
					"  Settings   ",
					"    Exit     "
				  };




void single_start() {
	list_printer(appArgs.pWall, appArgs.pConfig->WALL_COLOR, 0, appArgs.window_game);
	resume_thread(&appArgs.pInput1->pause_flag, &appArgs.pInput1->thr_mutex, &appArgs.pInput1->pause_cond);
	resume_thread(&appArgs.pInput2->pause_flag, &appArgs.pInput2->thr_mutex, &appArgs.pInput2->pause_cond);
	resume_thread(&appArgs.pSnake1->pause_flag, &appArgs.pSnake1->snake_mutex, &appArgs.pSnake1->pause_cond);
}

void* menu_thread(void* args) {
	menuArgs* pmenuArgs = (menuArgs*)args;
	cbreak;
	noecho();
	keypad(appArgs.window_menu, TRUE);
	nodelay(appArgs.window_menu, FALSE);

	ITEM** menuItems;
	ITEM* curItem;
	MENU* gameMenu;
	int n_choices = ARRAY_SIZE(choices);
	int key;
	menuItems = (ITEM**)calloc(n_choices + 1, sizeof(ITEM*));

	for (int i = 0; i < n_choices; i++) {
		menuItems[i] = new_item(choices[i], NULL);
	}
	set_item_userptr(menuItems[0], single_start);
	void(*p)();


	while (appArgs.appState) {
		pthread_mutex_lock(&pmenuArgs->thr_mutex);
		while (pmenuArgs->pause_flag) {
			pthread_cond_wait(&pmenuArgs->pause_cond, &pmenuArgs->thr_mutex);
		}
		gameMenu = new_menu((ITEM**)menuItems);
		set_menu_win(gameMenu, appArgs.window_menu);
		set_menu_sub(gameMenu, derwin(appArgs.window_menu, 4, 13, 3, 2));
		set_menu_format(gameMenu, 4, 1);
		set_menu_mark(gameMenu, "");
		box(appArgs.window_menu, 0, 0);
		mvwaddch(appArgs.window_menu, 2, 0, ACS_LTEE);
		mvwhline(appArgs.window_menu, 2, 1, ACS_HLINE, 15);
		mvwaddch(appArgs.window_menu, 2, 16, ACS_RTEE);
		mvwprintw(appArgs.window_menu, 1, 3, "Snake by IP");
		post_menu(gameMenu);
		wrefresh(appArgs.window_menu);
		while ((key = wgetch(appArgs.window_menu)) != '\n') {
		switch (key) {
		case KEY_DOWN:
			menu_driver(gameMenu, REQ_DOWN_ITEM);
			break;
		case KEY_UP:
			menu_driver(gameMenu, REQ_UP_ITEM);
			break;
		}
		}
		curItem = current_item(gameMenu);
		p = item_userptr(curItem);
		p();


		
	}
	pthread_exit(NULL);
}
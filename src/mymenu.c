/*
 ============================================================================
 Name        : mymenu.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include "appdata.h"
#include "thread.h"

extern jmp_buf jmp_buffer10;
jmp_buf jmp_buffer11;


char* main_menu_names[4] = { "Single Player", " Multiplayer ", "  Settings   ", "    Quit     " };
char* pause_menu_names[4] = { "  Continue   ", "    Back     ", "  Settings   ", "    Quit     " };
//char* settings[];//TODO add settings



void func_Single_Player() {
	wclear(appArgs.window_game);
	pthread_mutex_lock(&GameThreads.thr_mutex[thr_menu]);
	resume_thread(thr_main);
	pthread_cond_wait(&GameThreads.pause_cond[thr_menu], &GameThreads.thr_mutex[thr_menu]);
	pthread_mutex_unlock(&GameThreads.thr_mutex[thr_menu]);
	GAME_STATE = STARTED;
	GAME_MODE = SINGLE_PLAYER;
	resume_thread(thr_input1);
	resume_thread(thr_input2);
	list_printer(appArgs.pWall, appArgs.pConfig->WALL_COLOR, 0, appArgs.window_game);
	appArgs.pSnake1->is_alive = true;
	resume_thread(thr_snake1);
}

void func_Multiplayer() {
	wclear(appArgs.window_game);
	pthread_mutex_lock(&GameThreads.thr_mutex[thr_menu]);
	resume_thread(thr_main);
	pthread_cond_wait(&GameThreads.pause_cond[thr_menu], &GameThreads.thr_mutex[thr_menu]);
	pthread_mutex_unlock(&GameThreads.thr_mutex[thr_menu]);
	GAME_STATE = STARTED;
	GAME_MODE = MULTIPLAYER;
	resume_thread(thr_input1);
	resume_thread(thr_input2);
	list_printer(appArgs.pWall, appArgs.pConfig->WALL_COLOR, 0, appArgs.window_game);
	appArgs.pSnake1->is_alive = true;
	resume_thread(thr_snake1);
	appArgs.pSnake2->is_alive = true;
	resume_thread(thr_snake2);
}

void func_Settings() {
	//TODO implement
}

void func_Quit() {
	//TODO turn appArgs.appState to enum
	GAME_STATE = QUIT;
	pause_thread(thr_input1);
	pause_thread(thr_input2);
	pause_thread(thr_snake1);
	pause_thread(thr_snake2);
	resume_thread(thr_main);
}

void func_Continue() {
	switch (GAME_MODE) {
	case SINGLE_PLAYER:
		func_Single_Player();
		break;
	case MULTIPLAYER:
		func_Multiplayer();
		break;
	}
}

void func_Back() {
	GAME_STATE = MAIN_MENU;
	resume_thread(thr_menu);
}

void delete_menu(MENU* menu, ITEM** items, int n_choices) {
	if (menu != NULL) {
		free_menu(menu);
	}
	if (items != NULL) {
		for (int i = 0; i < n_choices; i++) {
			free_item(items[i]);
		}
	}
}

void delete_menuThrArgs() {
	if (appArgs.pMenuThrArgs == NULL) {
		return;
	}
	delete_menu(appArgs.pMenuThrArgs->main_menu, appArgs.pMenuThrArgs->main_menu_items, appArgs.pMenuThrArgs->n_choices_main);
	delete_menu(appArgs.pMenuThrArgs->pause_menu, appArgs.pMenuThrArgs->pause_menu_items, appArgs.pMenuThrArgs->n_choices_pause);
	//TODO Settings
	// delete_menu(appArgs.pMenuThrArgs->settings_menu, appArgs.pMenuThrArgs->settings_menu_items, appArgs.pMenuThrArgs->n_choices_settings);
}

ITEM* create_item(char* string1, char* string2) {
	ITEM* item = NULL;
	if (string2 == NULL) {
		item = new_item(string1, NULL);
	}
	else {
		item = new_item(string1, string2);
	}
	if (item == NULL) {
		longjmp(jmp_buffer11, 1);
	}
}

ITEM** create_item_list(char* choices1[], char* choices2[], int n_choices) {
	ITEM** items = (ITEM**)calloc(n_choices, sizeof(ITEM*));
	memset(items, 0, n_choices * sizeof(ITEM*));
	if (items == NULL) {
		longjmp(jmp_buffer10, 1);
	}
	
	if (setjmp(jmp_buffer11) != 1) {
		for (int i = 0; i < n_choices; i++) {
			items[i] = create_item(choices1[i], NULL);

		}
		items[n_choices] = (ITEM*)NULL;
	}
	else {
		for (int i = 0; i < n_choices; i++) {
			free_item(items[i]);
			items[i] = NULL;
		}
		longjmp(jmp_buffer10, 1);
	}
	return items;
}

MENU* create_menu(ITEM** items) {
	MENU* menu = NULL;
	menu = new_menu(items);
	if (menu == NULL) {
		longjmp(jmp_buffer10, 1);
	}
	return menu;
}

void set_menu_attr(MENU* menu) {
	int width, height;
	set_menu_win(menu, appArgs.window_menu);
	width = getmaxx(appArgs.window_menu);
	height = getmaxy(appArgs.window_menu);
	WINDOW* subwindow = NULL;
	subwindow = derwin(appArgs.window_menu, height - 4, width - 3, 3, 2);
	if (subwindow == NULL) {
		longjmp(jmp_buffer10, 1);
	}
	set_menu_sub(menu, subwindow);
	set_menu_mark(menu, "");
}

MenuThrArgs* create_menuThrArgs() {
	MenuThrArgs* pMenuThrArgs = (MenuThrArgs*)malloc(sizeof(MenuThrArgs));
	if (pMenuThrArgs == NULL) {
		longjmp(jmp_buffer10, 1);
	}
	
	pMenuThrArgs->n_choices_main = ARRAY_SIZE(main_menu_names);
	pMenuThrArgs->main_menu_items = create_item_list(main_menu_names, NULL, pMenuThrArgs->n_choices_main);
	pMenuThrArgs->main_menu = create_menu(pMenuThrArgs->main_menu_items);
	set_menu_attr(pMenuThrArgs->main_menu);
	
	pMenuThrArgs->n_choices_pause = ARRAY_SIZE(pause_menu_names);
	pMenuThrArgs->pause_menu_items =  create_item_list(pause_menu_names, NULL, pMenuThrArgs->n_choices_pause);
	pMenuThrArgs->pause_menu = create_menu(pMenuThrArgs->pause_menu_items);
	set_menu_attr(pMenuThrArgs->pause_menu);
	
	//TODO add settings
	return pMenuThrArgs;
}

void* menu_thread(void* args) {
	MenuThrArgs* pMenuThrArgs = (MenuThrArgs*)args;
	cbreak;
	noecho();
	keypad(appArgs.window_menu, TRUE);
	nodelay(appArgs.window_menu, FALSE);
	

	ITEM* curItem;
	int key;
	int thrnum = get_thrnum(pthread_self());
	MENU* curMenu = pMenuThrArgs->main_menu;
	//TODO turn these to a function
	set_item_userptr(pMenuThrArgs->main_menu->items[0], func_Single_Player);
	set_item_userptr(pMenuThrArgs->main_menu->items[1], func_Multiplayer);
	set_item_userptr((pMenuThrArgs->main_menu->items[2]), func_Settings);
	set_item_userptr((pMenuThrArgs->main_menu->items[3]), func_Quit);
	set_menu_format(pMenuThrArgs->main_menu, 4, 1);
	
	set_item_userptr(pMenuThrArgs->pause_menu->items[0], func_Continue);
	set_item_userptr(pMenuThrArgs->pause_menu->items[1], func_Back);
	set_item_userptr(pMenuThrArgs->pause_menu->items[2], func_Settings);
	set_item_userptr(pMenuThrArgs->pause_menu->items[3], func_Quit);
	set_menu_format(pMenuThrArgs->pause_menu, 4, 1);
	
	void(*p)();


	while (GameThreads.is_thr_init[thrnum]) {
		pthread_mutex_lock(&GameThreads.thr_mutex[thrnum]);
		while (GameThreads.pause_flag[thrnum]) {
			pthread_cond_wait(&GameThreads.pause_cond[thrnum], &GameThreads.thr_mutex[thrnum]);
			if (GAME_STATE == QUIT) {
				pthread_mutex_unlock(&GameThreads.thr_mutex[thrnum]);
				pthread_exit(NULL);
			}
		}
		pthread_mutex_unlock(&GameThreads.thr_mutex[thrnum]);
		
		box(appArgs.window_menu, 0, 0);
		mvwaddch(appArgs.window_menu, 2, 0, ACS_LTEE);
		mvwhline(appArgs.window_menu, 2, 1, ACS_HLINE, 15);
		mvwaddch(appArgs.window_menu, 2, 16, ACS_RTEE);
		mvwprintw(appArgs.window_menu, 1, 3, "Snake by IP");
		switch (GAME_STATE) {
		case MAIN_MENU:
			curMenu = pMenuThrArgs->main_menu;
			break;
		case STARTED:
			curMenu = pMenuThrArgs->pause_menu;
			break;
		}
		
		post_menu(curMenu);
		wrefresh(appArgs.window_menu);
		while ((key = wgetch(appArgs.window_menu)) != '\n') {
		switch (key) {
		case KEY_DOWN:
			menu_driver(curMenu, REQ_DOWN_ITEM);
			break;
		case KEY_UP:
			menu_driver(curMenu, REQ_UP_ITEM);
			break;
		}
		}
		curItem = current_item(curMenu);
		p = item_userptr(curItem);
		unpost_menu(curMenu);
		wclear(appArgs.window_menu);
		wrefresh(appArgs.window_menu);
		pause_thread(thr_menu);
		p();

	}
	pthread_exit(NULL);
}
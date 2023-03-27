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
#include "config.h"
#include "error_message.h"

#include "terminal.h"

extern jmp_buf jmp_buffer10;
jmp_buf jmp_buffer11;


char* main_menu_names[4] = {
	"Single Player",
	" Multiplayer ",
	"  Settings   ",
	"    Quit     "
};
char* pause_menu_names[4] = {
	"  Continue   ",
	"    Back     ",
	"  Settings   ",
	"    Quit     "
};

char* settings[5] = {
	"Player 1 Color",
	"Player 2 Color",
	"Wall Color",
	"Background Color",
	"Food Color"
};
char* config_value_names[5];


void func_Single_Player() {
	wclear(appWindows[GAME_WIN]);
	pthread_mutex_lock(&GameThreads.thr_mutex[thr_menu]);
	resume_thread(thr_main);
	pthread_cond_wait(&GameThreads.pause_cond[thr_menu], &GameThreads.thr_mutex[thr_menu]);
	pthread_mutex_unlock(&GameThreads.thr_mutex[thr_menu]);
	GAME_STATE = STARTED;
	GAME_MODE = SINGLE_PLAYER;
	resume_thread(thr_input1);
	resume_thread(thr_input2);
	list_printer(appArgs.pWall, appArgs.pConfig->configs[WALL_COLOR], 0, appWindows[GAME_WIN]);
	appArgs.pSnake1->is_alive = true;
	resume_thread(thr_snake1);
}

void func_Multiplayer() {
	wclear(appWindows[GAME_WIN]);
	pthread_mutex_lock(&GameThreads.thr_mutex[thr_menu]);
	resume_thread(thr_main);
	pthread_cond_wait(&GameThreads.pause_cond[thr_menu], &GameThreads.thr_mutex[thr_menu]);
	pthread_mutex_unlock(&GameThreads.thr_mutex[thr_menu]);
	GAME_STATE = STARTED;
	GAME_MODE = MULTIPLAYER;
	resume_thread(thr_input1);
	resume_thread(thr_input2);
	list_printer(appArgs.pWall, appArgs.pConfig->configs[WALL_COLOR], 0, appWindows[GAME_WIN]);
	appArgs.pSnake1->is_alive = true;
	resume_thread(thr_snake1);
	appArgs.pSnake2->is_alive = true;
	resume_thread(thr_snake2);
}

void func_Settings() {
	//TODO implement
	GAME_STATE = SETTINGS;
	settingsfunction();
	GAME_STATE = MAIN_MENU;
	resume_thread(thr_menu);
	
}

void func_Quit() {
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
	delete_menu(appArgs.pMenuThrArgs->settings_menu, appArgs.pMenuThrArgs->settings_menu_items, appArgs.pMenuThrArgs->n_choices_settings);
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
			if (choices2 != NULL && choices2[i] != NULL) {
				items[i] = create_item(choices1[i], choices2[i]);
			}
			else {
				items[i] = create_item(choices1[i], NULL);
			}
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

void get_config_value_names(char* config_value_names[]) {
	//TODO turn 5 to original value
	for (int i = 0; i < 5; i++) {
		config_value_names[i] = num_to_color(appArgs.pConfig->configs[i]);
	}
}

void settingsfunction() {
	ITEM* curitem;
	int key, value, setting;

	print_menu(appArgs.pMenuThrArgs->settings_menu, "Settings");
	while (key != KEY_F(1)) {	
		while ((key = wgetch(appWindows[MENU_WIN])) != '\n') {
			switch (key) {
			case KEY_DOWN:
				menu_driver(appArgs.pMenuThrArgs->settings_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(appArgs.pMenuThrArgs->settings_menu, REQ_UP_ITEM);
				break;
			}
			if (key == KEY_F(1)) {
				break;
			}
		}
		if (key == KEY_F(1)) {
			break;
		}
		curitem = current_item(appArgs.pMenuThrArgs->settings_menu);
		setting = item_index(curitem);
		value = value_changer(curitem, appArgs.pMenuThrArgs->settings_menu);
		change_config(setting, value);

	}
	erase_menu(appArgs.pMenuThrArgs->settings_menu);
}

void change_config(int setting, int value) {
	switch (setting) {
	case 0:
		appArgs.pConfig->configs[PLAYER_1_COLOR] = value;
		break;
	case 1:
		appArgs.pConfig->configs[PLAYER_2_COLOR] = value;
		break;
	case 2:
		appArgs.pConfig->configs[WALL_COLOR] = value;
		break;
	case 3:
		appArgs.pConfig->configs[BACKGROUND_COLOR] = value;
		break;
	case 4:
		appArgs.pConfig->configs[FOOD_COLOR] = value;
		break;
	case 5:
		appArgs.pConfig->configs[TEXT_1_COLOR] = value;
		break;
	case 6:
		appArgs.pConfig->configs[TEXT_2_COLOR] = value;
		break;
	case 7:
		appArgs.pConfig->configs[PLAYER_1_UP] = value;
		break;
	case 8:
		appArgs.pConfig->configs[PLAYER_1_LEFT] = value;
		break;
	case 9:
		appArgs.pConfig->configs[PLAYER_1_RIGHT] = value;
		break;
	case 10:
		appArgs.pConfig->configs[PLAYER_1_DOWN] = value;
		break;
	case 11:
		appArgs.pConfig->configs[PLAYER_2_UP] = value;
		break;
	case 12:
		appArgs.pConfig->configs[PLAYER_2_LEFT] = value;
		break;
	case 13:
		appArgs.pConfig->configs[PLAYER_2_RIGHT] = value;
		break;
	case 14:
		appArgs.pConfig->configs[PLAYER_2_DOWN] = value;
		break;
	case 15:
		appArgs.pConfig->configs[SCREEN_OFFSET_X] = value;
		break;
	case 16:
		appArgs.pConfig->configs[SCREEN_OFFSET_Y] = value;
		break;
	}
	write_config(appArgs.pConfig);
}

int value_changer(ITEM* item, MENU* menu) {
	int key;
	int color_number = color_to_num(item->description.str);
	while ((key = wgetch(appWindows[MENU_WIN])) != '\n') {
		switch (key) {
		case KEY_DOWN:
			color_number--;
			if (color_number == 0) {
				color_number = 8;
			}
			item->description.str = num_to_color(color_number);
			unpost_menu(menu);
			post_menu(menu);
			break;
		case KEY_UP:
			color_number++;
			if (color_number == 9) {
				color_number = 1;
			}
			item->description.str = num_to_color(color_number);
			unpost_menu(menu);
			post_menu(menu);
			break;
		}
	}
	return color_number;
}

int color_to_num(const char* str) {
	if (strcmp(str, "  Black") == 0) {
		return 1;
	}
	else if (strcmp(str, "   Blue") == 0) {
		return 2;
	}
	else if (strcmp(str, "  Green") == 0) {
		return 3;
	}
	else if (strcmp(str, "   Cyan") == 0) {
		return 4;
	}
	else if (strcmp(str, "    Red") == 0) {
		return 5;
	}
	else if (strcmp(str, "Magenta") == 0) {
		return 6;
	}
	else if (strcmp(str, " Yellow") == 0) {
		return 7;
	}
	else if (strcmp(str, "  White") == 0) {
		return 8;
	}
}

char* num_to_color(int num) {
	switch (num) {
	case 1:
		return "  Black";
	case 2:
		return "   Blue";
	case 3:
		return "  Green";
	case 4:
		return "   Cyan";
	case 5:
		return "    Red";
	case 6:
		return "Magenta";
	case 7:
		return " Yellow";
	case 8:
		return "  White";
	}
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
	set_menu_win(menu, appWindows[MENU_WIN]);
	width = getmaxx(appWindows[MENU_WIN]);
	height = getmaxy(appWindows[MENU_WIN]);
	WINDOW* subwindow = NULL;
	subwindow = derwin(appWindows[MENU_WIN], height - 4, width - 3, 3, 2);
	if (subwindow == NULL) {
		error_message("ERROR: func set_menu_attr: derwin() failed");
		longjmp(jmp_buffer10, 1);
	}
	set_menu_sub(menu, subwindow);
	set_menu_mark(menu, "");
	set_menu_format(menu, 4, 1);
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
	set_item_userptr(pMenuThrArgs->main_menu->items[0], func_Single_Player);
	set_item_userptr(pMenuThrArgs->main_menu->items[1], func_Multiplayer);
	set_item_userptr((pMenuThrArgs->main_menu->items[2]), func_Settings);
	set_item_userptr((pMenuThrArgs->main_menu->items[3]), func_Quit);

	pMenuThrArgs->n_choices_pause = ARRAY_SIZE(pause_menu_names);
	pMenuThrArgs->pause_menu_items =  create_item_list(pause_menu_names, NULL, pMenuThrArgs->n_choices_pause);
	pMenuThrArgs->pause_menu = create_menu(pMenuThrArgs->pause_menu_items);
	set_menu_attr(pMenuThrArgs->pause_menu);
	set_item_userptr(pMenuThrArgs->pause_menu->items[0], func_Continue);
	set_item_userptr(pMenuThrArgs->pause_menu->items[1], func_Back);
	set_item_userptr(pMenuThrArgs->pause_menu->items[2], func_Settings);
	set_item_userptr(pMenuThrArgs->pause_menu->items[3], func_Quit);

	//TODO add settings
	pMenuThrArgs->n_choices_settings = ARRAY_SIZE(settings);
	get_config_value_names(config_value_names);
	pMenuThrArgs->settings_menu_items = create_item_list(settings, config_value_names, pMenuThrArgs->n_choices_settings);
	pMenuThrArgs->settings_menu = create_menu(pMenuThrArgs->settings_menu_items);
	set_menu_attr(pMenuThrArgs->settings_menu);

	return pMenuThrArgs;
}

void print_menu(MENU* menu, char* title) {
	int height = getmaxy(appWindows[MENU_WIN]);
	int width = getmaxx(appWindows[MENU_WIN]);
    box(appWindows[MENU_WIN], 0, 0);
	mvwaddch(appWindows[MENU_WIN], 2, 0, ACS_LTEE);
	mvwhline(appWindows[MENU_WIN], 2, 1, ACS_HLINE, width);
	mvwaddch(appWindows[MENU_WIN], 2, width - 1, ACS_RTEE);
	mvwprintw(appWindows[MENU_WIN], 1, (width - strlen(title)) / 2, "%s", title);
	post_menu(menu);
	wrefresh(appWindows[MENU_WIN]);
}

void erase_menu(MENU* menu) {
	unpost_menu(menu);
	wclear(appWindows[MENU_WIN]);
	wrefresh(appWindows[MENU_WIN]);
}


void* menu_thread(void* args) {
	MenuThrArgs* pMenuThrArgs = (MenuThrArgs*)args;
	cbreak;
	noecho();
	keypad(appWindows[MENU_WIN], TRUE);
	nodelay(appWindows[MENU_WIN], FALSE);
	

	ITEM* curItem;
	int key;
	int thrnum = get_thrnum(pthread_self());
	MENU* curMenu = pMenuThrArgs->main_menu;
	
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
		
		switch (GAME_STATE) {
		case MAIN_MENU:
			curMenu = pMenuThrArgs->main_menu;
			print_menu(curMenu, "Snake by IP");
			break;
		case STARTED:
			curMenu = pMenuThrArgs->pause_menu;
			print_menu(curMenu, "Game Paused");	
			break;
		}
		
		while ((key = wgetch(appWindows[MENU_WIN])) != '\n') {
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
		erase_menu(curMenu);
		pause_thread(thr_menu);
		p();

	}
	pthread_exit(NULL);
}
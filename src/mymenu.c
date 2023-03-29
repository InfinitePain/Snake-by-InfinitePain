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
#include <ctype.h>

#include "terminal.h"
#define SPACED 0
#define NOT_SPACED 1

extern jmp_buf jmp_buffer10;
jmp_buf jmp_buffer11;


char* main_menu_names[4] = {
	"Single Player",
	"Multiplayer",
	"Settings",
	"Quit"
};
char* pause_menu_names[4] = {
	"Continue",
	"Back",
	"Settings",
	"Quit"
};

char* settings_menu_names[NUM_CONFIGS - 2];
char* settings_menu_descriptions[NUM_CONFIGS - 2];


void func_Single_Player() {
	wclear(appWindows[GAME_WIN]);
	erase_info();
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
	erase_info();
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
	setting_info();
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
	free(appArgs.pMenuThrArgs);
	for (int i = 0; i < NUM_CONFIGS - 2; i++) {
		if (settings_menu_names[i] != NULL) {
			free(settings_menu_names[i]);
		}
		if (settings_menu_descriptions[i] != NULL) {
			free(settings_menu_descriptions[i]);
		}
	}
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

void settingsfunction() {
	int key = -1;
	int old_values[NUM_CONFIGS - 2];
	for (int i = 0; i < NUM_CONFIGS - 2; i++) {
		old_values[i] = appArgs.pConfig->configs[i];
	}
	bool save_flag = false;
	print_menu(appArgs.pMenuThrArgs);

	while (key != 27 && key != 34 && key != 94 && key != KEY_F(10) && key != 48) {
		while ((key = wgetch(appWindows[MENU_WIN])) != '\n' && key != 27 && key != 34 && key != 94 && key != KEY_F(10) && key != 48 && key != KEY_F(9) && key != 57) {

			switch (key) {
			case KEY_DOWN:
				menu_driver(appArgs.pMenuThrArgs->settings_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(appArgs.pMenuThrArgs->settings_menu, REQ_UP_ITEM);
				break;
			}
		}
		if (key == '\n') {
			settings_value_changer(appArgs.pMenuThrArgs->settings_menu);
		}
		if (key == KEY_F(10) || key == 48 || key == KEY_F(9) || key == 57) {
			write_config(appArgs.pConfig);
			save_flag = true;
		}
	}
	if (!save_flag) {
		for (int i = 0; i < NUM_CONFIGS - 2; i++) {
			if (old_values[i] != appArgs.pConfig->configs[i]) {
				appArgs.pConfig->configs[i] = old_values[i];
				appArgs.pMenuThrArgs->settings_menu_items[i]->description.str = config_value_to_string(i);
			}
		}
	}
	erase_menu(appArgs.pMenuThrArgs);
}

void color_value_changer(MENU* menu) {
	ITEM* item = current_item(menu);
	int index = item_index(item);
	int key = -1;
	int color_number = appArgs.pConfig->configs[index];
	while ((key = wgetch(appWindows[MENU_WIN])) != '\n' && key != 27 && key != 34 && key != 94) {
		switch (key) {
		case KEY_DOWN:
			color_number--;
			if (color_number == 0) {
				color_number = 8;
			}
			item->description.str = color_to_string(color_number);
			unpost_menu(menu);
			post_menu(menu);
			break;
		case KEY_UP:
			color_number++;
			if (color_number == 9) {
				color_number = 1;
			}
			item->description.str = color_to_string(color_number);
			unpost_menu(menu);
			post_menu(menu);
			break;
		}
	}
	if (key == '\n') {
		appArgs.pConfig->configs[index] = color_number;
	}
	else {
		item->description.str = color_to_string(appArgs.pConfig->configs[index]);
		unpost_menu(menu);
		post_menu(menu);
	}
}

void int_value_changer(MENU* menu) {
	ITEM* item = current_item(menu);
	int index = item_index(item);
	int key = -1;
	int value = appArgs.pConfig->configs[index];
	while ((key = wgetch(appWindows[MENU_WIN])) != '\n' && key != 27 && key != 34 && key != 94) {
		switch (key) {
		case KEY_DOWN:
			value--;
			if (value == 17) {
				value = 300;
			}
			item->description.str = int_to_string(value);
			unpost_menu(menu);
			post_menu(menu);
			break;
		case KEY_UP:
			value++;
			if (value == 301) {
				value = 18;
			}
			item->description.str = int_to_string(value);
			unpost_menu(menu);
			post_menu(menu);
			break;
			case KEY_LEFT:
				value -= 10;
				if (value == 17) {
					value = 300;
				}
				item->description.str = int_to_string(value);
				unpost_menu(menu);
				post_menu(menu);
				break;
			case KEY_RIGHT:
				value += 10;
				if (value == 301) {
					value = 18;
				}
				item->description.str = int_to_string(value);
				unpost_menu(menu);
				post_menu(menu);
				break;
		}
	}
	if (key == '\n') {
		appArgs.pConfig->configs[index] = value;
	}
	else {
		item->description.str = int_to_string(appArgs.pConfig->configs[index]);
		unpost_menu(menu);
		post_menu(menu);
	}
}

void key_value_changer(MENU* menu) {
	ITEM* item = current_item(menu);
	int index = item_index(item);
	int key = -1;
	int new_value;
	int value = appArgs.pConfig->configs[index];
	while ((key = wgetch(appWindows[MENU_WIN])) != '\n' && key != 27 && key != 34 && key != 94) {
		new_value = key;
		item->description.str = key_to_string(new_value);
		unpost_menu(menu);
		post_menu(menu);
	}
	if (key == '\n') {
		appArgs.pConfig->configs[index] = new_value;
	}
	else {
		item->description.str = int_to_string(appArgs.pConfig->configs[index]);
		unpost_menu(menu);
		post_menu(menu);
	}
}

void settings_value_changer(MENU* menu) {
	switch (item_index(current_item(menu))) {
	case PLAYER_1_COLOR:
	case PLAYER_2_COLOR:
	case WALL_COLOR:
	case BACKGROUND_COLOR:
	case FOOD_COLOR:
	case TEXT_1_COLOR:
		return color_value_changer(menu);
	case PLAYER_1_UP:
	case PLAYER_1_LEFT:
	case PLAYER_1_RIGHT:
	case PLAYER_1_DOWN:
	case PLAYER_2_UP:
	case PLAYER_2_LEFT:
	case PLAYER_2_RIGHT:
	case PLAYER_2_DOWN:
		return key_value_changer(menu);
	case SCREEN_WIDTH:
	case SCREEN_HEIGHT:
	case SNAKE_LENGTH:
		return int_value_changer(menu);
	}
}

const char* color_to_string(int color) {
	static const char* color_strings[8] = { "      Black", "       Blue", "      Green", "       Cyan", "        Red", "    Magenta", "     Yellow", "      White" };

	if (color >= 1 && color <= 8) {
		return color_strings[color - 1];
	}

	return "    Unknown";
}

const char* key_to_string(int key) {
	static char key_str[12] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 0};

	if (isprint(key)) {
		key_str[10] = (char)key;
		return key_str;
	}

	switch (key) {
	case KEY_UP: return "         UP";
	case KEY_DOWN: return "       DOWN";
	case KEY_LEFT: return "       LEFT";
	case KEY_RIGHT: return "      RIGHT";
	default: return "    Unknown";
	}
}

const char* int_to_string(int value) {
	static char int_str[12] = { 0 };
	snprintf(int_str, sizeof(int_str), "%11d", value);
	return int_str;
}

const char* config_value_to_string(int config_index) {
	switch (config_index) {
	case PLAYER_1_COLOR:
	case PLAYER_2_COLOR:
	case WALL_COLOR:
	case BACKGROUND_COLOR:
	case FOOD_COLOR:
	case TEXT_1_COLOR:
		return color_to_string(appArgs.pConfig->configs[config_index]);
	case PLAYER_1_UP:
	case PLAYER_1_LEFT:
	case PLAYER_1_RIGHT:
	case PLAYER_1_DOWN:
	case PLAYER_2_UP:
	case PLAYER_2_LEFT:
	case PLAYER_2_RIGHT:
	case PLAYER_2_DOWN:
		return key_to_string(appArgs.pConfig->configs[config_index]);
	case SCREEN_WIDTH:
	case SCREEN_HEIGHT:
	case SNAKE_LENGTH:
		return int_to_string(appArgs.pConfig->configs[config_index]);
	default:
		return "   Unknown";
	}
}

void get_settings_item_strings(char* item_names[], char* item_descriptions[]) {
	for (int i = 0; i < NUM_CONFIGS - 2; i++) {
		item_names[i] = NULL;
		item_descriptions[i] = NULL;
	}
	for (int i = 0; i < NUM_CONFIGS - 2; i++) {
		item_names[i] = strdup(config_names[i]);
		if (item_names[i] == NULL) {
			error_message("ERROR: func get_settings_item_strings(): strdup() failed");
			longjmp(jmp_buffer10, 1);
		}
    }

    for (int i = 0; i < NUM_CONFIGS - 2; i++) {
        item_descriptions[i] = strdup(config_value_to_string(i));
        if (item_descriptions[i] == NULL) {
			error_message("ERROR: func get_settings_item_strings(): strdup() failed");
			longjmp(jmp_buffer10, 1);
        }
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
	subwindow = derwin(appWindows[MENU_WIN], height - 4, width - 2, 3, 1);
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
	pMenuThrArgs->pause_menu_items = create_item_list(pause_menu_names, NULL, pMenuThrArgs->n_choices_pause);
	pMenuThrArgs->pause_menu = create_menu(pMenuThrArgs->pause_menu_items);
	set_menu_attr(pMenuThrArgs->pause_menu);
	set_item_userptr(pMenuThrArgs->pause_menu->items[0], func_Continue);
	set_item_userptr(pMenuThrArgs->pause_menu->items[1], func_Back);
	set_item_userptr(pMenuThrArgs->pause_menu->items[2], func_Settings);
	set_item_userptr(pMenuThrArgs->pause_menu->items[3], func_Quit);

	//TODO add settings
	pMenuThrArgs->n_choices_settings = ARRAY_SIZE(settings_menu_names);
	get_settings_item_strings(settings_menu_names, settings_menu_descriptions);
	pMenuThrArgs->settings_menu_items = create_item_list(settings_menu_names, settings_menu_descriptions, pMenuThrArgs->n_choices_settings);
	pMenuThrArgs->settings_menu = create_menu(pMenuThrArgs->settings_menu_items);
	set_menu_attr(pMenuThrArgs->settings_menu);
	set_menu_format(pMenuThrArgs->settings_menu, 10, 1);

	return pMenuThrArgs;
}

void print_menu(MenuThrArgs* menuArgs) {
	MENU* menu;
	char* title;
	bool is_spaced;
	switch (GAME_STATE) {
	case MAIN_MENU:
		title = "Snake by InfinitePain";
		menu = menuArgs->main_menu;
		is_spaced = true;
		break;
	case STARTED:
		title = "Game Paused";
		menu = menuArgs->pause_menu;
		is_spaced = true;
		break;
	case SETTINGS:
		title = "Settings";
		menu = menuArgs->settings_menu;
		is_spaced = false;
		break;
	}

	int height = getmaxy(appWindows[MENU_WIN]);
	int width = getmaxx(appWindows[MENU_WIN]);
	ITEM** items = menu_items(menu);
	int count = item_count(menu);
	int current = item_index(current_item(menu));
	int menu_height = count * 2;
	int title_spacing = 2;
	int menu_start_y = (height - menu_height) / 2 + title_spacing;


	post_menu(menu);
	if (is_spaced) {
		wclear(appWindows[MENU_WIN]);
		for (int i = 0; i < count; ++i) {
			if (i == current) {
				wattron(appWindows[MENU_WIN], A_REVERSE);
			}
			mvwprintw(appWindows[MENU_WIN], menu_start_y + 2 * i, (width - strlen(item_name(items[i]))) / 2, "%s", item_name(items[i]));
			if (i == current) {
				wattroff(appWindows[MENU_WIN], A_REVERSE);
			}
		}
	}
	box(appWindows[MENU_WIN], 0, 0);
	mvwaddch(appWindows[MENU_WIN], title_spacing, 0, ACS_LTEE);
	mvwhline(appWindows[MENU_WIN], title_spacing, 1, ACS_HLINE, width - 2);
	mvwaddch(appWindows[MENU_WIN], title_spacing, width - 1, ACS_RTEE);
	mvwprintw(appWindows[MENU_WIN], 1, (width - strlen(title)) / 2, "%s", title); // Print title with 1 line of spacing
	wrefresh(appWindows[MENU_WIN]);
}

void erase_menu(MenuThrArgs* menuArgs) {
	MENU* menu;
	switch (GAME_STATE) {
	case MAIN_MENU:
		menu = menuArgs->main_menu;
		break;
	case STARTED:
		menu = menuArgs->pause_menu;
		break;
	case SETTINGS:
		menu = menuArgs->settings_menu;
		break;
	}
	unpost_menu(menu);
	wclear(appWindows[MENU_WIN]);
	wrefresh(appWindows[MENU_WIN]);
}

void erase_info() {
	for (int i = 1; i < appArgs.pConfig->configs[SCREEN_WIDTH] - 8; i++) {
		mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, i, ' ');
	}
	wrefresh(stdscr);
}

void menu_info() {
	erase_info();
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 1, ACS_LARROW);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 2, ACS_LRCORNER);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 3, " to select highlighted option");
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 33, ACS_VLINE);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 35, ACS_UARROW);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 36, '/');
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 37, ACS_DARROW);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 38, " to change highlighted option");
	wrefresh(stdscr);
}

void setting_info() {
	erase_info();
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 1, ACS_LARROW);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 2, ACS_LRCORNER);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 4, "sel");
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 7, ACS_VLINE);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 8, ACS_UARROW);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 9, '/');
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 10, ACS_DARROW);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 12, "chg opt");
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 19, ACS_VLINE);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 21, "esc back");
	wrefresh(stdscr);
}

void chg_info() {
	erase_info();
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 1, ACS_LARROW);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 2, ACS_LRCORNER);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 4, "confirm");
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 12, ACS_VLINE);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 14, ACS_UARROW);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 15, '/');
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 16, ACS_DARROW);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 18, "chg val");
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 25, ACS_VLINE);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 27, "esc cancel");
	wrefresh(stdscr);
}

void* menu_thread(void* args) {
	MenuThrArgs* pMenuThrArgs = (MenuThrArgs*)args;
	cbreak;
	noecho();
	keypad(appWindows[MENU_WIN], TRUE);
	nodelay(appWindows[MENU_WIN], FALSE);


	ITEM* curItem;
	int key, index;
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
			menu_info();
			wrefresh(stdscr);
			break;
		case STARTED:
			curMenu = pMenuThrArgs->pause_menu;
			menu_info();
			break;
		case SETTINGS:
			curMenu = pMenuThrArgs->settings_menu;
			curMenu = pMenuThrArgs->pause_menu;
			break;
		}


		print_menu(pMenuThrArgs);
		index = 0;
		while ((key = wgetch(appWindows[MENU_WIN])) != '\n') {
			switch (key) {
			case KEY_DOWN:
				index++;
				if (index == 4) {
					index = 0;
					menu_driver(curMenu, REQ_FIRST_ITEM);
				}
				else {
					menu_driver(curMenu, REQ_DOWN_ITEM);
				}
				break;
			case KEY_UP:
				index--;
				if (index == -1) {
					index = 3;
					menu_driver(curMenu, REQ_LAST_ITEM);
				}
				else {
					menu_driver(curMenu, REQ_UP_ITEM);
				}
				break;
			}
			print_menu(pMenuThrArgs);
		}
		curItem = current_item(curMenu);
		p = item_userptr(curItem);
		erase_menu(pMenuThrArgs);
		pause_thread(thr_menu);
		p();

	}
	pthread_exit(NULL);
}
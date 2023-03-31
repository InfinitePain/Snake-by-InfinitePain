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
#include <ctype.h>
#include "appdata.h"
#include "error_message.h"
#include "terminal.h"

extern jmp_buf jmp_buffer10;
jmp_buf jmp_buffer11;

MENU* game_menus[3];
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
bool should_save = false;
char* title;
bool is_spaced;

void start_game(GameMode mode, bool is_new_game) {
	wclear(appWindows[GAME_WIN]);
	erase_info();
	if (is_new_game) {
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_menu]);
		resume_thread(thr_main);
		pthread_cond_wait(&GameThreads.pause_cond[thr_menu], &GameThreads.thr_mutex[thr_menu]);
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_menu]);
		appArgs.pSnake1->dir = MOVE_RIGHT;
		appArgs.pSnake2->dir = MOVE_RIGHT;
	}
	GAME_STATE = STARTED;
	GAME_MODE = mode;
	resume_thread(thr_input1);
	resume_thread(thr_input2);
	list_printer(appArgs.pWall, appArgs.pConfig->configs[WALL_COLOR], 0, appWindows[GAME_WIN]);
	appArgs.pSnake1->is_alive = true;
	resume_thread(thr_snake1);
	if (mode == MULTIPLAYER) {
		appArgs.pSnake2->is_alive = true;
		resume_thread(thr_snake2);
	}
}

void func_Single_Player() {
	start_game(SINGLE_PLAYER, true);
}

void func_Multiplayer() {
	start_game(MULTIPLAYER, true);
}

void func_Settings() {
	GAME_STATE = SETTINGS;
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
	start_game(GAME_MODE, false);
}

void func_Back() {
	GAME_STATE = MAIN_MENU;
	resume_thread(thr_menu);
}

void delete_menu(int index) {
	if (game_menus[index] != NULL) {
		ITEM** items = menu_items(game_menus[index]);
		if (items != NULL) {
			for (int i = 0; i < item_count(game_menus[index]); i++) {
				free_item(items[i]);
			}
		}
		free_menu(game_menus[index]);
	}
}

void delete_menus() {
	if (game_menus == NULL) {
		return;
	}
	for (int i = 0; i < 3; i++) {
		delete_menu(i);
	}
	for (int i = 0; i < NUM_CONFIGS - 2; i++) {
		if (settings_menu_names[i] != NULL) {
			free(settings_menu_names[i]);
		}
		if (settings_menu_descriptions[i] != NULL) {
			free(settings_menu_descriptions[i]);
		}
	}
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
	set_menu_format(menu, height - 4, 1);
}

ITEM* create_item(char* string1, char* string2) {
	ITEM* item = new_item(string1, string2);
	if (item == NULL) {
		longjmp(jmp_buffer11, 1);
	}
	return item;
}

MENU* create_menu(ITEM** items) {
	MENU* menu = new_menu(items);
	if (menu == NULL) {
		error_message("ERROR: func create_menu: new_menu() failed");
		longjmp(jmp_buffer11, 1);
	}
	return menu;
}

void free_items(ITEM** items, int n_choices) {
	for (int i = 0; i < n_choices; i++) {
		free_item(items[i]);
		items[i] = NULL;
	}
}

WINDOW* create_subwindow(WINDOW* parent, int height_offset, int width_offset, int y_offset, int x_offset) {
	int width, height;
	getmaxyx(parent, height, width);
	WINDOW* subwindow = derwin(parent, height - height_offset, width - width_offset, y_offset, x_offset);
	if (subwindow == NULL) {
		error_message("ERROR: func create_subwindow: derwin() failed");
		longjmp(jmp_buffer11, 1);
	}
	return subwindow;
}

MENU* create_game_menu(char* choices1[], char* choices2[], void* funcs[], int n_choices) {
	MENU* menu = NULL;
	ITEM** items = (ITEM**)calloc(n_choices + 1, sizeof(ITEM*));
	if (items == NULL) {
		longjmp(jmp_buffer10, 1);
	}
	if (setjmp(jmp_buffer11) != 1) {
		for (int i = 0; i < n_choices; i++) {
			items[i] = create_item(choices1[i], choices2 ? choices2[i] : NULL);
		}
		menu = create_menu(items);
		set_menu_win(menu, appWindows[MENU_WIN]);
		WINDOW* subwindow = create_subwindow(appWindows[MENU_WIN], 4, 2, 3, 1);
		set_menu_sub(menu, subwindow);
		set_menu_sub(menu, subwindow);
		set_menu_mark(menu, "");
		set_menu_format(menu, getmaxy(subwindow), 1);
	}
	else {
		free_items(items, n_choices);
		free_menu(menu);
		longjmp(jmp_buffer10, 1);
	}
	for (int i = 0; i < n_choices; i++) {
		set_item_userptr(items[i], funcs[i]);
	}

	return menu;
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

//TODO combine with config_value_to_string ask two arg and decide the index wether menu is NULL
void settings_value_changer(MENU* menu) {
	switch (item_index(current_item(menu))) {
	case PLAYER_1_COLOR:
	case PLAYER_2_COLOR:
	case WALL_COLOR:
	case BACKGROUND_COLOR:
	case FOOD_COLOR:
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

void get_settings_item_strings(char** item_names, char** item_descriptions) {
	for (int i = 0; i < NUM_CONFIGS - 2; i++) {
		item_names[i] = NULL;
		item_descriptions[i] = NULL;
	}
	for (int i = 0; i < NUM_CONFIGS - 2; i++) {
		item_names[i] = strdup(config_names[i]);
		item_descriptions[i] = strdup(config_value_to_string(i));
		if (item_names[i] == NULL || item_descriptions[i] == NULL) {
			error_message("ERROR: func get_settings_item_strings(): strdup() failed");
			longjmp(jmp_buffer10, 1);
		}
    }
}

void create_game_menus() {
	void (*menu_functions[][4])() = {
	{func_Single_Player, func_Multiplayer, func_Settings, func_Quit},
	{func_Continue, func_Back, func_Settings, func_Quit},
	{NULL, NULL, NULL, NULL}
	};
	get_settings_item_strings(settings_menu_names, settings_menu_descriptions);
    char ** menu_names[3] = {main_menu_names, pause_menu_names, settings_menu_names};
	char** descriptions[3] = { NULL, NULL, settings_menu_descriptions };
	int n_choices[3] = { ARRAY_SIZE(main_menu_names), ARRAY_SIZE(pause_menu_names), ARRAY_SIZE(settings_menu_names) };
    for (int i = 0; i < 3; i++) {
        game_menus[i] = create_game_menu(menu_names[i], descriptions[i], (void**)menu_functions[i], n_choices[i]);
    }
}

void print_menu(MENU* menu) {
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

void erase_menu(MENU* menu) {
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

void print_info_enter(int x, char* text) {
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, x, ACS_LARROW);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, x+1, ACS_LRCORNER);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, x+4, text);
}

void print_info_ud_arrow(int x, char* text) {
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, x, ACS_UARROW);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, x+1, '/');
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, x+2, ACS_DARROW);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, x+4, text);
}


void menu_info() {
	erase_info();
	print_info_enter(1, "select");
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 11, ACS_VLINE);
	print_info_ud_arrow(13, "change option");
	wrefresh(stdscr);
}

void setting_info() {
	erase_info();
	print_info_enter(1, "select");
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 7, ACS_VLINE);
	print_info_ud_arrow(9, "change option");
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 19, ACS_VLINE);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 21, "esc back");
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 30, "F9 save");
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 38, "F10 save & exit");
	wrefresh(stdscr);
	wrefresh(stdscr);
}

void chg_info() {
	erase_info();
	print_info_enter(1, "confirm");
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 12, ACS_VLINE);
	print_info_ud_arrow(14, "change value");
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 25, ACS_VLINE);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 27, "esc cancel");
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 39, ACS_VLINE);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 40, ACS_LARROW);
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 41, '/');
	mvwaddch(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 42, ACS_RARROW);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, 44, "+/-10");
	wrefresh(stdscr);
}

void custom_menu_driver(MENU* menu, int key, bool* loop_flag) {
	int n_choices = item_count(menu);
	int index = item_index(current_item(menu));
	void (*pfunc)() = NULL;
	int old_values[NUM_CONFIGS - 2];
	if (GAME_STATE == SETTINGS) {
		for (int i = 0; i < NUM_CONFIGS - 2; i++) {
			old_values[i] = appArgs.pConfig->configs[i];
		}
	}
	
	switch (key) {
	case KEY_DOWN:
		index++;
		if (index == n_choices) {
			menu_driver(menu, REQ_FIRST_ITEM);
		}
		else {
			menu_driver(menu, REQ_DOWN_ITEM);
		}
		print_menu(menu);
		break;
	case KEY_UP:
		index--;
		if (index == -1) {
			menu_driver(menu, REQ_LAST_ITEM);
		}
		else {
			menu_driver(menu, REQ_UP_ITEM);
		}
		print_menu(menu);
		break;
	case '\n':
		if (GAME_STATE == NOT_STARTED || GAME_STATE == STARTED) {
			*loop_flag = false;
			erase_menu(menu);
			pause_thread(thr_menu);
			pfunc = item_userptr(current_item(menu));
			pfunc();
		}
		else if (GAME_STATE == SETTINGS) {
			settings_value_changer(menu);
		}
		break;
	case 27:
	case 34:
	case 94:
		if (GAME_STATE == SETTINGS) {
			*loop_flag = false;
			if (!should_save) {
				for (int i = 0; i < NUM_CONFIGS - 2; i++) {
					if (old_values[i] != appArgs.pConfig->configs[i]) {
						appArgs.pConfig->configs[i] = old_values[i];
						menu->items[i]->description.str = config_value_to_string(i);
					}
				}
			}
			erase_menu(menu);
			if (GAME_MODE == SINGLE_PLAYER || GAME_MODE == MULTIPLAYER) {
				GAME_STATE = STARTED;
			}
			else {
				GAME_STATE = NOT_STARTED;
			}
		}
		break;
		case KEY_F(9):
		case 57:
			if (GAME_STATE == SETTINGS) {
				should_save = true;
			}
			break;
		case KEY_F(10):
		case 48:
			if (GAME_STATE == SETTINGS) {
				should_save = true;
				*loop_flag = false;
				erase_menu(menu);
				if (GAME_MODE == SINGLE_PLAYER || GAME_MODE == MULTIPLAYER) {
					GAME_STATE = STARTED;
				}
				else {
					GAME_STATE = NOT_STARTED;
				}
			}
			break;
	}
}

void* menu_thread(void* args) {
	cbreak;
	noecho();
	keypad(appWindows[MENU_WIN], TRUE);
	nodelay(appWindows[MENU_WIN], FALSE);

	int key;
	int thrnum = get_thrnum(pthread_self());
	MENU* curMenu = game_menus[MAIN_MENU];
	bool loop_flag = true;
	int old_values[NUM_CONFIGS - 2];
	for (int i = 0; i < NUM_CONFIGS - 2; i++) {
		old_values[i] = appArgs.pConfig->configs[i];
	}

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
		case NOT_STARTED:
			curMenu = game_menus[MAIN_MENU];
			menu_info();
			title = "Snake by InfinitePain";
			is_spaced = true;
			break;
		case STARTED:
			curMenu = game_menus[PAUSE_MENU];
			menu_info();
			title = "Game Paused";
			is_spaced = true;
			break;
		case SETTINGS:
			curMenu = game_menus[SETTINGS_MENU];
			if (!should_save) {
				for (int i = 0; i < NUM_CONFIGS - 2; i++) {
					appArgs.pConfig->configs[i] = old_values[i];
					curMenu->items[i]->description.str = config_value_to_string(i);
				}
			}
			else {
				write_config(appArgs.pConfig);
				should_save = false;
				for (int i = 0; i < NUM_CONFIGS - 2; i++) {
					old_values[i] = appArgs.pConfig->configs[i];
				}
			}
			title = "Settings";
			is_spaced = false;
			break;
		}
		loop_flag = true;


		print_menu(curMenu);
		while (loop_flag) {
			key = wgetch(appWindows[MENU_WIN]);
			custom_menu_driver(curMenu, key, &loop_flag);
		}

	}
	pthread_exit(NULL);
}
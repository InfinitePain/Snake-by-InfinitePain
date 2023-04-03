/*
 ============================================================================
 Name        : mymenu.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef MYMENU_H_
#define MYMENU_H_

#include <ncurses.h>
#include <menu.h>
#include <pthread.h>
#include <stdlib.h>

#define MAIN_MENU 0
#define PAUSE_MENU 1
#define SETTINGS_MENU 2

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


extern MENU* game_menus[3];

void delete_menu(int index);
void delete_menus();
void set_menu_attr(MENU* menu);
ITEM* create_item(char* string1, char* string2);
MENU* create_menu(ITEM** items);
void free_items(ITEM** items, int n_choices);
WINDOW* create_subwindow(WINDOW* parent, int height_offset, int width_offset, int y_offset, int x_offset);
MENU* create_game_menu(char* choices1[], char* choices2[], void* funcs[], int n_choices);
void color_value_changer(MENU* menu);
void int_value_changer(MENU* menu);
void key_value_changer(MENU* menu);
void settings_value_changer(MENU* menu);
void color_to_string(int color, int index);
void key_to_string(int key, int index);
void config_value_to_string(int config_index);
void get_settings_item_strings(char** item_names, char** item_descriptions);
void create_game_menus();
void print_title(int width);
void print_menu(MENU* menu);
void erase_menu(MENU* menu);
void erase_info();
void print_info_enter(int x, char* text);
void print_info_ud_arrow(int x, char* text);
void menu_info();
void setting_info();
void chg_info(bool rl_arrow);
void* menu_thread(void* args);

#endif /*MYMENU_H_*/
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

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

typedef struct MenuThrArgs {
	MENU* main_menu;
	ITEM** main_menu_items;
	int n_choices_main;
	MENU* pause_menu;
	ITEM** pause_menu_items;
	int n_choices_pause;
	MENU* settings_menu;
	ITEM** settings_menu_items;
	int n_choices_settings;
} MenuThrArgs;

void delete_menu(MENU* menu, ITEM** items, int n_choices);
void delete_menuThrArgs();
ITEM* create_item(char* string1, char* string2);
ITEM** create_item_list(char* choices1[], char* choices2[], int n_choices);
void color_value_changer(MENU* menu);
void int_value_changer(MENU* menu);
void key_value_changer(MENU* menu);
void settings_value_changer(MENU* menu);
void settingsfunction();
const char* color_to_string(int color);
const char* key_to_string(int key);
const char* int_to_string(int value);
const char* config_value_to_string(int config_index);
void get_settings_item_strings(char* item_names[], char* item_descriptions[]);
MENU* create_menu(ITEM** items);
void set_menu_attr(MENU* menu);
MenuThrArgs* create_menuThrArgs();
void print_menu(MenuThrArgs* menuArgs);
void erase_menu(MenuThrArgs* menuArgs);
void erase_info();
void menu_info();
void setting_info();
void chg_info();
void* menu_thread(void* args);

#endif /*MYMENU_H_*/
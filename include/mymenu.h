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
	bool is_thr_init;
	bool pause_flag;
	pthread_mutex_t thr_mutex;
	pthread_cond_t pause_cond;
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
MENU* create_menu(ITEM** items);
void set_menu_attr(MENU* menu);
MenuThrArgs* create_menuThrArgs();
void* menu_thread(void* args);

#endif /*MYMENU_H_*/
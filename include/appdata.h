/*
 ============================================================================
 Name        : appData.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef APPDATA_H_
#define APPDATA_H_

#include <stdbool.h>
#include <pthread.h>
#include <ncurses.h>
#include "thread.h"
#include "config.h"
#include "input.h"
#include "mymenu.h"
#include "snake.h"
#include "wall.h"
#include "menu.h"

typedef struct appData {
	//main

	bool appState;

	//windows

	WINDOW* window_game;
	pthread_mutex_t mutex_win_game;
	WINDOW* window_menu;
	
	//game related

	MenuThrArgs* pMenuThrArgs;
	Config* pConfig;
	Input* pInput1;
	Input* pInput2;
	bool animation;
	List* pWall;
	Snake* pSnake1;
	Snake* pSnake2;
} appData;

extern appData appArgs;

void init_appData();
void destroy_appData();

#endif /*APPDATA_H_*/
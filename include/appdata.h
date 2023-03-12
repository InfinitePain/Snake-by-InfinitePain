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
#include "config.h"
#include "input.h"
#include "mymenu.h"
#include "alphabet.h"
#include "snake.h"
#include "wall.h"

typedef struct appData {
	//main
	
	bool appState;
	pthread_mutex_t mutex_main;
	pthread_cond_t cond_main;
	bool pause_flag_main;
	
	//threads
	
	pthread_t thr[5];
	
	//windows
	
	WINDOW* window_game;
	pthread_mutex_t mutex_win_game;
	WINDOW* window_menu;
	
	//game related
	
	Config* pConfig;
	Input* pInput1;
	Input* pInput2;
	Mymenu* pMymenu;
	Alphabet* pAlphabet;
	bool animation;
	List* pWall;
	Snake* pSnake1;
	Snake* pSnake2;
} appData;

extern appData appArgs;

#endif /*APPDATA_H_*/
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

typedef enum GameState {
	NOT_STARTED,
	STARTED,
	GAME_OVER,
	SETTINGS,
	QUIT,
	CRITICAL_ERROR,
} GameState;


typedef enum GameMode {
	NOT_SELECTED,
	SINGLE_PLAYER,
	MULTIPLAYER
} GameMode;

extern GameState GAME_STATE;
extern GameMode GAME_MODE;

typedef struct appData {
	Config* pConfig;
	List* pWall;
	Snake* pSnake1;
	Snake* pSnake2;
	List* pFood_Main;
	List* pFood_Multiplayer;
	List* pFood_Single_Player;
} appData;

extern appData appArgs;

void init_appData();
void destroy_appData();

#endif /*APPDATA_H_*/
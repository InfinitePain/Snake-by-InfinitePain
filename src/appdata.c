/*
 ============================================================================
 Name        : appdata.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include "appdata.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "terminal.h"
#include "food.h"

GameState GAME_STATE = MAIN_MENU;
GameMode GAME_MODE = NOT_SELECTED;

appData appArgs = {
	.pConfig = NULL,
	.pWall = NULL,
	.pSnake1 = NULL,
	.pSnake2 = NULL,
	.pFood_Main = NULL,
	.pFood_Multiplayer = NULL,
	.pFood_Single_Player = NULL,
};

void init_appData() {
	appArgs.pConfig = read_config();
	create_app_windows();
	create_game_menus();
	create_thread(thr_menu);
	create_thread(thr_input1);
	create_thread(thr_input2);
	appArgs.pWall = create_wall();
	appArgs.pSnake1 = create_snake();
	create_thread(thr_snake1);
	appArgs.pSnake1->dir = MOVE_RIGHT;
	appArgs.pSnake1->color = &appArgs.pConfig->configs[PLAYER_1_COLOR];
	set_snake_position(appArgs.pSnake1, 3, (getmaxy(appWindows[GAME_WIN]) / 2) - 1);
	appArgs.pSnake2 = create_snake();
	create_thread(thr_snake2);
	appArgs.pSnake2->dir = MOVE_LEFT;
	appArgs.pSnake2->color = &appArgs.pConfig->configs[PLAYER_2_COLOR];
	set_snake_position(appArgs.pSnake2, getmaxx(appWindows[GAME_WIN]) - 4, (getmaxy(appWindows[GAME_WIN]) / 2) + 1);
	init_foods();
	create_thread(thr_food);
	create_thread(thr_collision);
}

void destroy_appData() {
	delete_config(appArgs.pConfig);
	destroy_thread(thr_menu);
	delete_menus();
	destroy_thread(thr_input1);
	destroy_thread(thr_input2);
	delete_list(appArgs.pWall);
	destroy_thread(thr_snake1);
	delete_snake(appArgs.pSnake1);
	destroy_thread(thr_snake2);
	delete_snake(appArgs.pSnake2);
	destroy_thread(thr_food);
	delete_foods();
	destroy_thread(thr_collision);
	destroy_thread(thr_main);
}
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
#include "direction_buffer.h"


appData appArgs = {
	.pConfig = NULL,
	.pWall = NULL,
	.pSnake1 = NULL,
	.pSnake2 = NULL,
	.pFood_Main = NULL,
	.pFood_Multiplayer = NULL,
	.pFood_Single_Player = NULL,
	.pAvailablePositions = NULL,
};

void init_appData() {
	appArgs.pConfig = read_config();
	create_app_windows();
	create_game_menus();
	create_thread(thr_menu);
	create_thread(thr_input1);
	create_thread(thr_input2);
	appArgs.pWall = create_wall();
	appArgs.pSnake1 = create_snake(appArgs.pConfig->configs[SNAKE_LENGTH], appArgs.pConfig->configs[PLAYER_1_COLOR], MOVE_RIGHT, 3, (getmaxy(appWindows[GAME_WIN]) / 2) - 1);
	create_thread(thr_snake1);
	appArgs.pSnake2 = create_snake(appArgs.pConfig->configs[SNAKE_LENGTH], appArgs.pConfig->configs[PLAYER_2_COLOR], MOVE_LEFT, getmaxx(appWindows[GAME_WIN]) - 4, (getmaxy(appWindows[GAME_WIN]) / 2) - 1);
	create_thread(thr_snake2);
	init_foods(appArgs.pConfig->configs[FOOD_AMOUNT_SINGLE_PLAYER], appArgs.pConfig->configs[FOOD_AMOUNT_MULTIPLAYER]);
	appArgs.pAvailablePositions = create_available_positions(getmaxx(appWindows[GAME_WIN]), getmaxy(appWindows[GAME_WIN]));
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
	delete_available_positions(appArgs.pAvailablePositions);
	delete_foods();
	destroy_thread(thr_collision);
	destroy_thread(thr_main);
}

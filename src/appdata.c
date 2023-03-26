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
#include "terminal.h"

GameState GAME_STATE = MAIN_MENU;
GameMode GAME_MODE = NOT_SELECTED;

appData appArgs = {
	.window_game = NULL,
	.window_menu = NULL,
	.pMenuThrArgs = NULL,
	.pConfig = NULL,
	.pInput1 = NULL,
	.pInput2 = NULL,
	.animation = true,
	.pWall = NULL,
	.pSnake1 = NULL,
	.pSnake2 = NULL
};

void init_appData() {
	appArgs.window_game = create_win(appArgs.window_game, LINES, COLS, 0, 0);
	appArgs.window_menu = create_win(appArgs.window_menu, 8, 28, (LINES - 8) / 2, (COLS - 28) / 2);
	appArgs.pConfig = read_config();
	appArgs.pMenuThrArgs = create_menuThrArgs();
	create_thread(thr_menu);
	appArgs.pInput1 = create_input();
	create_thread(thr_input1);
	appArgs.pInput2 = create_input();
	create_thread(thr_input2);
	appArgs.pWall = create_wall();
	appArgs.pSnake1 = create_snake();
	create_thread(thr_snake1);
	appArgs.pSnake1->dir = MOVE_RIGHT;
	appArgs.pSnake1->color = &appArgs.pConfig->configs[PLAYER_1_COLOR];
	appArgs.pSnake2 = create_snake();
	create_thread(thr_snake2);
	appArgs.pSnake2->dir = MOVE_RIGHT;
	appArgs.pSnake2->color = &appArgs.pConfig->configs[PLAYER_2_COLOR];
}

void destroy_appData() {
	delwin(appArgs.window_game);
	delwin(appArgs.window_menu);
	destroy_thread(thr_menu);
	delete_menuThrArgs();
	delete_config(appArgs.pConfig);
	destroy_thread(thr_input1);
	delete_input(appArgs.pInput1);
	destroy_thread(thr_input2);
	delete_input(appArgs.pInput2);
	delete_list(appArgs.pWall);
	destroy_thread(thr_snake1);
	delete_snake(appArgs.pSnake1);
	destroy_thread(thr_snake2);
	delete_snake(appArgs.pSnake2);
	destroy_thread(thr_main);
}
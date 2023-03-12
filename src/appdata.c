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

appData appArgs = {
	.appState = true,
	.mutex_main = PTHREAD_MUTEX_INITIALIZER,
	.cond_main = PTHREAD_COND_INITIALIZER,
	.pause_flag_main = true,
	.window_game = NULL,
	.mutex_win_game = PTHREAD_MUTEX_INITIALIZER,
	.window_menu = NULL,
	.pConfig = NULL,
	.pInput1 = NULL,
	.pInput2 = NULL,
	.pMymenu = NULL,
	.pAlphabet = NULL,
	.animation = true,
	.pWall = NULL,
	.pSnake1 = NULL,
	.pSnake2 = NULL
};
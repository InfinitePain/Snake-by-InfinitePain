/*
 ============================================================================
 Name        : snakebyip.c
 Author      : InfinitePain
 Version     : 0.1.0
 License     : Apache License 2.0
 Description : Snake game made by InfinitePain
 ============================================================================
 */

#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "config.h"
#include "input.h"
#include <pthread.h>
#include "error_message.h"
#include "mymenu.h"
#include "terminal.h"
#include <setjmp.h>
#include <stdbool.h>
#include "appdata.h"
#include "thread.h"
#include "alphabet.h"

 /*
jmp_buffer1 for create_alphabet
jmp_buffer2  for create_letter_
jmp_buffer3  for copy_list
jmp_buffer4  for create_mymenu_options
jmp_buffer5  for create_mymenu_highlight
jmp_buffer6  for mymenu
jmp_buffer7  for init_wall
jmp_buffer8  for move_snake
jmp_buffer9  for single_player
jmp_buffer10 for main
 */

#define thr_input1 0
#define thr_input2 1
#define thr_mymenu 2
#define thr_snake1 3
#define thr_snake2 4
jmp_buf jmp_buffer10;

void init_main() {
	sleep(1);
	init_screen();
	appArgs.window_game = create_win(appArgs.window_game);
	appArgs.window_menu = create_win(appArgs.window_menu);
	appArgs.pConfig = read_config();
	appArgs.pInput1 = create_input();
	create_input_thread(&appArgs.thr[thr_input1], appArgs.pInput1);
	appArgs.pInput2 = create_input();
	create_input_thread(&appArgs.thr[thr_input2], appArgs.pInput2);
	appArgs.pMymenu = create_Mymenu(appArgs.pMymenu); //todo make this a thread
	appArgs.pAlphabet = create_alphabet();
	appArgs.pWall = create_wall();
	appArgs.pSnake1 = create_snake();
	create_snake_thread(&appArgs.thr[thr_snake1], appArgs.pSnake1);
	appArgs.pSnake1->dir = MOVE_RIGHT;
	appArgs.pSnake1->color = appArgs.pConfig->PLAYER_1_COLOR;
	appArgs.pSnake2 = create_snake();
	create_snake_thread(&appArgs.thr[thr_snake2], appArgs.pSnake2);
	appArgs.pSnake2->dir = MOVE_RIGHT;
	appArgs.pSnake2->color = appArgs.pConfig->PLAYER_2_COLOR;
}

void destroy_main() {
	pthread_mutex_destroy(&appArgs.mutex_main);
	pthread_cond_destroy(&appArgs.cond_main);
	delwin(appArgs.window_game);
	delwin(appArgs.window_menu);
	pthread_mutex_destroy(&appArgs.mutex_win_game);
	delete_config(appArgs.pConfig);
	destroy_thread(&appArgs.pInput1->is_thr_init, appArgs.thr[thr_input1], &appArgs.pInput1->pause_flag, &appArgs.pInput1->thr_mutex, &appArgs.pInput1->pause_cond);
	delete_input(appArgs.pInput1);
	destroy_thread(&appArgs.pInput2->is_thr_init, appArgs.thr[thr_input2], &appArgs.pInput2->pause_flag, &appArgs.pInput2->thr_mutex, &appArgs.pInput2->pause_cond);
	delete_input(appArgs.pInput2);
	delete_mymenu(appArgs.pMymenu);
    delete_alphabet(appArgs.pAlphabet);
	delete_list(appArgs.pWall);
	destroy_thread(&appArgs.pSnake1->is_thr_init, appArgs.thr[thr_snake1], &appArgs.pSnake1->pause_flag, &appArgs.pSnake1->snake_mutex, &appArgs.pSnake1->pause_cond);
	delete_snake(appArgs.pSnake1);
	destroy_thread(&appArgs.pSnake2->is_thr_init, appArgs.thr[thr_snake2], &appArgs.pSnake2->pause_flag, &appArgs.pSnake2->snake_mutex, &appArgs.pSnake2->pause_cond);
	delete_snake(appArgs.pSnake2);
}

int main(void) {
	if (setjmp(jmp_buffer10) != 1) {
		init_main();
	}
	else {
		destroy_main();
		mvprintw(error_line, 0, "ERROR: A critical error occurred. Press any key to close this window . . .");
		refresh();
		getchar();
		destroy_screen();
		return EXIT_FAILURE;
	}
	
	resume_thread(&appArgs.pInput1->pause_flag, &appArgs.pInput1->thr_mutex, &appArgs.pInput1->pause_cond);
	switch (mymenu())
	{
	case 0:
		resume_thread(&appArgs.pInput2->pause_flag, &appArgs.pInput2->thr_mutex, &appArgs.pInput2->pause_cond);
		list_printer(appArgs.pWall, appArgs.pConfig->WALL_COLOR, 0, appArgs.window_game);
		resume_thread(&appArgs.pSnake1->pause_flag, &appArgs.pSnake1->snake_mutex, &appArgs.pSnake1->pause_cond);
		pthread_join(appArgs.thr[1], NULL);
		break;
	case 1:
	 	resume_thread(&appArgs.pInput2->pause_flag, &appArgs.pInput2->thr_mutex, &appArgs.pInput2->pause_cond);
		list_printer(appArgs.pWall, appArgs.pConfig->WALL_COLOR, 0, appArgs.window_game);
		resume_thread(&appArgs.pSnake1->pause_flag, &appArgs.pSnake1->snake_mutex, &appArgs.pSnake1->pause_cond);
		resume_thread(&appArgs.pSnake2->pause_flag, &appArgs.pSnake2->snake_mutex, &appArgs.pSnake2->pause_cond);
		pthread_join(appArgs.thr[1], NULL);
		break;
	case 2:
		/* code */
		break;
	}


	getchar();
	destroy_main();
	destroy_screen();

	return EXIT_SUCCESS;
}

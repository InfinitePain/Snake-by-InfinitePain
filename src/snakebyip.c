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
#include "terminal.h"
#include <setjmp.h>
#include <stdbool.h>
#include "appdata.h"
#include "thread.h"
#include "mymenu.h"

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
jmp_buffer11 for create_item_list
 */


jmp_buf jmp_buffer10;

void init_main() {
	sleep(1);
	init_screen();
	appArgs.window_game = create_win(appArgs.window_game, LINES, COLS, 0, 0);
	appArgs.window_menu = create_win(appArgs.window_menu, 8, 17, (LINES - 8) / 2, (COLS - 17) / 2);
	//TODO error handling for window creation


	appArgs.pMenuThrArgs = create_menuThrArgs();
	create_thread(thr_menu);
	appArgs.pConfig = read_config();
	appArgs.pInput1 = create_input();
	create_thread(thr_input1);
	appArgs.pInput2 = create_input();
	create_thread(thr_input2);
	appArgs.pWall = create_wall();
	appArgs.pSnake1 = create_snake();
	create_thread(thr_snake1);
	appArgs.pSnake1->dir = MOVE_RIGHT;
	appArgs.pSnake1->color = appArgs.pConfig->PLAYER_1_COLOR;
	appArgs.pSnake2 = create_snake();
	create_thread(thr_snake2);
	appArgs.pSnake2->dir = MOVE_RIGHT;
	appArgs.pSnake2->color = appArgs.pConfig->PLAYER_2_COLOR;
}

void destroy_main() {
	pthread_mutex_destroy(&appArgs.mutex_main);
	pthread_cond_destroy(&appArgs.cond_main);
	delwin(appArgs.window_game);
	delwin(appArgs.window_menu);
	pthread_mutex_destroy(&appArgs.mutex_win_game);
	delete_menuThrArgs();
	destroy_thread(thr_menu);
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
	
	resume_thread(thr_menu);
	pthread_join(appArgs.thr[thr_menu], NULL);


	getchar();
	destroy_main();
	destroy_screen();

	return EXIT_SUCCESS;
}
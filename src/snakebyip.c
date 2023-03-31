/*
 ============================================================================
 Name        : snakebyip.c
 Author      : InfinitePain
 Version     : 0.7.3
 License     : Apache License 2.0
 Description : Snake game made by InfinitePain
 ============================================================================
 */

#include "CMakeProjectConfig.h"
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



int main(int argc, char* argv[]) {
	if (setjmp(jmp_buffer10) != 1) {
		init_screen();
		init_appData();
	}
	else {
		destroy_appData();
		mvprintw(error_line, 0, "ERROR: A critical error occurred. Press any key to close this window . . .");
		refresh();
		getchar();
		destroy_screen();
		return EXIT_FAILURE;
	}
	wmanual_box(stdscr, 0, 0, appArgs.pConfig->configs[SCREEN_WIDTH], appArgs.pConfig->configs[SCREEN_HEIGHT]);
	mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT]-2, appArgs.pConfig->configs[SCREEN_WIDTH]-8, "V %i.%i.%i", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
	refresh();
	resume_thread(thr_menu);
	
	while (GAME_STATE != QUIT) {
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_main]);
		if (GameThreads.pause_flag[thr_main]) {
			pthread_cond_wait(&GameThreads.pause_cond[thr_main], &GameThreads.thr_mutex[thr_main]);
		}
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_main]);

		switch (GAME_STATE) {
		case MAIN_MENU:
			if (GAME_MODE == SINGLE_PLAYER) {
				restart_snake(appArgs.pSnake1);
				appArgs.pSnake1->dir = MOVE_RIGHT;
			}
			else if (GAME_MODE == MULTIPLAYER) {
				restart_snake(appArgs.pSnake1);
				appArgs.pSnake1->dir = MOVE_RIGHT;
				restart_snake(appArgs.pSnake2);
				appArgs.pSnake2->dir = MOVE_RIGHT;
			}
			pthread_cond_signal(&GameThreads.pause_cond[thr_menu]);
			break;
		case STARTED:
			pthread_cond_signal(&GameThreads.pause_cond[thr_menu]);
			break;
		case CRITICAL_ERROR:
			longjmp(jmp_buffer10, 1);
			break;
		}
			pause_thread(thr_main);
	}
	destroy_appData();
	destroy_screen();
	return EXIT_SUCCESS;
}
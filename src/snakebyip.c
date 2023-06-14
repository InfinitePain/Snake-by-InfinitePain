/*
 ============================================================================
 Name        : snakebyip.c
 Author      : InfinitePain
 Version     : 1.3.1
 License     : Apache License 2.0
 Description : Snake game made by InfinitePain
 ============================================================================
 */

#include "CMakeProjectConfig.h"
#include <stdlib.h>
#include <unistd.h>
#define NCURSES_STATIC
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
#include "food.h"
#include "collision.h"
#include "direction_buffer.h"
#include "app_status.h"

int main(void) {
	init_screen();
	init_appData();
	
	while (GAME_STATE != QUIT && GAME_STATE != CRITICAL_ERROR) {
		wmanual_box(stdscr, 0, 0, appArgs.pConfig->configs[SCREEN_WIDTH], appArgs.pConfig->configs[SCREEN_HEIGHT]);
		mvwprintw(stdscr, appArgs.pConfig->configs[SCREEN_HEIGHT] - 2, appArgs.pConfig->configs[SCREEN_WIDTH] - 8, "V %i.%i.%i", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
		refresh();
		resume_thread(thr_menu);
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_main]);
		if (GameThreads.pause_flag[thr_main]) {
			increment_waiting_thread_count();
			pthread_cond_wait(&GameThreads.pause_cond[thr_main], &GameThreads.thr_mutex[thr_main]);
			decrement_waiting_thread_count();
		}
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_main]);

		switch (GAME_STATE) {
		case NOT_STARTED:
			if (GAME_MODE == SINGLE_PLAYER) {
				restart_snake(appArgs.pSnake1);
				buffer_clear(&appArgs.pSnake1->dir_buffer);
				buffer_push(&appArgs.pSnake1->dir_buffer, MOVE_RIGHT);
				game_timer_needs_reset = true;

			}
			else if (GAME_MODE == MULTIPLAYER) {
				restart_snake(appArgs.pSnake1);
				buffer_clear(&appArgs.pSnake1->dir_buffer);
				buffer_push(&appArgs.pSnake1->dir_buffer, MOVE_RIGHT);
				set_snake_position(appArgs.pSnake1, 3, (getmaxy(appWindows[GAME_WIN]) / 2) - 1);
				restart_snake(appArgs.pSnake2);
				buffer_clear(&appArgs.pSnake2->dir_buffer);
				buffer_push(&appArgs.pSnake2->dir_buffer, MOVE_LEFT);
				game_timer_needs_reset = true;
			}
			pthread_cond_signal(&GameThreads.pause_cond[thr_menu]);
			break;
		case STARTED:
			pthread_cond_signal(&GameThreads.pause_cond[thr_menu]);
			break;
		case SETTINGS:
			resize_foods(appArgs.pConfig->configs[FOOD_AMOUNT_SINGLE_PLAYER], appArgs.pConfig->configs[FOOD_AMOUNT_MULTIPLAYER]);
			pthread_cond_signal(&GameThreads.pause_cond[thr_menu]);
			break;
		}
		pause_thread(thr_main);
	}
	if (GAME_STATE == CRITICAL_ERROR) {
		destroy_appData();
		mvprintw(error_line, 0, "ERROR: A critical error occurred. Press any key to close this window . . .");
		refresh();
		getchar();
		destroy_screen();
		return EXIT_FAILURE;
	}
	destroy_appData();
	destroy_screen();
	return EXIT_SUCCESS;
}
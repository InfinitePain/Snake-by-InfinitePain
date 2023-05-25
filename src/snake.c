/*
 ============================================================================
 Name        : snake.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include "snake.h"
#include <stdlib.h>
#include "error_message.h"
#include "appdata.h"
#include "thread.h"
#include "terminal.h"
#include "timing_utils.h"
#include "app_status.h"

void delete_snake(Snake* pSnake) {
	if (pSnake == NULL) {
		return;
	}
	delete_list(pSnake->pos_snake);
	free(pSnake);
	pSnake = NULL;
}

void set_snake_position(Snake* pSnake, int x, int y) {
	if (GAME_STATE == CRITICAL_ERROR) {
		return;
	}
	pSnake->pos_snake->head->pos.posx = x;
	pSnake->pos_snake->head->pos.posy = y;
}

Snake* create_snake(int length, int color, int direction, int startx, int starty) {
	if (GAME_STATE == CRITICAL_ERROR) {
		return NULL;
	}
	Snake* pSnake = (Snake*)malloc(sizeof(Snake));
	if (pSnake == NULL)
	{
		error_message("ERROR: func create_snake(): malloc");
		GAME_STATE = CRITICAL_ERROR;
		return NULL;
	}
	pSnake->pos_snake = create_list();
	if (pSnake->pos_snake == NULL)
	{
		error_message("ERROR: func create_snake(): create_list() failed");
		GAME_STATE = CRITICAL_ERROR;
		return NULL;
	}
	pSnake->is_alive = true;
	pSnake->point = 0;
	pSnake->grow = length;
	buffer_init(&pSnake->dir_buffer);
	buffer_init(&pSnake->dir_buffer);
	add_element_to_head(pSnake->pos_snake, create_element(-1, -1));
	if (pSnake->pos_snake->head == NULL) {
		error_message("ERROR: func create_snake(): create_element() failed");
		GAME_STATE = CRITICAL_ERROR;
		return NULL;
	}
	buffer_push(&pSnake->dir_buffer, direction);
	set_snake_color(pSnake, color);
	set_snake_position(pSnake, startx, starty);
	return pSnake;
}

void set_snake_color(Snake* pSnake, int color) {
	if (GAME_STATE == CRITICAL_ERROR) {
		return;
	}
	pSnake->color = color;
}

void restart_snake(Snake* pSnake) {
	delete_list(pSnake->pos_snake);
	pSnake->pos_snake = create_list();
	if (pSnake->pos_snake == NULL)
	{
		error_message("ERROR: func create_snake(): create_list() failed");
		GAME_STATE = CRITICAL_ERROR;
		return;
	}
	pSnake->is_alive = true;
	pSnake->point = 0;
	pSnake->grow = appArgs.pConfig->configs[SNAKE_LENGTH];
	add_element_to_head(pSnake->pos_snake, create_element(-1, -1));
	if (pSnake->pos_snake->head == NULL) {
		error_message("ERROR: func restart_snake(): create_element() failed");
		GAME_STATE = CRITICAL_ERROR;
		return;
	}
}

void move_snake(const Config* pConfig, int direction, Snake* pSnake) {
	if (GAME_STATE == CRITICAL_ERROR) {
		return;
	}
	int posx = pSnake->pos_snake->head->pos.posx;
	int posy = pSnake->pos_snake->head->pos.posy;
	pthread_mutex_lock(&GameThreads.thr_mutex[thr_food]);
	Element* curr;
	switch (direction)
	{
	case MOVE_UP:
		curr = create_element(posx, posy - 1);
		break;
	case MOVE_LEFT:
		curr = create_element(posx - 1, posy);
		break;
	case MOVE_RIGHT:
		curr = create_element(posx + 1, posy);
		break;
	case MOVE_DOWN:
		curr = create_element(posx, posy + 1);
		break;
	}

	if (GAME_STATE == CRITICAL_ERROR) {
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_food]);
		error_message("ERROR: func move_snake(): create_element() failed");
		return;
	}
	add_element_to_head(pSnake->pos_snake, curr);
	
	if (pSnake->grow > 0) {
		pSnake->grow--;
	}
	else if (pSnake->grow == 0) {
		delete_last_element(pSnake->pos_snake);
	}
	pthread_mutex_unlock(&GameThreads.thr_mutex[thr_food]);
}

bool is_key_reverse(int key, int dir) {
	if (key == MOVE_UP && dir == MOVE_DOWN) {
		return true;
	}
	else if (key == MOVE_DOWN && dir == MOVE_UP) {
		return true;
	}
	else if (key == MOVE_LEFT && dir == MOVE_RIGHT) {
		return true;
	}
	else if (key == MOVE_RIGHT && dir == MOVE_LEFT) {
		return true;
	}
	return false;
}

void* snake_thread(void* args) {
	Snake* pSnake = (Snake*)args;
	int key, next_key;
	int thrnum = get_thrnum(pthread_self());
	double desired_sleep_time, loop_start_time, loop_end_time, time_spent, time_to_sleep;
	while (GameThreads.is_thr_init[thrnum]) {
		desired_sleep_time = appArgs.pConfig->configs[GAME_SPEED] * 0.025;
		loop_start_time = get_current_time_in_seconds();
		pthread_mutex_lock(&GameThreads.thr_mutex[thrnum]);
		while (GameThreads.pause_flag[thrnum]) {
			increment_waiting_thread_count();
			pthread_cond_wait(&GameThreads.pause_cond[thrnum], &GameThreads.thr_mutex[thrnum]);
			loop_start_time = get_current_time_in_seconds();
			if (!pSnake->is_alive) {
				GameThreads.pause_flag[thrnum] = true;
				pthread_mutex_unlock(&GameThreads.thr_mutex[thrnum]);
				break;
			}
			decrement_waiting_thread_count();
			if (buffer_peek(&pSnake->dir_buffer) != -1) {
				key = buffer_pop(&pSnake->dir_buffer);
			}
		}
		pthread_mutex_unlock(&GameThreads.thr_mutex[thrnum]);
		if (GAME_STATE == QUIT) {
			break;
		}

		
		if (buffer_peek(&pSnake->dir_buffer) != -1) {
			next_key = buffer_pop(&pSnake->dir_buffer);
			if (!is_key_reverse(next_key, key)) {
				key = next_key;
			}
		}
		pthread_mutex_lock(&GameThreads.thr_mutex[mutex_win_game]);
		list_printer(pSnake->pos_snake, appArgs.pConfig->configs[BACKGROUND_COLOR], 0, appWindows[GAME_WIN]);
		pthread_mutex_unlock(&GameThreads.thr_mutex[mutex_win_game]);
		move_snake(appArgs.pConfig, key, pSnake);
		pthread_mutex_lock(&GameThreads.thr_mutex[mutex_win_game]);
		list_printer(pSnake->pos_snake, pSnake->color, 0, appWindows[GAME_WIN]);
		pthread_mutex_unlock(&GameThreads.thr_mutex[mutex_win_game]);
		loop_end_time = get_current_time_in_seconds();
		time_spent = loop_end_time - loop_start_time;
		time_to_sleep = desired_sleep_time - time_spent;
		if (time_to_sleep > 0) {
			usleep(time_to_sleep * 1000000);
		}
		
		if (GAME_STATE == CRITICAL_ERROR) {
			pause_thread(thr_snake1);
			pause_thread(thr_snake2);
			pause_thread(thr_input1);
			pause_thread(thr_input2);
			pause_thread(thr_menu);
			resume_thread(thr_main);
		}
	}
	pthread_exit(NULL);
}
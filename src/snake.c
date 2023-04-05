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

extern jmp_buf jmp_buffer10;

void delete_snake(Snake* pSnake) {
	if (pSnake == NULL) {
		return;
	}
	delete_list(pSnake->pos_snake);
	free(pSnake);
	pSnake = NULL;
}

void set_snake_position(Snake* pSnake, int x, int y) {
	pSnake->pos_snake->head->pos.posx = x;
	pSnake->pos_snake->head->pos.posy = y;
}

Snake* create_snake() {
	Snake* pSnake = (Snake*)malloc(sizeof(Snake));
	if (pSnake == NULL)
	{
		error_message("ERROR: func create_snake(): malloc");
		longjmp(jmp_buffer10, 1);
	}
	pSnake->pos_snake = create_list();
	if (pSnake->pos_snake == NULL)
	{
		error_message("ERROR: func create_snake(): create_list() failed");
		longjmp(jmp_buffer10, 1);
	}
	pSnake->is_alive = true;
	pSnake->point = 0;
	pSnake->grow = appArgs.pConfig->configs[SNAKE_LENGTH];
	add_element_to_head(pSnake->pos_snake, create_element(-1, -1, 0));
	if (pSnake->pos_snake->head == NULL) {
		error_message("ERROR: func create_snake(): create_element() failed");
		longjmp(jmp_buffer10, 1);
	}
	return pSnake;
}

Element* create_element_snake(int x, int y, jmp_buf jmp_buffer) {
	Element* pElement = create_element(x, y, 0);
	if (pElement == NULL) {
		error_message("ERROR: func create_element_snake(): create_element() failed");
		longjmp(jmp_buffer, 1);
	}
	return pElement;
}

void restart_snake(Snake* pSnake) {
	delete_list(pSnake->pos_snake);
	pSnake->pos_snake = create_list();
		if (pSnake->pos_snake == NULL)
	{
		error_message("ERROR: func create_snake(): create_list() failed");
		longjmp(jmp_buffer10, 1);
	}
	pSnake->is_alive = true;
	pSnake->point = 0;
	pSnake->grow = appArgs.pConfig->configs[SNAKE_LENGTH];
	add_element_to_head(pSnake->pos_snake, create_element(-1, -1, 0));
	if (pSnake->pos_snake->head == NULL) {
		error_message("ERROR: func restart_snake(): create_element() failed");
		longjmp(jmp_buffer10, 1);
	}
}

void move_snake(const Config* pConfig, int direction, Snake* pSnake, jmp_buf jmp_buffer9) {
	jmp_buf jmp_buffer8;
	int posx = pSnake->pos_snake->head->pos.posx;
	int posy = pSnake->pos_snake->head->pos.posy;
	pthread_mutex_lock(&GameThreads.thr_mutex[thr_food]);
	if (setjmp(jmp_buffer8) != 1) {
		switch (direction)
		{
		case MOVE_UP:
			add_element_to_head(pSnake->pos_snake, create_element_snake(posx, posy - 1, jmp_buffer8));
			break;
		case MOVE_LEFT:
			add_element_to_head(pSnake->pos_snake, create_element_snake(posx - 1, posy, jmp_buffer8));
			break;
		case MOVE_RIGHT:
			add_element_to_head(pSnake->pos_snake, create_element_snake(posx + 1, posy, jmp_buffer8));
			break;
		case MOVE_DOWN:
			add_element_to_head(pSnake->pos_snake, create_element_snake(posx, posy + 1, jmp_buffer8));
			break;
		}
	}
	else {
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_food]);
		error_message("ERROR: func move_snake(): create_element() failed");
		longjmp(jmp_buffer9, 1);
	}
	
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
	jmp_buf jmp_buffer9;
	int key = pSnake->dir;
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
			key = pSnake->dir;
			decrement_waiting_thread_count();
		}
		pthread_mutex_unlock(&GameThreads.thr_mutex[thrnum]);
		if (GAME_STATE == QUIT) {
			break;
		}

		if (setjmp(jmp_buffer9) != 1) {
			if (pSnake->dir != key && !is_key_reverse(key, pSnake->dir)) {
				key = pSnake->dir;
			}
			pthread_mutex_lock(&GameThreads.thr_mutex[mutex_win_game]);
			list_printer(pSnake->pos_snake, appArgs.pConfig->configs[BACKGROUND_COLOR], 0, appWindows[GAME_WIN]);
			pthread_mutex_unlock(&GameThreads.thr_mutex[mutex_win_game]);
			move_snake(appArgs.pConfig, key, pSnake, jmp_buffer9);
			pthread_mutex_lock(&GameThreads.thr_mutex[mutex_win_game]);
			list_printer(pSnake->pos_snake, *pSnake->color, 0, appWindows[GAME_WIN]);
			pthread_mutex_unlock(&GameThreads.thr_mutex[mutex_win_game]);
			loop_end_time = get_current_time_in_seconds();
			time_spent = loop_end_time - loop_start_time;
			time_to_sleep = desired_sleep_time - time_spent;
			if (time_to_sleep > 0) {
				usleep(time_to_sleep * 1000000);
			}
		}
		else {
			GAME_STATE = CRITICAL_ERROR;
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
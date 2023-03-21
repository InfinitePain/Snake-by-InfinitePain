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
#include <setjmp.h>
#include "appdata.h"

jmp_buf jmp_buffer8;
jmp_buf jmp_buffer9;
extern jmp_buf jmp_buffer10;

void delete_snake(Snake* pSnake) {
	if (pSnake == NULL) {
		return;
	}
	delete_list(pSnake->pos_snake);
	pthread_mutex_destroy(&pSnake->thr_mutex);
	pthread_cond_destroy(&pSnake->pause_cond);
	free(pSnake);
	pSnake = NULL;
}

Snake* create_snake() {
	Snake* pSnake = (Snake*)malloc(sizeof(Snake));
	if (pSnake == NULL)
	{
		error_message("ERROR: Can't initialize Snake");
		longjmp(jmp_buffer10, 1);
	}
	pSnake->pos_snake = create_list();
	if (pSnake->pos_snake == NULL)
	{
		error_message("ERROR: Can't initialize Snake");
		longjmp(jmp_buffer10, 1);
	}
	pSnake->gameState = true;
	pSnake->point = 0;
	pSnake->grow = appArgs.pConfig->SNAKE_LENGTH;
	pSnake->is_thr_init = false;
	pthread_mutex_init(&pSnake->thr_mutex, NULL);
	pthread_cond_init(&pSnake->pause_cond, NULL);
	pSnake->pause_flag = true;
	add_element_to_head(pSnake->pos_snake, create_element(5, 5, 0));
	return pSnake;
}

void move_snake(const Config* pConfig, int direction, Snake* pSnake) {
	int posx = pSnake->pos_snake->head->pos.posx;
	int posy = pSnake->pos_snake->head->pos.posy;

	if (setjmp(jmp_buffer8) != 1) {
		switch (direction)
		{
		case MOVE_UP:
			add_element_to_head(pSnake->pos_snake, create_element(posx, posy - 1, 8));
			break;
		case MOVE_LEFT:
			add_element_to_head(pSnake->pos_snake, create_element(posx - 1, posy, 8));
			break;
		case MOVE_RIGHT:
			add_element_to_head(pSnake->pos_snake, create_element(posx + 1, posy, 8));
			break;
		case MOVE_DOWN:
			add_element_to_head(pSnake->pos_snake, create_element(posx, posy + 1, 8));
			break;
		}
	}
	else {
		error_message("ERROR: Can't move Snake");
		longjmp(jmp_buffer9, 1);
	}
	
	if (pSnake->grow > 0) {
		pSnake->grow--;
	}
	else if (pSnake->grow == 0) {
		delete_last_element(pSnake->pos_snake);
	}
}



void* snake_thread(void* args) {
	Snake* pSnake = (Snake*)args;
	int key = pSnake->dir;
	while (pSnake->is_thr_init) {
		pthread_mutex_lock(&pSnake->thr_mutex);
		while (pSnake->pause_flag) {
			pthread_cond_wait(&pSnake->pause_cond, &pSnake->thr_mutex);
			if (!pSnake->gameState) {
				pSnake->pause_flag = true;
			}
		}
		pthread_mutex_unlock(&pSnake->thr_mutex);
		
		if (setjmp(jmp_buffer9) != 1) {
			if (pSnake->dir != key) {
				key = pSnake->dir;
			}
			pthread_mutex_lock(&appArgs.mutex_win_game);
			list_printer(pSnake->pos_snake, appArgs.pConfig->BACKGROUND_COLOR, 0, appArgs.window_game);
			pthread_mutex_unlock(&appArgs.mutex_win_game);
			move_snake(appArgs.pConfig, key, pSnake);
			pthread_mutex_lock(&appArgs.mutex_win_game);
			list_printer(pSnake->pos_snake, pSnake->color, 0, appArgs.window_game);
			pthread_mutex_unlock(&appArgs.mutex_win_game);
			usleep(100000);
		}
		else {
			pthread_exit(NULL);
		}
	}
	pthread_exit(NULL);
}
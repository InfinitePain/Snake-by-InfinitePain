/*
 ============================================================================
 Name        : input.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
 ============================================================================
 */

#include "input.h"
#include "config.h"
#include "error_message.h"
#include <stdlib.h>
#include <setjmp.h>
#include "appdata.h"

extern jmp_buf jmp_buffer10;

void delete_input(Input* pInput) {
	if (pInput == NULL) {
		return;
	}
	if (pInput->window_input != NULL) {
		delwin(pInput->window_input);
	}

	pthread_mutex_destroy(&pInput->thr_mutex);
	pthread_mutex_destroy(&pInput->input_mutex);
	pthread_cond_destroy(&pInput->pause_cond);	
	free(pInput);
	pInput = NULL;
}

Input* create_input() {
	Input* pInput = (Input*)malloc(sizeof(Input));
	if (pInput == NULL) {
		error_message("ERROR func create_input");
		longjmp(jmp_buffer10, 1);
	}
	pInput->is_thr_init = false;
	pInput->pause_flag = true;
	pInput->thr_mutex = PTHREAD_MUTEX_INITIALIZER;
	pInput->input_mutex = PTHREAD_MUTEX_INITIALIZER;
	pInput->pause_cond = PTHREAD_COND_INITIALIZER;
	pInput->player_input = -1;
	pInput->window_input = newwin(LINES, COLS, 0, 0);
	if (pInput->window_input == NULL) {
		error_message("ERROR func create_input");
		delete_input(pInput);
		longjmp(jmp_buffer10, 1);
	}
	return pInput;
}

void reset_input(Input* pInput) {
	if (pthread_mutex_trylock(&pInput->input_mutex) == 0) {
		pInput->player_input = ERR;
		pthread_mutex_unlock(&pInput->input_mutex);	
	}
}

int read_input(const Config* pConfig, const int key) {
	if (key == pConfig->PLAYER_1_UP || key == pConfig->PLAYER_2_UP) {
		return MOVE_UP;
	}
	else if (key == pConfig->PLAYER_1_LEFT || key == pConfig->PLAYER_2_LEFT) {
		return MOVE_LEFT;
	}
	else if (key == pConfig->PLAYER_1_RIGHT || key == pConfig->PLAYER_2_RIGHT) {
		return MOVE_RIGHT;
	}
	else if (key == pConfig->PLAYER_1_DOWN || key == pConfig->PLAYER_2_DOWN) {
		return MOVE_DOWN;
	}
	return -1;
}

int differentiator(const Config* pConfig, const int key) {
	if (key == pConfig->PLAYER_1_UP || key == pConfig->PLAYER_1_LEFT || key == pConfig->PLAYER_1_RIGHT || key == pConfig->PLAYER_1_DOWN) {
		return 1;
	}
	else if (key == pConfig->PLAYER_2_UP || key == pConfig->PLAYER_2_LEFT || key == pConfig->PLAYER_2_RIGHT || key == pConfig->PLAYER_2_DOWN) {
		return 2;
	}
	return -1;
}

void* input_thread(void* args) {
	Input* pInput = (Input*)args;
	int inp, player, dir;
	noecho();
	cbreak();
	nodelay(pInput->window_input, TRUE);
	keypad(pInput->window_input, TRUE);
	timeout(100);
	while (pInput->is_thr_init) {
		pthread_mutex_lock(&pInput->thr_mutex);
		while (pInput->pause_flag){
			pthread_cond_wait(&pInput->pause_cond, &pInput->thr_mutex);
		}
		inp = wgetch(pInput->window_input);
		if (inp != ERR) {
			pthread_mutex_lock(&pInput->input_mutex);
			pInput->player_input = inp;
			switch (differentiator(appArgs.pConfig, inp)) {
			case 1:
				appArgs.pSnake1->dir = read_input(appArgs.pConfig, inp);
				break;
			case 2:
				appArgs.pSnake2->dir = read_input(appArgs.pConfig, inp);
				break;
			default:
				break;
			}
			pthread_mutex_unlock(&pInput->input_mutex);
		}
		pthread_mutex_unlock(&pInput->thr_mutex);
	}
	pthread_exit(NULL);
}

bool is_up_pressed(Input* pInput) {
	if (pInput->player_input == (int)'w' || pInput->player_input == KEY_UP) {
		return true;
	}
	return false;
}

bool is_down_pressed(Input* pInput) {
	if (pInput->player_input == (int)'s' || pInput->player_input == KEY_DOWN) {
		return true;
	}
	return false;
}
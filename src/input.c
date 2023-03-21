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
#include "thread.h"

extern jmp_buf jmp_buffer10;

void delete_input(Input* pInput) {
	if (pInput == NULL) {
		return;
	}
	if (pInput->window_input != NULL) {
		delwin(pInput->window_input);
	}

	pthread_mutex_destroy(&pInput->thr_mutex);
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
	pthread_mutex_init(&pInput->thr_mutex, NULL);
	pthread_cond_init(&pInput->pause_cond, NULL);
	pInput->window_input = newwin(LINES, COLS, 0, 0);
	if (pInput->window_input == NULL) {
		error_message("ERROR func create_input");
		delete_input(pInput);
		longjmp(jmp_buffer10, 1);
	}
	return pInput;
}

int read_input(const int key) {
	if (key == appArgs.pConfig->PLAYER_1_UP || key == appArgs.pConfig->PLAYER_2_UP) {
		return MOVE_UP;
	}
	else if (key == appArgs.pConfig->PLAYER_1_LEFT || key == appArgs.pConfig->PLAYER_2_LEFT) {
		return MOVE_LEFT;
	}
	else if (key == appArgs.pConfig->PLAYER_1_RIGHT || key == appArgs.pConfig->PLAYER_2_RIGHT) {
		return MOVE_RIGHT;
	}
	else if (key == appArgs.pConfig->PLAYER_1_DOWN || key == appArgs.pConfig->PLAYER_2_DOWN) {
		return MOVE_DOWN;
	}
	return -1;
}

int differentiator(const int key) {
	if (key == appArgs.pConfig->PLAYER_1_UP || key == appArgs.pConfig->PLAYER_1_LEFT || key == appArgs.pConfig->PLAYER_1_RIGHT || key == appArgs.pConfig->PLAYER_1_DOWN) {
		return 1;
	}
	else if (key == appArgs.pConfig->PLAYER_2_UP || key == appArgs.pConfig->PLAYER_2_LEFT || key == appArgs.pConfig->PLAYER_2_RIGHT || key == appArgs.pConfig->PLAYER_2_DOWN) {
		return 2;
	}
	return -1;
}


void input_driver(const int key, Input* pInput) {
	switch (differentiator(key))
	{
	case Player_1:
		appArgs.pSnake1->dir = read_input(key);
		break;
	case Player_2:
		appArgs.pSnake2->dir = read_input(key);
		break;
	default:
		if (key == 'q') {
			pause_thread(thr_snake1);
			pause_thread(thr_snake2);
			if (pInput == appArgs.pInput1) {
				appArgs.pInput1->pause_flag = true;
				pause_thread(thr_input2);
			}
			else {
				appArgs.pInput2->pause_flag = true;
				pause_thread(thr_input1);
			}
			resume_thread(thr_menu);
		}
		break;
	}
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
		while (appArgs.pInput1->pause_flag && appArgs.pInput2->pause_flag){
			pthread_cond_wait(&pInput->pause_cond, &pInput->thr_mutex);
		}
		inp = wgetch(pInput->window_input);
		if (inp != ERR) {
			input_driver(inp, pInput);
		}
		pthread_mutex_unlock(&pInput->thr_mutex);
	}
	pthread_exit(NULL);
}
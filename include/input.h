/*
 ============================================================================
 Name        : input.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
 ============================================================================
 */

#ifndef INPUT_H_
#define INPUT_H_
#include <stdbool.h>
#include <pthread.h>
#include "config.h"
#include <ncurses.h>

typedef struct Input {
	bool is_thr_init;
	bool pause_flag;
	pthread_mutex_t thr_mutex;
	pthread_mutex_t input_mutex;
	pthread_cond_t pause_cond;
	int player_input;
	WINDOW* window_input;
} Input;

void delete_input(Input* pInput);
Input* create_input();
void reset_input(Input* pInput);
int read_input(const Config* pConfig, const int key);
int differentiator(const Config* pConfig, const int key);
void* input_thread(void* args);
bool is_up_pressed(Input* pInput);
bool is_down_pressed(Input* pInput);

#endif /* INPUT_H_ */
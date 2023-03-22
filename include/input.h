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

#define Player_1 1
#define Player_2 2
#define Key_ESC  27

typedef struct Input {
	pthread_mutex_t thr_mutex;
	pthread_cond_t thr_cond;
	WINDOW* window_input;
} Input;

void delete_input(Input* pInput);
Input* create_input();
int read_input(const int key);
int differentiator(const int key);
void input_driver(const int key, Input* pInput);
void* input_thread(void* args);

#endif /* INPUT_H_ */
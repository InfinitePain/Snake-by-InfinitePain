/*
 ============================================================================
 Name        : thread.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef THREAD_H_
#define THREAD_H_

#include <stdbool.h>
#include <pthread.h>
#include "input.h"
#include "snake.h"

#define thr_main   5
#define mutex_win_game 6
#define mutex_win_menu 7
#define thr_input1 0
#define thr_input2 1
#define thr_menu   2
#define thr_snake1 3
#define thr_snake2 4

typedef struct Threads {
	pthread_t thr[5];
	bool is_thr_init[6];
	bool pause_flag[6];
	pthread_mutex_t thr_mutex[8];
	pthread_cond_t pause_cond[6];
} Threads;

extern Threads GameThreads;

int get_thrnum(pthread_t thr_id);
void pause_thread(int thrnum);
void pause_game();
void resume_thread(int thrnum);
void resume_game();
void destroy_thread(int thrnum);
void create_thread(int thrnum);

#endif /*THREAD_H_*/
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

#define NUM_THREADS 8

#define thr_input1	 	0 // has thr, is_thr_init, pause_flag, thr_mutex, pause_cond
#define thr_input2 		1 // has thr, is_thr_init, pause_flag, thr_mutex, pause_cond
#define thr_menu   		2 // has thr, is_thr_init, pause_flag, thr_mutex, pause_cond
#define thr_snake1 		3 // has thr, is_thr_init, pause_flag, thr_mutex, pause_cond
#define thr_snake2 		4 // has thr, is_thr_init, pause_flag, thr_mutex, pause_cond
#define thr_collision 	5 // has thr, is_thr_init, pause_flag, thr_mutex, pause_cond
#define thr_food   		6 // has thr, is_thr_init, thr_mutex, pause_cond
#define thr_main		7 // has thr, pause_flag, thr_mutex, pause_cond
#define mutex_win_game  8 // has thr_mutex
#define mutex_food		9 // has thr_mutex

typedef struct Threads {
	pthread_t thr[7];
	bool is_thr_init[7];
	bool pause_flag[8];
	pthread_mutex_t thr_mutex[11];
	pthread_cond_t pause_cond[9];
} Threads;

extern int waiting_thread_count;
extern pthread_mutex_t mutex_waiting_thread_count;
extern pthread_cond_t cond_waiting_thread_count;
extern Threads GameThreads;

int get_thrnum(pthread_t thr_id);
void pause_thread(int thrnum);
void resume_thread(int thrnum);
void destroy_thread(int thrnum);
void create_thread(int thrnum);
void increment_waiting_thread_count();
void decrement_waiting_thread_count();

#endif /*THREAD_H_*/
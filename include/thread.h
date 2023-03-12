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

void pause_thread(bool* pause_flag, pthread_mutex_t* mutex, pthread_cond_t* cond);
void resume_thread(bool* pause_flag, pthread_mutex_t* mutex, pthread_cond_t* cond);
void destroy_thread(bool* is_thr_init, pthread_t thr, bool* pause_flag, pthread_mutex_t* mutex, pthread_cond_t* cond);
void create_input_thread(pthread_t* thr, Input* pInput);
void create_snake_thread(pthread_t* thr, Snake* pSnake);

#endif /*THREAD_H_*/
/*
 ============================================================================
 Name        : thread.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include "thread.h"
#include "appdata.h"
#include "error_message.h"
#include <setjmp.h>
#include "snake.h"

extern jmp_buf jmp_buffer10;

void pause_thread(bool* pause_flag, pthread_mutex_t* mutex, pthread_cond_t* cond) {
	pthread_mutex_lock(mutex);
	*pause_flag = true;
	pthread_mutex_unlock(mutex);
}

void resume_thread(bool* pause_flag, pthread_mutex_t* mutex, pthread_cond_t* cond) {
	pthread_mutex_lock(mutex);
	*pause_flag = false;
	pthread_cond_signal(cond);
	pthread_mutex_unlock(mutex);
}

void destroy_thread(bool* is_thr_init, pthread_t thr, bool* pause_flag, pthread_mutex_t* mutex, pthread_cond_t* cond) {
	if (is_thr_init) {
		pause_thread(pause_flag, mutex, cond);
		appArgs.appState = false;
		resume_thread(pause_flag, mutex, cond);
		pthread_join(thr, NULL);
		*is_thr_init = false;
	}
}

void create_input_thread(pthread_t* thr, Input* pInput) {
	if (pthread_create(thr, NULL, &input_thread, pInput) != 0) {
		error_message("ERROR: Can't create Thread\n");
		longjmp(jmp_buffer10, 1);
	}
	pInput->is_thr_init = true;
}

void pause_input_thread(Input* pInput) {
	pthread_mutex_lock(&pInput->thr_mutex);
	pInput->pause_flag = true;
	pthread_mutex_unlock(&pInput->thr_mutex);
}

void resume_input_thread(Input* pInput) {
	pthread_mutex_lock(&pInput->thr_mutex);
	pInput->pause_flag = false;
	pthread_cond_signal(&pInput->pause_cond);
	pthread_mutex_unlock(&pInput->thr_mutex);
}

void destroy_input_thread(Input* pInput, pthread_t thr) {
	if (pInput->is_thr_init) {
		pause_input_thread(pInput);
		appArgs.appState = false;
		resume_input_thread(pInput);
		pthread_join(thr, NULL);
		pInput->is_thr_init = false;
	}
}

void create_snake_thread(pthread_t* thr, Snake* pSnake) {
	if (pthread_create(thr, NULL, &snake_thread, pSnake) != 0) {
		error_message("ERROR: Can't create Thread\n");
		longjmp(jmp_buffer10, 1);
	}
	pSnake->is_thr_init = true;
}

/*
 ============================================================================
 Name        : mymenu.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef MYMENU_H_
#define MYMENU_H_

typedef struct menuArgs {
	bool is_thr_init;
	bool pause_flag;
	pthread_mutex_t thr_mutex;
	pthread_cond_t pause_cond;
} menuArgs;

void* menu_thread(void* args);

#endif /*MYMENU_H_*/
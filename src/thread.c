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
#include <mymenu.h>
#include <appdata.h>

extern jmp_buf jmp_buffer10;

void pause_thread(int thrnum) {
	switch (thrnum) {
	case thr_main:
		pthread_mutex_lock(&appArgs.mutex_main);
		appArgs.pause_flag_main = true;
		pthread_mutex_unlock(&appArgs.mutex_main);
	case thr_input1:
		pthread_mutex_lock(&appArgs.pInput1->thr_mutex);
		appArgs.pInput1->pause_flag = true;
		pthread_mutex_unlock(&appArgs.pInput1->thr_mutex);
		break;
	case thr_input2:
		pthread_mutex_lock(&appArgs.pInput2->thr_mutex);
		appArgs.pInput2->pause_flag = true;
		pthread_mutex_unlock(&appArgs.pInput2->thr_mutex);
		break;
	case thr_menu:
		pthread_mutex_lock(&appArgs.pMenuThrArgs->thr_mutex);
		appArgs.pMenuThrArgs->pause_flag = true;
		pthread_mutex_unlock(&appArgs.pMenuThrArgs->thr_mutex);
		break;
	case thr_snake1:
		pthread_mutex_lock(&appArgs.pSnake1->thr_mutex);
		appArgs.pSnake1->pause_flag = true;
		pthread_mutex_unlock(&appArgs.pSnake1->thr_mutex);
		break;
	case thr_snake2:
		pthread_mutex_lock(&appArgs.pSnake2->thr_mutex);
		appArgs.pSnake2->pause_flag = true;
		pthread_mutex_unlock(&appArgs.pSnake2->thr_mutex);
		break;
	}
}

void resume_thread(int thrnum) {
	switch (thrnum) {
	case thr_main:
		pthread_mutex_lock(&appArgs.mutex_main);
		appArgs.pause_flag_main = false;
		pthread_cond_signal(&appArgs.cond_main);
		pthread_mutex_unlock(&appArgs.mutex_main);
		break;
	case thr_input1:
		pthread_mutex_lock(&appArgs.pInput1->thr_mutex);
		appArgs.pInput1->pause_flag = false;
		pthread_cond_signal(&appArgs.pInput1->pause_cond);
		pthread_mutex_unlock(&appArgs.pInput1->thr_mutex);
		break;
	case thr_input2:
		pthread_mutex_lock(&appArgs.pInput2->thr_mutex);
		appArgs.pInput2->pause_flag = false;
		pthread_cond_signal(&appArgs.pInput2->pause_cond);
		pthread_mutex_unlock(&appArgs.pInput2->thr_mutex);
		break;
	case thr_menu:
		pthread_mutex_lock(&appArgs.pMenuThrArgs->thr_mutex);
		appArgs.pMenuThrArgs->pause_flag = false;
		pthread_cond_signal(&appArgs.pMenuThrArgs->pause_cond);
		pthread_mutex_unlock(&appArgs.pMenuThrArgs->thr_mutex);
		break;
	case thr_snake1:
		pthread_mutex_lock(&appArgs.pSnake1->thr_mutex);
		appArgs.pSnake1->pause_flag = false;
		pthread_cond_signal(&appArgs.pSnake1->pause_cond);
		pthread_mutex_unlock(&appArgs.pSnake1->thr_mutex);
		break;
	case thr_snake2:
		pthread_mutex_lock(&appArgs.pSnake2->thr_mutex);
		appArgs.pSnake2->pause_flag = false;
		pthread_cond_signal(&appArgs.pSnake2->pause_cond);
		pthread_mutex_unlock(&appArgs.pSnake2->thr_mutex);
		break;
	}
}

void destroy_thread(int thrnum) {
	switch (thrnum) {
	case thr_input1:
		if (appArgs.pInput1->is_thr_init) {
			pause_thread(thr_input1);
			appArgs.pInput1->is_thr_init = false;
			resume_thread(thr_input1);
			pthread_join(appArgs.thr[thr_input1], NULL);
		}
		break;
	case thr_input2:
		if (appArgs.pInput2->is_thr_init) {
			pause_thread(thr_input2);
			appArgs.pInput2->is_thr_init = false;
			resume_thread(thr_input2);
			pthread_join(appArgs.thr[thr_input2], NULL);
		}
		break;
	case thr_menu:
		if (appArgs.pMenuThrArgs->is_thr_init) {
			pause_thread(thr_menu);
			appArgs.pMenuThrArgs->is_thr_init = false;
			resume_thread(thr_menu);
			pthread_join(appArgs.thr[thr_menu], NULL);
		}
		break;
	case thr_snake1:
		if (appArgs.pSnake1->is_thr_init) {
			pause_thread(thr_snake1);
			appArgs.pSnake1->is_thr_init = false;
			resume_thread(thr_snake1);
			pthread_join(appArgs.thr[thr_snake1], NULL);
		}
		break;
	case thr_snake2:
		if (appArgs.pSnake2->is_thr_init) {
			pause_thread(thr_snake2);
			appArgs.pSnake2->is_thr_init = false;
			resume_thread(thr_snake2);
			pthread_join(appArgs.thr[thr_snake2], NULL);
		}
		break;
	}
}

void create_thread(int thrnum) {
	switch (thrnum) {
	case thr_input1:
		appArgs.pInput1->is_thr_init;
		appArgs.pInput1->is_thr_init = true;
		if (pthread_create(&appArgs.thr[thr_input1], NULL, &input_thread, appArgs.pInput1) != 0) {
			error_message("ERROR: Can't create Thread\n");
			appArgs.pInput1->is_thr_init = false;
			longjmp(jmp_buffer10, 1);
		}
		break;
	case thr_input2:
		appArgs.pInput2->is_thr_init;
		appArgs.pInput2->is_thr_init = true;
		if (pthread_create(&appArgs.thr[thr_input2], NULL, &input_thread, appArgs.pInput2) != 0) {
			error_message("ERROR: Can't create Thread\n");
			appArgs.pInput2->is_thr_init = false;
			longjmp(jmp_buffer10, 1);
		}
		break;
	case thr_menu:
		appArgs.pMenuThrArgs->is_thr_init = true;
		if (pthread_create(&appArgs.thr[thr_menu], NULL, &menu_thread, appArgs.pMenuThrArgs) != 0) {
			error_message("ERROR: Can't create Thread\n");
			appArgs.pMenuThrArgs->is_thr_init = false;
			longjmp(jmp_buffer10, 1);
		}
		break;
	case thr_snake1:
		appArgs.pSnake1->is_thr_init = true;
		if (pthread_create(&appArgs.thr[thr_snake1], NULL, &snake_thread, appArgs.pSnake1) != 0) {
			error_message("ERROR: Can't create Thread\n");
			appArgs.pSnake1->is_thr_init = false;
			longjmp(jmp_buffer10, 1);
		}
		break;
	case thr_snake2:
		appArgs.pSnake2->is_thr_init = true;
		if (pthread_create(&appArgs.thr[thr_snake2], NULL, &snake_thread, appArgs.pSnake2) != 0) {
			error_message("ERROR: Can't create Thread\n");
			appArgs.pSnake2->is_thr_init = false;
			longjmp(jmp_buffer10, 1);
		}
		break;
	}
}
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

Threads GameThreads = {
	.is_thr_init = { false, false, false, false, false, false },
	.pause_flag = { true, true, true, true, true, true },
	.thr_mutex = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER},
	.pause_cond = { PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER }
};

int get_thrnum(pthread_t thr_id) {
	if (pthread_equal(thr_id, GameThreads.thr[thr_input1])) {
		return thr_input1;
	}
	else if (pthread_equal(thr_id, GameThreads.thr[thr_input2])) {
		return thr_input2;
	}
	else if (pthread_equal(thr_id, GameThreads.thr[thr_menu])) {
		return thr_menu;
	}
	else if (pthread_equal(thr_id, GameThreads.thr[thr_snake1])) {
		return thr_snake1;
	}
	else if (pthread_equal(thr_id, GameThreads.thr[thr_snake2])) {
		return thr_snake2;
	}
}

void pause_thread(int thrnum) {
	switch (thrnum) {
	case thr_main:
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_main]);
		GameThreads.pause_flag[thr_main] = true;
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_main]);
	case thr_input1:
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_input1]);
		GameThreads.pause_flag[thr_input1] = true;
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_input1]);
		break;
	case thr_input2:
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_input2]);
		GameThreads.pause_flag[thr_input2] = true;
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_input2]);
		break;
	case thr_menu:
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_menu]);
		GameThreads.pause_flag[thr_menu] = true;
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_menu]);
		break;
	case thr_snake1:
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_snake1]);
		GameThreads.pause_flag[thr_snake1] = true;
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_snake1]);
		break;
	case thr_snake2:
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_snake2]);
		GameThreads.pause_flag[thr_snake2] = true;
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_snake2]);
		break;
	}
}

void resume_thread(int thrnum) {
	switch (thrnum) {
	case thr_main:
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_main]);
		GameThreads.pause_flag[thr_main] = false;
		pthread_cond_signal(&GameThreads.pause_cond[thr_main]);
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_main]);
		break;
	case thr_input1:
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_input1]);
		GameThreads.pause_flag[thr_input1] = false;
		pthread_cond_signal(&GameThreads.pause_cond[thr_input1]);
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_input1]);
		break;
	case thr_input2:
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_input2]);
		GameThreads.pause_flag[thr_input2] = false;
		pthread_cond_signal(&GameThreads.pause_cond[thr_input2]);
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_input2]);
		break;
	case thr_menu:
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_menu]);
		GameThreads.pause_flag[thr_menu] = false;
		pthread_cond_signal(&GameThreads.pause_cond[thr_menu]);
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_menu]);
		break;
	case thr_snake1:
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_snake1]);
		GameThreads.pause_flag[thr_snake1] = false;
		pthread_cond_signal(&GameThreads.pause_cond[thr_snake1]);
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_snake1]);
		break;
	case thr_snake2:
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_snake2]);
		GameThreads.pause_flag[thr_snake2] = false;
		pthread_cond_signal(&GameThreads.pause_cond[thr_snake2]);
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_snake2]);
		break;
	}
}

void destroy_thread(int thrnum) {
	switch (thrnum) {
	case thr_input1:
		if (GameThreads.is_thr_init[thr_input1]) {
			pause_thread(thr_input1);
			GameThreads.is_thr_init[thr_input1] = false;
			resume_thread(thr_input1);
			pthread_join(GameThreads.thr[thr_input1], NULL);
			pthread_mutex_destroy(&GameThreads.thr_mutex[thr_input1]);
			pthread_cond_destroy(&GameThreads.pause_cond[thr_input1]);
		}
		break;
	case thr_input2:
		if (GameThreads.is_thr_init[thr_input2]) {
			pause_thread(thr_input2);
			GameThreads.is_thr_init[thr_input2] = false;
			resume_thread(thr_input2);
			pthread_join(GameThreads.thr[thr_input2], NULL);
			pthread_mutex_destroy(&GameThreads.thr_mutex[thr_input2]);
			pthread_cond_destroy(&GameThreads.pause_cond[thr_input2]);
		}
		break;
	case thr_menu:
		if (GameThreads.is_thr_init[thr_menu]) {
			pause_thread(thr_menu);
			GameThreads.is_thr_init[thr_menu] = false;
			resume_thread(thr_menu);
			pthread_join(GameThreads.thr[thr_menu], NULL);
			pthread_mutex_destroy(&GameThreads.thr_mutex[thr_menu]);
			pthread_cond_destroy(&GameThreads.pause_cond[thr_menu]);
		}
		break;
	case thr_snake1:
		if (GameThreads.is_thr_init[thr_snake1]) {
			pause_thread(thr_snake1);
			GameThreads.is_thr_init[thr_snake1] = false;
			resume_thread(thr_snake1);
			pthread_join(GameThreads.thr[thr_snake1], NULL);
			pthread_mutex_destroy(&GameThreads.thr_mutex[thr_snake1]);
			pthread_cond_destroy(&GameThreads.pause_cond[thr_snake1]);
		}
		break;
	case thr_snake2:
		if (GameThreads.is_thr_init[thr_snake2]) {
			pause_thread(thr_snake2);
			GameThreads.is_thr_init[thr_snake2] = false;
			resume_thread(thr_snake2);
			pthread_join(GameThreads.thr[thr_snake2], NULL);
			pthread_mutex_destroy(&GameThreads.thr_mutex[thr_snake2]);
			pthread_cond_destroy(&GameThreads.pause_cond[thr_snake2]);
		}
		break;
	case thr_main:
		if (GameThreads.is_thr_init[thr_main]) {
			GameThreads.is_thr_init[thr_main] = false;
			pthread_mutex_destroy(&GameThreads.thr_mutex[thr_main]);
			pthread_cond_destroy(&GameThreads.pause_cond[thr_main]);
		}
		break;
	}
}

void create_thread(int thrnum) {
	switch (thrnum) {
	case thr_input1:
		GameThreads.is_thr_init[thr_input1] = true;
		if (pthread_create(&GameThreads.thr[thr_input1], NULL, &input_thread, appArgs.pInput1) != 0) {
			error_message("ERROR: Can't create Thread\n");
			GameThreads.is_thr_init[thr_input1] = false;
			longjmp(jmp_buffer10, 1);
		}
		break;
	case thr_input2:
		GameThreads.is_thr_init[thr_input2] = true;
		if (pthread_create(&GameThreads.thr[thr_input2], NULL, &input_thread, appArgs.pInput2) != 0) {
			error_message("ERROR: Can't create Thread\n");
			GameThreads.is_thr_init[thr_input2] = false;
			longjmp(jmp_buffer10, 1);
		}
		break;
	case thr_menu:
		GameThreads.is_thr_init[thr_menu] = true;
		if (pthread_create(&GameThreads.thr[thr_menu], NULL, &menu_thread, appArgs.pMenuThrArgs) != 0) {
			error_message("ERROR: Can't create Thread\n");
			GameThreads.is_thr_init[thr_menu] = false;
			longjmp(jmp_buffer10, 1);
		}
		break;
	case thr_snake1:
		GameThreads.is_thr_init[thr_snake1] = true;
		if (pthread_create(&GameThreads.thr[thr_snake1], NULL, &snake_thread, appArgs.pSnake1) != 0) {
			error_message("ERROR: Can't create Thread\n");
			GameThreads.is_thr_init[thr_snake1] = false;
			longjmp(jmp_buffer10, 1);
		}
		break;
	case thr_snake2:
		GameThreads.is_thr_init[thr_snake2] = true;
		if (pthread_create(&GameThreads.thr[thr_snake2], NULL, &snake_thread, appArgs.pSnake2) != 0) {
			error_message("ERROR: Can't create Thread\n");
			GameThreads.is_thr_init[thr_snake2] = false;
			longjmp(jmp_buffer10, 1);
		}
		break;
	}
}
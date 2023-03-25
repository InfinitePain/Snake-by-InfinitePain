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
	pthread_mutex_lock(&GameThreads.thr_mutex[thrnum]);
	GameThreads.pause_flag[thrnum] = true;
	pthread_mutex_unlock(&GameThreads.thr_mutex[thrnum]);
}

void resume_thread(int thrnum) {
	pthread_mutex_lock(&GameThreads.thr_mutex[thrnum]);
	GameThreads.pause_flag[thrnum] = false;
	pthread_cond_signal(&GameThreads.pause_cond[thrnum]);
	pthread_mutex_unlock(&GameThreads.thr_mutex[thrnum]);
}

void destroy_thread(int thrnum) {
	if (GameThreads.is_thr_init[thrnum] && thrnum < 5) {
		pause_thread(thrnum);
		GameThreads.is_thr_init[thrnum] = false;
		resume_thread(thrnum);
		pthread_join(GameThreads.thr[thrnum], NULL);
		pthread_mutex_destroy(&GameThreads.thr_mutex[thrnum]);
		pthread_cond_destroy(&GameThreads.pause_cond[thrnum]);
	}
	else if (thrnum == thr_main) {
		GameThreads.is_thr_init[thr_main] = false;
		pthread_mutex_destroy(&GameThreads.thr_mutex[thr_main]);
		pthread_cond_destroy(&GameThreads.pause_cond[thr_main]);
		pthread_mutex_destroy(&GameThreads.thr_mutex[mutex_win_game]);
		pthread_mutex_destroy(&GameThreads.thr_mutex[mutex_win_menu]);
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
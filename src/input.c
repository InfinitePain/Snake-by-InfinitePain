/*
 ============================================================================
 Name        : input.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
 ============================================================================
 */

#include "input.h"
#include "config.h"
#include "error_message.h"
#include <stdlib.h>
#include <setjmp.h>
#include "appdata.h"
#include "thread.h"
#include "terminal.h"

extern jmp_buf jmp_buffer10;


int read_input(const int key) {
	if (key == appArgs.pConfig->configs[PLAYER_1_UP] || key == appArgs.pConfig->configs[PLAYER_2_UP]) {
		return MOVE_UP;
	}
	else if (key == appArgs.pConfig->configs[PLAYER_1_LEFT] || key == appArgs.pConfig->configs[PLAYER_2_LEFT]) {
		return MOVE_LEFT;
	}
	else if (key == appArgs.pConfig->configs[PLAYER_1_RIGHT] || key == appArgs.pConfig->configs[PLAYER_2_RIGHT]) {
		return MOVE_RIGHT;
	}
	else if (key == appArgs.pConfig->configs[PLAYER_1_DOWN] || key == appArgs.pConfig->configs[PLAYER_2_DOWN]) {
		return MOVE_DOWN;
	}
	return -1;
}

int differentiator(const int key) {
	if (key == appArgs.pConfig->configs[PLAYER_1_UP] || key == appArgs.pConfig->configs[PLAYER_1_LEFT] || key == appArgs.pConfig->configs[PLAYER_1_RIGHT] || key == appArgs.pConfig->configs[PLAYER_1_DOWN]) {
		return 1;
	}
	else if (key == appArgs.pConfig->configs[PLAYER_2_UP] || key == appArgs.pConfig->configs[PLAYER_2_LEFT] || key == appArgs.pConfig->configs[PLAYER_2_RIGHT] || key == appArgs.pConfig->configs[PLAYER_2_DOWN]) {
		return 2;
	}
	return -1;
}


void input_driver(const int key) {
	switch (differentiator(key))
	{
	case Player_1:
		buffer_push(&appArgs.pSnake1->dir_buffer, read_input(key));
		break;
	case Player_2:
		buffer_push(&appArgs.pSnake2->dir_buffer, read_input(key));
		break;
	default:
		if (key == 27 || key == 34 || key == 94) {
			pause_thread(thr_snake1);
			pause_thread(thr_snake2);
			pause_thread(thr_input1);
			pause_thread(thr_input2);
			pause_thread(thr_collision);
			resume_thread(thr_menu);
		}
		break;
	}
}

void* input_thread(void* args) {
	int inp, player, dir;
	int thrnum = get_thrnum(pthread_self());
	noecho();
	cbreak();
	nodelay(appWindows[thrnum], TRUE);
	keypad(appWindows[thrnum], TRUE);
	timeout(100);
	while (GameThreads.is_thr_init[thrnum]) {
		pthread_mutex_lock(&GameThreads.thr_mutex[thrnum]);
		while (GameThreads.pause_flag[thrnum]) {
			increment_waiting_thread_count();
			pthread_cond_wait(&GameThreads.pause_cond[thrnum], &GameThreads.thr_mutex[thrnum]);
			decrement_waiting_thread_count();
		}
		pthread_mutex_unlock(&GameThreads.thr_mutex[thrnum]);
		if (GAME_STATE == QUIT) {
			break;
		}
		inp = wgetch(appWindows[thrnum]);
		if (inp != ERR) {
			input_driver(inp);
		}
	}
	pthread_exit(NULL);
}
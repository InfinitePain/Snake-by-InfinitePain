/*
 ============================================================================
 Name        : food.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include "food.h"
#include <stdlib.h>
#include <ncurses.h>
#include <setjmp.h>
#include "terminal.h"
#include "appdata.h"
#include "error_message.h"

extern jmp_buf jmp_buffer10;
jmp_buf jmp_buffer12;

void delete_foods() {
	delete_list(appArgs.pFood_Main);
	free(appArgs.pFood_Multiplayer);
	free(appArgs.pFood_Single_Player);
}

void random_coordinate_generator(int* x, int* y) {
	*x = (rand() % (getmaxx(appWindows[GAME_WIN]) - 2)) + 1;
	*y = (rand() % (getmaxy(appWindows[GAME_WIN]) - 2)) + 1;
}

bool food_coordinate_checker(List* pList, int x, int y) {
	if (pList == NULL) {
		return false;
	}
	Element* curr = pList->head;
	while (curr != NULL) {
		if (x == curr->pos.posx || y == curr->pos.posy)
		{
			return true;
		}
		curr = curr->next;
	}

	return false;
}

Element* food_adresser(List* pList) {
	Element* curr = pList->head;
	while (curr != NULL) {
		if (curr->pos.posx == -1 || curr->pos.posy == -1)
		{
			return curr;
		}
		curr = curr->next;
	}

	return NULL;
}

void random_food_generator(List* pFood, Snake* pSnake1, Snake* pSnake2) {
	int x, y;
	Element* curr = food_adresser(pFood);
	while (curr != NULL) {
		random_coordinate_generator(&x, &y);
		while (food_coordinate_checker(pFood, x, y) || food_coordinate_checker(pSnake1->pos_snake, x, y) || (pSnake2 != NULL && food_coordinate_checker(pSnake2->pos_snake, x, y))) {
			random_coordinate_generator(&x, &y);
		}
		curr->pos.posx = x;
		curr->pos.posy = y;
		curr = food_adresser(pFood);
	}
}

void reset_food(List* pFood) {
	Element* curr = pFood->head;
	while (curr != NULL) {
		curr->pos.posx = -1;
		curr->pos.posy = -1;
		curr = curr->next;
	}
}

void resize_foods() {
    int max_food = appArgs.pConfig->configs[FOOD_AMOUNT_MULTIPLAYER];
    if (appArgs.pConfig->configs[FOOD_AMOUNT_SINGLE_PLAYER] > max_food) {
        max_food = appArgs.pConfig->configs[FOOD_AMOUNT_SINGLE_PLAYER];
    }

    while (max_food != appArgs.pFood_Main->size) {
        if (max_food > appArgs.pFood_Main->size) {
            add_element_to_head(appArgs.pFood_Main, create_element(-1, -1, 12));
		}
		else {
			delete_last_element(appArgs.pFood_Main);
        }
	}
	
	make_list_from(appArgs.pFood_Main, appArgs.pFood_Multiplayer, appArgs.pConfig->configs[FOOD_AMOUNT_MULTIPLAYER]);
    make_list_from(appArgs.pFood_Main, appArgs.pFood_Single_Player, appArgs.pConfig->configs[FOOD_AMOUNT_SINGLE_PLAYER]);
}


void init_foods() {
	appArgs.pFood_Main = create_list();
	appArgs.pFood_Multiplayer = create_list();
	appArgs.pFood_Single_Player = create_list();
	if (appArgs.pFood_Main == NULL || appArgs.pFood_Multiplayer == NULL || appArgs.pFood_Single_Player == NULL) {
		longjmp(jmp_buffer10, 1);
	}
	srand(time(NULL));

	int max_food = appArgs.pConfig->configs[FOOD_AMOUNT_MULTIPLAYER];
	if (appArgs.pConfig->configs[FOOD_AMOUNT_SINGLE_PLAYER] > max_food) {
		max_food = appArgs.pConfig->configs[FOOD_AMOUNT_SINGLE_PLAYER];
	}

	if (setjmp(jmp_buffer12) != 1) {
		while (max_food != appArgs.pFood_Main->size) {
			add_element_to_head(appArgs.pFood_Main, create_element(-1, -1, 12));
		}
	}
	else {
		error_message("ERROR: func food_init(): create_element() failed");
		longjmp(jmp_buffer10, 1);
	}
	make_list_from(appArgs.pFood_Main, appArgs.pFood_Multiplayer, appArgs.pConfig->configs[FOOD_AMOUNT_MULTIPLAYER]);
	make_list_from(appArgs.pFood_Main, appArgs.pFood_Single_Player, appArgs.pConfig->configs[FOOD_AMOUNT_SINGLE_PLAYER]);
}

void* food_thread(void* args) {
	while (GameThreads.is_thr_init[thr_food]) {
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_food]);
		increment_waiting_thread_count();
		pthread_cond_wait(&GameThreads.pause_cond[thr_food], &GameThreads.thr_mutex[thr_food]);
		decrement_waiting_thread_count();
		if (GAME_STATE == QUIT) {
			pthread_mutex_unlock(&GameThreads.thr_mutex[thr_food]);
			break;
		}

		switch (GAME_MODE) {
		case SINGLE_PLAYER:
			random_food_generator(appArgs.pFood_Single_Player, appArgs.pSnake1, NULL);
			pthread_mutex_lock(&GameThreads.thr_mutex[mutex_win_game]);
			list_printer(appArgs.pFood_Single_Player, appArgs.pConfig->configs[FOOD_COLOR], 0, appWindows[GAME_WIN]);
			pthread_mutex_unlock(&GameThreads.thr_mutex[mutex_win_game]);
			break;
		case MULTIPLAYER:
			random_food_generator(appArgs.pFood_Multiplayer, appArgs.pSnake1, appArgs.pSnake2);
			pthread_mutex_lock(&GameThreads.thr_mutex[mutex_win_game]);
			list_printer(appArgs.pFood_Multiplayer, appArgs.pConfig->configs[FOOD_COLOR], 0, appWindows[GAME_WIN]);
			pthread_mutex_unlock(&GameThreads.thr_mutex[mutex_win_game]);
			break;
		}
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_food]);
	}
	pthread_exit(NULL);
}
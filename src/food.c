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
#define NCURSES_STATIC
#include <ncurses.h>
#include <setjmp.h>
#include "terminal.h"
#include "appdata.h"
#include "error_message.h"
#include "app_status.h"

void delete_available_positions(AvailablePositions* ap) {
	if (ap == NULL) {
		return;
	}
	free(ap->positions);
	free(ap);
}

AvailablePositions* create_available_positions(int width, int height) {
	if (GAME_STATE == CRITICAL_ERROR) {
		return NULL;
	}
	AvailablePositions* ap = (AvailablePositions*)malloc(sizeof(AvailablePositions));
	if (ap == NULL) {
		error_message("ERROR: func create_available_positions(): malloc");
		GAME_STATE = CRITICAL_ERROR;
		return NULL;
	}
	int max_positions = (width - 2) * (height - 2);
	ap->positions = (Position*)malloc(max_positions * sizeof(Position));
	if (ap->positions == NULL) {
		error_message("ERROR: func create_available_positions(): malloc");
		GAME_STATE = CRITICAL_ERROR;
		return NULL;
	}
	ap->count = 0;

	for (int x = 1; x < width - 1; x++) {
		for (int y = 1; y < height - 1; y++) {
			Position pos = { x, y };
			ap->positions[ap->count++] = pos;
		}
	}
	return ap;
}

void remove_position(AvailablePositions* ap, int x, int y) {
	if (ap == NULL || GAME_STATE == CRITICAL_ERROR) {
		return;
	}
	for (int i = 0; i < ap->count; i++) {
		if (ap->positions[i].posx == x && ap->positions[i].posy == y) {
			for (int j = i; j < ap->count - 1; j++) {
				ap->positions[j] = ap->positions[j + 1];
			}
			ap->count--;
			return;
		}
	}
}

void add_position(AvailablePositions* ap, int x, int y) {
	if (ap == NULL || GAME_STATE == CRITICAL_ERROR) {
		return;
	}
	Position pos = { x, y };
	ap->positions[ap->count++] = pos;
}

void delete_foods() {
	delete_list(appArgs.pFood_Main);
	free(appArgs.pFood_Multiplayer);
	free(appArgs.pFood_Single_Player);
}

void get_random_position(AvailablePositions* ap, int* x, int* y) {
	if (ap == NULL || GAME_STATE == CRITICAL_ERROR) {
		return;
	}
	int index = rand() % ap->count;
	*x = ap->positions[index].posx;
	*y = ap->positions[index].posy;
}

Element* food_adresser(List* pList) {
	if (pList == NULL || GAME_STATE == CRITICAL_ERROR) {
		return NULL;
	}
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
	if (pFood == NULL || pSnake1 == NULL || pSnake2 == NULL || GAME_STATE == CRITICAL_ERROR) {
		return;
	}
	int x, y;
	Element* curr = food_adresser(pFood);
	if (curr == NULL) {
		return;
	}

	Element* currSnake = pSnake1->pos_snake->head;
	while (currSnake != NULL) {
		remove_position(appArgs.pAvailablePositions, currSnake->pos.posx, currSnake->pos.posy);
		currSnake = currSnake->next;
	}
	if (GAME_MODE == MULTIPLAYER) {
		currSnake = pSnake2->pos_snake->head;
		while (currSnake != NULL) {
			remove_position(appArgs.pAvailablePositions, currSnake->pos.posx, currSnake->pos.posy);
			currSnake = currSnake->next;
		}
	}

	while (curr != NULL) {
		if (appArgs.pAvailablePositions->count <= 0) {
			break;
		}
		get_random_position(appArgs.pAvailablePositions, &x, &y);
		remove_position(appArgs.pAvailablePositions, x, y);
		curr->pos.posx = x;
		curr->pos.posy = y;
		curr = food_adresser(pFood);
	}

	currSnake = pSnake1->pos_snake->head;
	while (currSnake != NULL) {
		add_position(appArgs.pAvailablePositions, currSnake->pos.posx, currSnake->pos.posy);
		currSnake = currSnake->next;
	}
	if (GAME_MODE == MULTIPLAYER) {
		currSnake = pSnake2->pos_snake->head;
		while (currSnake != NULL) {
			add_position(appArgs.pAvailablePositions, currSnake->pos.posx, currSnake->pos.posy);
			currSnake = currSnake->next;
		}
	}
}

void reset_food(List* pFood) {
	if (pFood == NULL || GAME_STATE == CRITICAL_ERROR) {
		return;
	}
	Element* curr = pFood->head;
	while (curr != NULL) {
		curr->pos.posx = -1;
		curr->pos.posy = -1;
		curr = curr->next;
	}
}

void reset_available_positions(AvailablePositions* ap, int width, int height) {
    if (ap == NULL || GAME_STATE == CRITICAL_ERROR) {
        return;
    }
    ap->count = 0;

    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            Position pos = { x, y };
            ap->positions[ap->count++] = pos;
        }
    }
}


void resize_foods(int food_amount_single_player, int food_amount_multiplayer) {
	if (GAME_STATE == CRITICAL_ERROR) {
		return;
	}
	int max_food = food_amount_multiplayer;
	if (food_amount_single_player > max_food) {
		max_food = food_amount_single_player;
	}

	while (max_food != appArgs.pFood_Main->size) {
		if (max_food > appArgs.pFood_Main->size) {
			Element* curr = create_element(-1, -1);
			add_element_to_head(appArgs.pFood_Main, curr);
		}
		else {
			delete_last_element(appArgs.pFood_Main);
		}
	}

	make_list_from(appArgs.pFood_Main, appArgs.pFood_Multiplayer, food_amount_multiplayer);
	make_list_from(appArgs.pFood_Main, appArgs.pFood_Single_Player, food_amount_single_player);
}

void init_foods(int food_amount_single_player, int food_amount_multiplayer) {
	if (GAME_STATE == CRITICAL_ERROR) {
		return;
	}
	appArgs.pFood_Main = create_list();
	appArgs.pFood_Multiplayer = create_list();
	appArgs.pFood_Single_Player = create_list();
	if (appArgs.pFood_Main == NULL || appArgs.pFood_Multiplayer == NULL || appArgs.pFood_Single_Player == NULL) {
		error_message("ERROR: func food_init(): create_list() failed");
		GAME_STATE = CRITICAL_ERROR;
		return;
	}
	srand(time(NULL));

	int max_food = food_amount_multiplayer;
	if (food_amount_single_player > max_food) {
		max_food = food_amount_single_player;
	}

	Element* curr;
	while (max_food != appArgs.pFood_Main->size) {
		curr = create_element(-1, -1);
		if (curr == NULL) {
			error_message("ERROR: func food_init(): create_element() failed");
			GAME_STATE = CRITICAL_ERROR;
			return;
		}
		add_element_to_head(appArgs.pFood_Main, curr);
	}
	make_list_from(appArgs.pFood_Main, appArgs.pFood_Multiplayer, food_amount_multiplayer);
	make_list_from(appArgs.pFood_Main, appArgs.pFood_Single_Player, food_amount_single_player);
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
			random_food_generator(appArgs.pFood_Single_Player, appArgs.pSnake1, appArgs.pSnake2);
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
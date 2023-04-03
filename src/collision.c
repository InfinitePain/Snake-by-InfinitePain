/*
 ============================================================================
 Name        : collision.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include "collision.h"
#include "appdata.h"
#include "food.h"
#include <ncurses.h>
#include "terminal.h"

bool is_snake_collided(Snake* pSnake) {
	Element* pElement = pSnake->pos_snake->head->next;
	while (pElement != NULL) {
		if (pElement->pos.posx == pSnake->pos_snake->head->pos.posx && pElement->pos.posy == pSnake->pos_snake->head->pos.posy) {
			return true;
		}
		pElement = pElement->next;
	}
	return false;
}

bool is_snake_collided_with_wall(Snake* pSnake) {
	if (pSnake->pos_snake->head->pos.posx == 0 || pSnake->pos_snake->head->pos.posx == getmaxx(appWindows[GAME_WIN]) - 1 || pSnake->pos_snake->head->pos.posy == 0 || pSnake->pos_snake->head->pos.posy == getmaxy(appWindows[GAME_WIN]) - 1) {
		return true;
	}
	return false;
}

//checks if the first snake bites the second snake
bool is_snake_collided_with_snake(Snake* pSnake1, Snake* pSnake2) {
	Element* pElement = pSnake2->pos_snake->head;
	while (pElement != NULL) {
		if (pElement->pos.posx == pSnake1->pos_snake->head->pos.posx && pElement->pos.posy == pSnake1->pos_snake->head->pos.posy) {
			return true;
		}
		pElement = pElement->next;
	}
	return false;
}

Element* get_collided_food_element(List* pFood, Snake* pSnake) {
	Element* pElementFood = pFood->head;
	while (pElementFood != NULL) {
		if (pElementFood->pos.posx == pSnake->pos_snake->head->pos.posx && pElementFood->pos.posy == pSnake->pos_snake->head->pos.posy) {
			pthread_mutex_unlock(&GameThreads.thr_mutex[thr_food]);
			return pElementFood;
		}
		pElementFood = pElementFood->next;
	}
	return NULL;
}

void handle_food_collision(Element* collided_food, Snake* pSnake) {
	if (collided_food != NULL) {
		collided_food->pos.posx = -1;
		collided_food->pos.posy = -1;
		pSnake->grow++;
		pSnake->point++;
	}
}

void game_over() {
	GAME_STATE = GAME_OVER;
	pause_thread(thr_input1);
	pause_thread(thr_input2);
	pause_thread(thr_snake1);
	pause_thread(thr_snake2);
	pause_thread(thr_collision);
	resume_thread(thr_menu);
}

void* collision_thread(void* arg) {
	Element* collided_food = NULL;
	while (GameThreads.is_thr_init[thr_collision]) {
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_collision]);
		while (GameThreads.pause_flag[thr_collision]) {
			pthread_cond_wait(&GameThreads.pause_cond[thr_collision], &GameThreads.thr_mutex[thr_collision]);
		}
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_collision]);
		if (GAME_STATE == QUIT) {
			break;
		}
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_food]);
		switch (GAME_MODE) {
		case SINGLE_PLAYER:
			if (is_snake_collided(appArgs.pSnake1) || is_snake_collided_with_wall(appArgs.pSnake1)) {
				game_over();
				break;
			}
			collided_food = get_collided_food_element(appArgs.pFood_Single_Player, appArgs.pSnake1);
			if (collided_food != NULL) {
				handle_food_collision(collided_food, appArgs.pSnake1);
				pthread_cond_signal(&GameThreads.pause_cond[thr_food]);
			}
			break;
		case MULTIPLAYER:
			if (is_snake_collided(appArgs.pSnake1) || is_snake_collided_with_wall(appArgs.pSnake1)) {
				game_over();
				break;
			}
			if (is_snake_collided(appArgs.pSnake2) || is_snake_collided_with_wall(appArgs.pSnake2)) {
				game_over();
				break;
			}
			if (is_snake_collided_with_snake(appArgs.pSnake1, appArgs.pSnake2)) {
				game_over();
				break;
			}
			if (is_snake_collided_with_snake(appArgs.pSnake2, appArgs.pSnake1)) {
				game_over();
				break;
			}
			collided_food = get_collided_food_element(appArgs.pFood_Multiplayer, appArgs.pSnake1);
			if (collided_food != NULL) {
				handle_food_collision(collided_food, appArgs.pSnake1);
				pthread_cond_signal(&GameThreads.pause_cond[thr_food]);
			}
			collided_food = get_collided_food_element(appArgs.pFood_Multiplayer, appArgs.pSnake2);
			if (collided_food != NULL) {
				handle_food_collision(collided_food, appArgs.pSnake2);
				pthread_cond_signal(&GameThreads.pause_cond[thr_food]);
			}
			break;
		}
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_food]);
	}
	pthread_exit(NULL);
}
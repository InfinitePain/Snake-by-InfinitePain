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
#include "time.h"
#include "terminal.h"

clock_t start_time;
clock_t pause_total_time;
bool game_timer_needs_reset = false;


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
	pthread_mutex_unlock(&GameThreads.thr_mutex[thr_food]);
	pthread_mutex_unlock(&GameThreads.thr_mutex[thr_snake1]);
	pthread_mutex_unlock(&GameThreads.thr_mutex[thr_snake2]);
	GAME_STATE = GAME_OVER;
	pause_thread(thr_input1);
	pause_thread(thr_input2);
	pause_thread(thr_snake1);
	pause_thread(thr_snake2);
	pause_thread(thr_collision);
	resume_thread(thr_menu);
}

bool collision_snake(Snake* pSnake) {
	bool is_collided = false;
	if (is_snake_collided(pSnake) || is_snake_collided_with_wall(pSnake)) {
		pSnake->is_alive = false;
		is_collided = true;
	}
	return is_collided;
}

void collision_food() {
	Element* collided_food = NULL;
	bool is_food_eaten = false;
	switch (GAME_MODE) {
	case SINGLE_PLAYER:
		collided_food = get_collided_food_element(appArgs.pFood_Single_Player, appArgs.pSnake1);
		if (collided_food != NULL) {
			handle_food_collision(collided_food, appArgs.pSnake1);
			is_food_eaten = true;
		}
		break;
	case MULTIPLAYER:
		collided_food = get_collided_food_element(appArgs.pFood_Multiplayer, appArgs.pSnake1);
		if (collided_food != NULL) {
			handle_food_collision(collided_food, appArgs.pSnake1);
			is_food_eaten = true;
		}
		collided_food = get_collided_food_element(appArgs.pFood_Multiplayer, appArgs.pSnake2);
		if (collided_food != NULL) {
			handle_food_collision(collided_food, appArgs.pSnake2);
			is_food_eaten = true;
		}
		break;
	}
	if (is_food_eaten) {
		pthread_cond_signal(&GameThreads.pause_cond[thr_food]);
	}
}

bool collision_snake_with_snake() {
	bool is_collided = false;
	if (is_snake_collided_with_snake(appArgs.pSnake1, appArgs.pSnake2)) {
		appArgs.pSnake1->is_alive = false;
		is_collided = true;
	}
	if (is_snake_collided_with_snake(appArgs.pSnake2, appArgs.pSnake1)) {
		appArgs.pSnake2->is_alive = false;
		game_over();
		is_collided = true;
	}
	return is_collided;
}

double calculate_elapsed_time(clock_t start_time) {
	clock_t end_time = clock();
	double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	return elapsed_time;
}

void print_time(double elapsed_time) {
	int minutes = (int)elapsed_time / 60;
	int seconds = (int)elapsed_time % 60;
	mvwprintw(stdscr, 1, appArgs.pConfig->configs[SCREEN_WIDTH] / 2 - 3, "%02d:%02d", minutes, seconds);
	wrefresh(stdscr);
}

void reset_game_timer() {
	start_time = clock();
	pause_total_time = 0;
}

void* collision_thread(void* arg) {
	bool is_collided = false;
	clock_t pause_start_time;

	start_time = clock();
	while (GameThreads.is_thr_init[thr_collision]) {
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_collision]);
		while (GameThreads.pause_flag[thr_collision]) {
			pause_start_time = clock();
			increment_waiting_thread_count();
			pthread_cond_wait(&GameThreads.pause_cond[thr_collision], &GameThreads.thr_mutex[thr_collision]);
			decrement_waiting_thread_count();
			pause_total_time += clock() - pause_start_time;
			is_collided = false;
		}
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_collision]);
		if (GAME_STATE == QUIT) {
			break;
		}

		if (game_timer_needs_reset) {
			reset_game_timer();
			game_timer_needs_reset = false;
		}
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_food]);
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_snake1]);
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_snake2]);
		switch (GAME_MODE) {
		case SINGLE_PLAYER:
			if (collision_snake(appArgs.pSnake1)) {
				is_collided = true;
				game_over();
			}
			else {
				collision_food();
			}
				break;
		case MULTIPLAYER:
			if (collision_snake(appArgs.pSnake1) || collision_snake(appArgs.pSnake2) || collision_snake_with_snake()) {
				is_collided = true;
				game_over();
			}
			else {
				collision_food();
			}
			break;
		}
		if (!is_collided) {
			pthread_mutex_unlock(&GameThreads.thr_mutex[thr_food]);
			pthread_mutex_unlock(&GameThreads.thr_mutex[thr_snake1]);
			pthread_mutex_unlock(&GameThreads.thr_mutex[thr_snake2]);
		}
		print_time(calculate_elapsed_time(start_time) - pause_total_time / CLOCKS_PER_SEC);
	}
	pthread_exit(NULL);
}
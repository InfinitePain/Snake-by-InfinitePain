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
#define NCURSES_STATIC
#include <ncurses.h>
#include "terminal.h"
#include "terminal.h"
#include "timing_utils.h"
#include "app_status.h"

bool game_timer_needs_reset = false;
bool score_changed = true;
bool time_changed = true;

bool is_snake_collided(Snake* pSnake) {
	if (GAME_STATE == CRITICAL_ERROR) {
		return true;
	}
	Element* pElement = pSnake->pos_snake->head->next;
	while (pElement != NULL) {
		if (pElement->pos.posx == pSnake->pos_snake->head->pos.posx && pElement->pos.posy == pSnake->pos_snake->head->pos.posy) {
			return true;
		}
		pElement = pElement->next;
	}
	return false;
}

bool is_snake_collided_with_wall(Snake* pSnake, int width, int height) {
	if (GAME_STATE == CRITICAL_ERROR) {
		return true;
	}
	if (pSnake->pos_snake->head->pos.posx == 0 || pSnake->pos_snake->head->pos.posx == width - 1 || pSnake->pos_snake->head->pos.posy == 0 || pSnake->pos_snake->head->pos.posy == height - 1) {
		return true;
	}
	return false;
}

bool is_snake_collided_with_snake(Snake* pSnake1, Snake* pSnake2) {
	if (GAME_STATE == CRITICAL_ERROR) {
		return true;
	}
	if (pSnake1 == pSnake2) {
		return false;
	}
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
	if (GAME_STATE == CRITICAL_ERROR) {
		return NULL;
	}
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
	if (GAME_STATE == CRITICAL_ERROR) {
		return;
	}
	if (collided_food != NULL) {
		collided_food->pos.posx = -1;
		collided_food->pos.posy = -1;
		pSnake->grow++;
		pSnake->point++;
		score_changed = true;
	}
}

int best_score() {
	int best_score = appArgs.pSnake1->point;
	if (best_score < appArgs.pSnake2->point) {
		best_score = appArgs.pSnake2->point;
	}
	if (best_score < appArgs.pConfig->configs[MAX_POINT]) {
		best_score = appArgs.pConfig->configs[MAX_POINT];
	}
	return best_score;
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
	appArgs.pConfig->configs[MAX_POINT] = best_score();
	write_config(appArgs.pConfig);
	resume_thread(thr_menu);
}

bool collision_snake(Snake* pSnake) {
	bool is_collided = false;
	if (is_snake_collided(pSnake) || is_snake_collided_with_wall(pSnake, getmaxx(appWindows[GAME_WIN]), getmaxy(appWindows[GAME_WIN])) || is_snake_collided_with_snake(pSnake, appArgs.pSnake1) || is_snake_collided_with_snake(pSnake, appArgs.pSnake2)) {
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

void print_time(double elapsed_time) {
	if (time_changed){
		int minutes = (int)elapsed_time / 60;
		int seconds = (int)elapsed_time % 60;
		pthread_mutex_lock(&GameThreads.thr_mutex[mutex_win_game]);
		mvwprintw(stdscr, 1, appArgs.pConfig->configs[SCREEN_WIDTH] / 2 - 3, "%02d:%02d", minutes, seconds);
		wrefresh(stdscr);
		pthread_mutex_unlock(&GameThreads.thr_mutex[mutex_win_game]);
		time_changed = false;
	}
}

void print_score() {
	if (score_changed) {
		for (int i = 1; i < appArgs.pConfig->configs[SCREEN_WIDTH] / 2 - 5; i++) {
			mvwaddch(stdscr, 1, i, ' ');
			mvwaddch(stdscr, 2, i, ' ');
		}
		pthread_mutex_lock(&GameThreads.thr_mutex[mutex_win_game]);
		switch (GAME_MODE) {
		case SINGLE_PLAYER:
			mvwprintw(stdscr, 1, 3, "Score: %d", appArgs.pSnake1->point);
			break;
		case MULTIPLAYER:
			mvwprintw(stdscr, 1, 3, "Score 1: %d", appArgs.pSnake1->point);
			mvwprintw(stdscr, 2, 3, "Score 2: %d", appArgs.pSnake2->point);
			break;
		}
		mvwprintw(stdscr, 1, appArgs.pConfig->configs[SCREEN_WIDTH] - 12, "Best: %3d", best_score());
		wrefresh(stdscr);
		pthread_mutex_unlock(&GameThreads.thr_mutex[mutex_win_game]);
		score_changed = false;
	}
}

void* collision_thread(void* arg) {
	bool is_collided = false;
	double start_time, pause_start_time, pause_total_time;
	double last_time_update = 0;

	reset_timer(&start_time, &pause_total_time);
	while (GameThreads.is_thr_init[thr_collision]) {
		pthread_mutex_lock(&GameThreads.thr_mutex[thr_collision]);
		while (GameThreads.pause_flag[thr_collision]) {
			pause_start_time = get_current_time_in_seconds();
			increment_waiting_thread_count();
			pthread_cond_wait(&GameThreads.pause_cond[thr_collision], &GameThreads.thr_mutex[thr_collision]);
			decrement_waiting_thread_count();
			pause_total_time += get_current_time_in_seconds() - pause_start_time;
			is_collided = false;
			score_changed = true;
		}
		pthread_mutex_unlock(&GameThreads.thr_mutex[thr_collision]);
		if (GAME_STATE == QUIT) {
			break;
		}

		if (game_timer_needs_reset) {
			reset_timer(&start_time, &pause_total_time);
			last_time_update = start_time;
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
		print_score();
		if (!is_collided) {
			pthread_mutex_unlock(&GameThreads.thr_mutex[thr_food]);
			pthread_mutex_unlock(&GameThreads.thr_mutex[thr_snake1]);
			pthread_mutex_unlock(&GameThreads.thr_mutex[thr_snake2]);
		}
        double current_time = calculate_elapsed_time(start_time) - pause_total_time;
        if (current_time - last_time_update >= 1.0) {
            time_changed = true;
            last_time_update = current_time;
        }

        print_time(current_time);
	}
	pthread_exit(NULL);
}
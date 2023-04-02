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
	*x = (rand() % (getmaxx(appWindows[GAME_WIN]) - 1)) + 1;
	*y = (rand() % (getmaxy(appWindows[GAME_WIN]) - 1)) + 1;
}

bool food_coordinate_checker(List* pList, int x, int y) {
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

void random_food_generator(List* pFood, Snake* pSnake) {
	int x, y;
	Element* curr = food_adresser(pFood);
	while (curr != NULL) {
		random_coordinate_generator(&x, &y);
		while (food_coordinate_checker(pFood, x, y) || food_coordinate_checker(pSnake->pos_snake, x, y)) {
			random_coordinate_generator(&x, &y);
		}
		curr->pos.posx = x;
		curr->pos.posy = y;
		curr = food_adresser(pFood);
	}
}

void handle_food_collision(List* pFood, Snake* pSnake) {
	Element* pElementFood = pFood->head;
	while (pElementFood != NULL) {
		if (pElementFood->pos.posx == pSnake->pos_snake->head->pos.posx && pElementFood->pos.posy == pSnake->pos_snake->head->pos.posy) {
			pElementFood->pos.posx = -1;
			pElementFood->pos.posy = -1;
			pSnake->grow++;
		}
		pElementFood = pElementFood->next;
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

	
}
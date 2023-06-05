/*
 ============================================================================
 Name        : food.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef FOOD_H_
#define FOOD_H_

#include <stdbool.h>
#include "list.h"
#include "snake.h"

typedef struct AvailablePositions {
	Position* positions;
	int count;
} AvailablePositions;

void delete_available_positions(AvailablePositions* ap);
AvailablePositions* create_available_positions(int width, int height);
void remove_position(AvailablePositions* ap, int x, int y);
void add_position(AvailablePositions* ap, int x, int y);
void delete_foods();
void get_random_position(AvailablePositions* ap, int* x, int* y);
Element* food_adresser(List* pList);
void random_food_generator(List* pFood, Snake* pSnake1, Snake* pSnake2);
void reset_food(List* pFood);
void reset_available_positions(AvailablePositions* ap, int width, int height);
void resize_foods(int food_amount_single_player, int food_amount_multiplayer);
void init_foods(int food_amount_single_player, int food_amount_multiplayer);
void* food_thread(void* arg);

#endif /*FOOD_H_*/
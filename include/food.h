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

void delete_foods();
void random_coordinate_generator(int* x, int* y);
bool food_coordinate_checker(List* pList, int x, int y);
Element* food_adresser(List* pList);
void random_food_generator(List* pFood, Snake* pSnake1, Snake* pSnake2);

void reset_food(List* pFood);
void init_foods();
void* food_thread(void* arg);

#endif /*FOOD_H_*/
/*
 ============================================================================
 Name        : snake.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef SNAKE_H_
#define SNAKE_H_

#include "list.h"
#include "config.h"
#include "input.h"
#include <stdbool.h>
#include <pthread.h>

typedef struct Snake
{
	
	bool gameState;
	List* pos_snake;
	int grow;
	int point;
	int dir;
	int color;
} Snake;

void delete_snake(Snake* pSnake);
Snake* create_snake();
void move_snake(const Config* pConfig, int direction, Snake* pSnake);
void* snake_thread(void* args);

#endif /*SNAKE_H_*/
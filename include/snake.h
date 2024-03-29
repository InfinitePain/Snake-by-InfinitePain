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
#include <setjmp.h>
#include "direction_buffer.h"

typedef struct Snake {
	bool is_alive;
	List* pos_snake;
	int grow;
	int point;
	DirectionBuffer dir_buffer;
	int color;
} Snake;

void delete_snake(Snake* pSnake);
void set_snake_position(Snake* pSnake, int x, int y);
Snake* create_snake(int length, int color, int direction, int startx, int starty);
void set_snake_color(Snake* pSnake, int color);
void restart_snake(Snake* pSnake);
void move_snake(const Config* pConfig, int direction, Snake* pSnake);
bool is_key_reverse(int key, int dir);
void* snake_thread(void* args);

#endif /*SNAKE_H_*/
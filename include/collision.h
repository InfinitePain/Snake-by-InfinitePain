/*
 ============================================================================
 Name        : collision.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef COLLISION_H_
#define COLLISION_H_

#include <stdbool.h>
#include "snake.h"

extern bool game_timer_needs_reset;


bool is_snake_collided(Snake* pSnake);
bool is_snake_collided_with_wall(Snake* pSnake, int width, int height);
bool is_snake_collided_with_snake(Snake* pSnake1, Snake* pSnake2);
Element* get_collided_food_element(List* pFood, Snake* pSnake);
void handle_food_collision(Element* collided_food, Snake* pSnake);
int best_score();
void game_over();
bool collision_snake(Snake* pSnake);
void collision_food();
bool collision_snake_with_snake();
void print_time(double elapsed_time);
void print_score();
void* collision_thread(void* arg);

#endif /*COLLISION_H_*/
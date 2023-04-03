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

bool is_snake_collided(Snake* pSnake);
bool is_snake_collided_with_wall(Snake* pSnake);
bool is_snake_collided_with_snake(Snake* pSnake1, Snake* pSnake2);
Element* get_collided_food_element(List* pFood, Snake* pSnake);
void handle_food_collision(Element* collided_food, Snake* pSnake);
void* collision_thread(void* arg);

#endif /*COLLISION_H_*/
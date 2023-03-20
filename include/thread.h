/*
 ============================================================================
 Name        : thread.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef THREAD_H_
#define THREAD_H_

#include <stdbool.h>
#include <pthread.h>
#include "input.h"
#include "snake.h"

void pause_thread(int thrnum);
void pause_game();
void resume_thread(int thrnum);
void resume_game();
void destroy_thread(int thrnum);
void create_thread(int thrnum);

#endif /*THREAD_H_*/
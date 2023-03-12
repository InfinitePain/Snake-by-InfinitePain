/*
 ============================================================================
 Name        : wall.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef WALL_H_
#define WALL_H_

#include "list.h"
#include "config.h"
#include "appdata.h"

int get_wall_start_x();
int get_wall_end_x();
int get_wall_start_y();
int get_wall_end_y();
List* create_wall();

#endif /*WALL_H_*/
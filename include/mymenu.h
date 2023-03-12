/*
 ============================================================================
 Name        : mymenu.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef MYMENU_H_
#define MYMENU_H_

#include "list.h"
#include "config.h"
#include "input.h"

typedef struct Mymenu {
	List* Options;
	List* Highlighter;
} Mymenu;

void delete_mymenu(Mymenu* pMymenu);
Mymenu* create_Mymenu();
void create_mymenu_options(Mymenu* pMymenu);
void create_mymenu_highlight(Mymenu* pMymenu);
void update_highlighter_pos(Element* curr, Element* prev, WINDOW* window);
int mymenu();

#endif /*MYMENU_H_*/
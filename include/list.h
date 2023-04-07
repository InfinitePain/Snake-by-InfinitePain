/*
 ============================================================================
 Name        : list.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
 ============================================================================
 */

#ifndef LIST_H_
#define LIST_H_

#include "element.h"
#include <stdbool.h>
#include <pthread.h>
#define NCURSES_STATIC
#include <ncurses.h>

typedef struct List {
	Element* head;
	int size;
} List;

List* create_list();
void add_element_to_head(List* pList, Element* pElement);
Element* get_element_at_index(List* pList, const int index);
void delete_last_element(List* pList);
void delete_list(List* pList);
void list_printer(List* pList, const int color, const int time, WINDOW* window);
void make_list_from(List* source_list, List* target_list, const int size);

#endif /* LIST_H_ */
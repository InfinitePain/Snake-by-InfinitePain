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

#include <element.h>
#include <stdbool.h>
#include <pthread.h>
#include <ncurses.h>

typedef struct List {
	Element* head;
	Element* tail;
	int size;
} List;

List* create_list();
void add_element_to_head(List* pList, Element* pElement);
void add_element_to_tail(List* pList, Element* pElement);
bool is_element_in_list(Element* pCheck, List* pList);
Element* find_element_at_index(List* pList, const int index);
void delete_first_element(List* pList);
void delete_last_element(List* pList);
void delete_list(List* pList);
void list_printer(List* pList, const int color, const int time, WINDOW* window);
void update_position(List* pList, const int posx, const int posy);
List* copy_list(List* original);

#endif /* LIST_H_ */
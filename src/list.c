/*
 ============================================================================
 Name        : list.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
 ============================================================================
 */

#include "list.h"
#include <stdlib.h>
#include <error_message.h>
#include <setjmp.h>

jmp_buf jmp_buffer3;

List* create_list(){
	List* pList = (List*)malloc(sizeof(List));
	if (pList == NULL) {
		error_message("ERROR: func create_list(): malloc");
		return NULL;
	}
	pList->head = NULL;
	pList->size = 0;
	return pList;
}

void add_element_to_head(List* pList, Element* pElement) {
	if (pList->head == NULL) {
		// The list is empty, so set the head and tail to the new element
		pList->head = pElement;
		pElement->next = NULL;
		pElement->index = 0;
	}
	else {
		// The list is not empty, so add the new element to the beginning
		Element* curr = pList->head;
		while (curr != NULL) {
			curr->index += 1;
			curr = curr->next;
		}
		pElement->next = pList->head;
		pElement->index = 0;
		pList->head = pElement;
	}
	pList->size++;
}

Element* get_element_at_index(List* pList, const int index) {
	if (pList->head == NULL) {
		return NULL;
	}
	Element* curr = pList->head;
	while (curr != NULL) {
		if (curr->index == index) {
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

void delete_last_element(List* pList) {
	if (pList->head == NULL) {
		return;
	}

	if (pList->head->next == NULL) {
		free(pList->head);
		pList->head = NULL;
	}
	else {
		Element* temp = get_element_at_index(pList, pList->size - 2);
		free(temp->next);
		temp->next = NULL;
	}
	--pList->size;
}

void delete_list(List* pList) {
	if (pList == NULL) {
		return;
	}
	if (pList->head == NULL) {
		free(pList);
		pList = NULL;
		return;
	}
	Element* curr = pList->head;
	Element* temp;

	while (pList->head != NULL) {
		delete_last_element(pList);
	}
	free(pList);
	pList = NULL;
}

void list_printer(List* pList, const int color, const int time, WINDOW* window) {
	Element* curr = pList->head;
	wattron(window, COLOR_PAIR(color));
	while (curr != NULL) {
		usleep(time);
		mvwaddch(window, curr->pos.posy, curr->pos.posx, ' ');
		wrefresh(window);
		curr = curr->next;
	}
	wattroff(window, COLOR_PAIR(color));
}

void make_list_from(List* source_list, List* target_list, const int size) {
	if (source_list == NULL || target_list == NULL) {
		return;
	}
	if (size > source_list->size) {
		return;
	}
	target_list->size = size;
	target_list->head = get_element_at_index(source_list, source_list->size - size);
}
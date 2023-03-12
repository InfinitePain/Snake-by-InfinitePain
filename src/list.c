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
		error_message("ERROR func create_list");
		return NULL;
	}
	pList->head = NULL;
	pList->tail = NULL;
	pList->size = 0;
	return pList;
}

void add_element_to_head(List* pList, Element* pElement) {
	if (pList->head == NULL) {
		// The list is empty, so set the head and tail to the new element
		pList->head = pElement;
		pList->tail = pElement;
		pElement->next = pElement;
		pElement->prev = pElement;
		pElement->index = 0;
	}
	else {
		// The list is not empty, so add the new element to the beginning
		Element* curr = pList->head;
		do {
			curr->index += 1;
			curr = curr->next;
		} while (curr != pList->head);
		pElement->next = pList->head;
		pElement->prev = pList->tail;
		pElement->index = 0;
		pList->head->prev = pElement;
		pList->tail->next = pElement;
		pList->head = pElement;
	}
	pList->size++;
}

void add_element_to_tail(List* pList, Element* pElement) {
	if (pList->tail == NULL) {
		// The list is empty, so set the head and tail to the new element
		pList->head = pElement;
		pList->tail = pElement;
		pElement->next = pElement;
		pElement->prev = pElement;
		pElement->index = pList->size;
	}
	else {
		// The list is not empty, so add the new element to the end
		pList->tail->next = pElement;
		pElement->prev = pList->tail;
		pElement->next = pList->head;
		pElement->index = pList->size;
		pList->tail = pElement;
		pList->head->prev = pList->tail;
	}
	pList->size++;
}

// Check if pCheck matches with one of pElements successors 
bool is_element_in_list(Element* pCheck, List* pList) {
	Element* curr = pList->head;
	do {
		if (pCheck->pos.posx == curr->pos.posx && pCheck->pos.posy == curr->pos.posy) {
			return true; //in the list
		}
		curr = curr->next;
	} while (curr != pList->head);
	return false; //not in the list
}

Element* find_element_at_index(List* pList, const int index) {
	Element* curr = pList->head;
	do {
		if (curr->index == index) {
			return curr;
		}
		curr = curr->next;
	} while (curr != pList->head);
	return NULL;
}

void delete_first_element(List* pList) {
	if (pList->head == pList->tail) {
		free(pList->head);
		pList->head = NULL;
		pList->tail = NULL;
	}
	else {
		pList->tail->next = pList->head->next;
		pList->head->next->prev = pList->tail;
		free(pList->head);
		pList->head = pList->tail->next;
		Element* curr = pList->head;
		do {
			curr->index -= 1;
			curr = curr->next;
		} while (curr != pList->head);
	}
	--pList->size;
}

void delete_last_element(List* pList) {
	if (pList->head == pList->tail) {
		free(pList->head);
		pList->head = NULL;
		pList->tail = NULL;
	}
	else {
		pList->head->prev = pList->tail->prev;
		pList->tail->prev->next = pList->head;
		free(pList->tail);
		pList->tail = pList->head->prev;
	}
	--pList->size;
}

void delete_list(List* pList) {
	if (pList == NULL) {
		return;
	}
	if (pList->head == NULL) {
		free(pList);
		return;
	}
	pList->head->prev = NULL;
	pList->tail->next = NULL;
	Element* curr = pList->head;
	Element* temp;
	while (curr != NULL) {
		temp = curr;
		curr = curr->next;
		free(temp);
		temp = NULL;
	}
	free(pList);
	pList = NULL;
}

void list_printer(List* pList, const int color, const int time, WINDOW* window) {
	Element* curr = pList->head;
	wattron(window, COLOR_PAIR(color));
	do {
		usleep(time);
		mvwaddch(window, curr->pos.posy, curr->pos.posx, ' ');
		wrefresh(window);
		curr = curr->next;
	} while (curr != pList->head);
	wattroff(window, COLOR_PAIR(color));
}

void update_position(List* pList, const int posx, const int posy) {
	Element* curr = pList->head;
	do {
		curr->pos.posx += posx;
		curr->pos.posy += posy;
		curr = curr->next;
	} while (curr != pList->head);
}

List* copy_list(List* original) {
	if (original == NULL)
	{
		return NULL;
	}
	
	List* copy = create_list();
	if (copy == NULL)
	{
		error_message("ERROR: func copy_list");
		return NULL;
	}	
	if (setjmp(jmp_buffer3) == 0) {
		for (int i = 0; i < original->size; i++)
		{
			add_element_to_tail(copy, create_element(0, 0, 3));
		}
	}
	else {
		delete_list(copy);
		return NULL;
	}
	Element* pOrg = original->head;
	Element* pCop = copy->head;
	do {
		pCop->pos.posx = pOrg->pos.posx;
		pCop->pos.posy = pOrg->pos.posy;
		pCop = pCop->next;
		pOrg = pOrg->next;
	} while (pOrg != original->head);
	return copy;
}




















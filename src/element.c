/*
 ============================================================================
 Name        : element.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
 ============================================================================
 */

#include "element.h"
#include <stdlib.h>
#include "error_message.h"
#define NCURSES_STATIC
#include <ncurses.h>
#include <setjmp.h>
#include <time.h>
#include "app_status.h"

Element* create_element(const int posx, const int posy) {
	if (GAME_STATE == CRITICAL_ERROR) {
		return NULL;
	}
	Element* pElement = (Element*)malloc(sizeof(Element));
	if (pElement == NULL) {
		error_message("ERROR: func create_element(): malloc");
		GAME_STATE = CRITICAL_ERROR;
		return NULL;
	}
	pElement->pos.posx = posx;
	pElement->pos.posy = posy;
	pElement->index = -1;
	pElement->next = NULL;
	return pElement;
}

void printer_element(const Element* pElement, const int color, const int time, WINDOW* window) {
	if (GAME_STATE == CRITICAL_ERROR || pElement == NULL || window == NULL) {
		return;
	}
	wattron(window, COLOR_PAIR(color));
	usleep(time);
	mvwaddch(window, pElement->pos.posy, pElement->pos.posx, ' ');
	wrefresh(window);
	wattroff(window, COLOR_PAIR(color));
}
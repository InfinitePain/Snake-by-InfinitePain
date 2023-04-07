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

extern jmp_buf jmp_buffer3;
extern jmp_buf jmp_buffer7;
extern jmp_buf jmp_buffer12;


/*
3 for list_copy
7 for the wall functions
12 for init_food
*/
Element* create_element(const int posx, const int posy, int purpose) {
	Element* pElement = (Element*)malloc(sizeof(Element));
	if (pElement == NULL) {
		error_message("ERROR: func create_element(): malloc");
		switch (purpose)
		{
		case 3:
			longjmp(jmp_buffer3, 1);
			break;
		case 7:
			longjmp(jmp_buffer7, 1);
			break;
		case 12:
			longjmp(jmp_buffer12, 1);
			break;
		}
		return NULL;
	}
	pElement->pos.posx = posx;
	pElement->pos.posy = posy;
	pElement->index = -1;
	pElement->next = NULL;
	return pElement;
}

void printer_element(const Element* pElement, const int color, const int time) {
	attron(COLOR_PAIR(color));
	usleep(time);
	mvaddch(pElement->pos.posy, pElement->pos.posx, ' ');
	refresh();
	attroff(COLOR_PAIR(color));
}
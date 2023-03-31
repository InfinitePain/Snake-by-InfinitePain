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
#include <ncurses.h>
#include <setjmp.h>
#include <time.h>

//extern jmp_buf jmp_buffer2;
extern jmp_buf jmp_buffer3;
extern jmp_buf jmp_buffer4;
extern jmp_buf jmp_buffer5;
extern jmp_buf jmp_buffer7;
extern jmp_buf jmp_buffer8;

/*
2 for letter_create
3 for list_copy
4 for create_mymenu_options
5 for create_mymenu_highlight
7 for the wall functions
8 for move_snake
*/
Element* create_element(const int posx, const int posy, int purpose) {
	Element* pElement = (Element*)malloc(sizeof(Element));
	if (pElement == NULL) {
		error_message("ERROR: func create_element(): malloc");
		switch (purpose)
		{
		/*case 2:
			longjmp(jmp_buffer2, 1);
			break;*/
		case 3:
			longjmp(jmp_buffer3, 1);
			break;
		/*case 4:
			longjmp(jmp_buffer4, 1);
			break;
		case 5:
			longjmp(jmp_buffer5, 1);
			break;*/
		case 7:
			longjmp(jmp_buffer7, 1);
			break;
		case 8:
			longjmp(jmp_buffer8, 1);
		default:
			break;
		}
		return NULL;
	}
	pElement->pos.posx = posx;
	pElement->pos.posy = posy;
	pElement->index = -1;
	pElement->next = NULL;
	pElement->prev = NULL;
	return pElement;
}

void printer_element(const Element* pElement, const int color, const int time) {
	attron(COLOR_PAIR(color));
	usleep(time);
	mvaddch(pElement->pos.posy, pElement->pos.posx, ' ');
	refresh();
	attroff(COLOR_PAIR(color));
}
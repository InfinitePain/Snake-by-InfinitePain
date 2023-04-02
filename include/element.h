/*
 ============================================================================
 Name        : element.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
 ============================================================================
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include "position.h"

typedef struct Element {
	int index;
	Position pos;
	struct Element* next;
} Element;

Element* create_element(const int posx, const int posy, int purpose);
void printer_element(const Element* pElement, const int color, const int time);

#endif /* ELEMENT_H_ */
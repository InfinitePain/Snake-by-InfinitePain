/*
 ============================================================================
 Name        : alphabet.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef ALPHABET_H_
#define ALPHABET_H_

#include "list.h"
#include "config.h"
#include "input.h"
#include <stdbool.h>

typedef struct Alphabet {
	List* letters[26];
} Alphabet;

void delete_alphabet(Alphabet *pAlphabet);
Alphabet* create_alphabet();
void print_text_at_position(const char *text, int posx, int posy, WINDOW* window);

#endif /*ALPHABET_H_*/
/*
 ============================================================================
 Name        : mymenu.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include "mymenu.h"
#include "letters.h"
#include "error_message.h"
#include <stdlib.h>
#include "alphabet.h"
#include <setjmp.h>
#include "appdata.h"

jmp_buf jmp_buffer4;
jmp_buf jmp_buffer5;
jmp_buf jmp_buffer6;
extern jmp_buf jmp_buffer10;

void delete_mymenu(Mymenu* pMymenu) {
	if (pMymenu == NULL){
		return;
	}	
	delete_list(pMymenu->Highlighter);
	delete_list(pMymenu->Options);
	free(pMymenu);
	pMymenu = NULL;
}

void create_mymenu_options(Mymenu* pMymenu) {
	pMymenu->Options = create_list();
	if (pMymenu->Options == NULL) {
		longjmp(jmp_buffer6, 1);
	}
	if (setjmp(jmp_buffer4) == 0)
	{
		add_element_to_head(pMymenu->Options, create_element(0, 40, 4));
		add_element_to_head(pMymenu->Options, create_element(0, 31, 4));
		add_element_to_head(pMymenu->Options, create_element(0, 22, 4));
		add_element_to_head(pMymenu->Options, create_element(0, 13, 4));
	}
	else {
		delete_mymenu(pMymenu);
		longjmp(jmp_buffer6, 1);
	}
}

void create_mymenu_highlight(Mymenu* pMymenu) {
	pMymenu->Highlighter = create_list();
	if (pMymenu->Highlighter == NULL) {
		longjmp(jmp_buffer6, 1);
	}
	if (setjmp(jmp_buffer5) != 1)
	{
		add_element_to_tail(pMymenu->Highlighter, create_element(0, 0, 5));
		add_element_to_tail(pMymenu->Highlighter, create_element(0, 1, 5));
		add_element_to_tail(pMymenu->Highlighter, create_element(0, 2, 5));
		add_element_to_tail(pMymenu->Highlighter, create_element(1, 0, 5));
		add_element_to_tail(pMymenu->Highlighter, create_element(1, 1, 5));
		add_element_to_tail(pMymenu->Highlighter, create_element(1, 2, 5));
	}
	else {
		delete_mymenu(pMymenu);
		longjmp(jmp_buffer6, 1);
	}
}

Mymenu* create_Mymenu() {
	Mymenu* pMymenu = (Mymenu*)malloc(sizeof(Mymenu));
	if (pMymenu == NULL) {
		error_message("ERROR func create_Mymenu");
		longjmp(jmp_buffer10, 1);
	}
	pMymenu->Options = NULL;
	pMymenu->Highlighter = NULL;
	if (setjmp(jmp_buffer6) != 1) {
		create_mymenu_options(pMymenu);
		create_mymenu_highlight(pMymenu);
	}
	else {
		error_message("ERROR: Can't initialize the menu");
		longjmp(jmp_buffer10, 1);
	}
	return pMymenu;
}

void update_highlighter_pos(Element* curr, Element* prev, WINDOW* window) {
	update_position(appArgs.pMymenu->Highlighter, prev->pos.posx, prev->pos.posy + 3);
	list_printer(appArgs.pMymenu->Highlighter, appArgs.pConfig->BACKGROUND_COLOR, 0, window);
	update_position(appArgs.pMymenu->Highlighter, -prev->pos.posx, -(prev->pos.posy + 3));
	update_position(appArgs.pMymenu->Highlighter, curr->pos.posx, curr->pos.posy + 3);
	list_printer(appArgs.pMymenu->Highlighter, appArgs.pConfig->TEXT_2_COLOR, 0, window);
	update_position(appArgs.pMymenu->Highlighter, -curr->pos.posx, -(curr->pos.posy + 3));
}

int mymenu() {

	print_text_at_position("SNAKE BY IP", 9, 0, appArgs.window_game);
	print_text_at_position("SINGLE PLAYER", 3, 13, appArgs.window_game);
	print_text_at_position("MULTIPLAYER", 3, 22, appArgs.window_game);
	print_text_at_position("SETTINGS", 3, 31, appArgs.window_game);
	print_text_at_position("QUIT", 3, 40, appArgs.window_game);
	reset_input(appArgs.pInput1);

	Element* curr = appArgs.pMymenu->Options->head;
	Element* prev = appArgs.pMymenu->Options->tail;
	update_position(appArgs.pMymenu->Highlighter, curr->pos.posx, curr->pos.posy + 3);
	list_printer(appArgs.pMymenu->Highlighter, appArgs.pConfig->TEXT_2_COLOR, 0, appArgs.window_game);
	update_position(appArgs.pMymenu->Highlighter, -curr->pos.posx, -(curr->pos.posy + 3));
	while (appArgs.pInput1->player_input != '\n')
	{
		pthread_mutex_trylock(&appArgs.pInput1->input_mutex);
		if (is_up_pressed(appArgs.pInput1)) {
			if (curr == appArgs.pMymenu->Options->head) {
				prev = curr;
				curr = appArgs.pMymenu->Options->tail;
			}
			else {
				prev = curr;
				curr = curr->prev;
			}
		}
		else if (is_down_pressed(appArgs.pInput1)) {
			if (curr == appArgs.pMymenu->Options->tail) {
				prev = curr;
				curr = appArgs.pMymenu->Options->head;
			}
			else {
				prev = curr;
				curr = curr->next;
			}
		}
		pthread_mutex_unlock(&appArgs.pInput1->input_mutex);
		reset_input(appArgs.pInput1);
		update_highlighter_pos(curr, prev, appArgs.window_game);
	}
	clear();
	refresh();
	return curr->index;
}
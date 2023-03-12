/*
 ============================================================================
 Name        : alphabet.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include "alphabet.h"
#include "error_message.h"
#include "letters.h"
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "appdata.h"

jmp_buf jmp_buffer1;
extern jmp_buf jmp_buffer10;

void delete_alphabet(Alphabet* pAlphabet) {
    for (int i = 0; i < 26; i++)
    {
         delete_list(pAlphabet->letters[i]);
    }
    free(pAlphabet);
    pAlphabet = NULL;
}

Alphabet* create_alphabet() {
	Alphabet* pAlphabet = (Alphabet*)malloc(sizeof(Alphabet));
	if (pAlphabet == NULL) {
		error_message("ERROR: Cant't initialize alphabet");
		longjmp(jmp_buffer10, 1);
	}
    memset(pAlphabet, 0, sizeof(Alphabet));
    if (setjmp(jmp_buffer1) == 0) {
        pAlphabet->letters[0] = create_letter_A();
        pAlphabet->letters[1] = create_letter_B();
        pAlphabet->letters[2] = create_letter_C();
        pAlphabet->letters[3] = create_letter_D();
        pAlphabet->letters[4] = create_letter_E();
        pAlphabet->letters[5] = create_letter_F();
        pAlphabet->letters[6] = create_letter_G();
        pAlphabet->letters[7] = create_letter_H();
        pAlphabet->letters[8] = create_letter_I();
        pAlphabet->letters[9] = create_letter_J();
        pAlphabet->letters[10] = create_letter_K();
        pAlphabet->letters[11] = create_letter_L();
        pAlphabet->letters[12] = create_letter_M();
        pAlphabet->letters[13] = create_letter_N();
        pAlphabet->letters[14] = create_letter_O();
        pAlphabet->letters[15] = create_letter_P();
        pAlphabet->letters[16] = create_letter_Q();
        pAlphabet->letters[17] = create_letter_R();
        pAlphabet->letters[18] = create_letter_S();
        pAlphabet->letters[19] = create_letter_T();
        pAlphabet->letters[20] = create_letter_U();
        pAlphabet->letters[21] = create_letter_V();
        pAlphabet->letters[22] = create_letter_W();
        pAlphabet->letters[23] = create_letter_X();
        pAlphabet->letters[24] = create_letter_Y();
        pAlphabet->letters[25] = create_letter_Z();
    }
    else {
        longjmp(jmp_buffer10, 1);
    }
    return pAlphabet;
}

void print_text_at_position(const char *text, int posx, int posy, WINDOW* window) {
	int time = 14000;
	for (int i = 0; text[i] != '\0'; i++) {
		char c = text[i];
		if (appArgs.animation) {
            if (appArgs.pInput1->player_input == 32 || appArgs.pInput1->player_input == 10 || appArgs.pInput1->player_input == 27) {
                appArgs.animation = false;
                appArgs.pInput1->player_input == ERR;
            }
		}
		
		if (c >= 'A' && c <= 'Z') {
			List* pList = copy_list(appArgs.pAlphabet->letters[c - 'A']);
			update_position(pList, posx, posy);
			if (appArgs.animation) {
				list_printer(pList, appArgs.pConfig->TEXT_2_COLOR, time, window);
			}
			else {
				list_printer(pList, appArgs.pConfig->TEXT_2_COLOR, 0, window);
			}
            posx += 6;
            delete_list(pList);
        }
        else if (c == ' ') {
			posx += 6;
		} else {
			// Print other characters as-is
			mvaddch(posy, posx, c);
            posx++;
            refresh();
            wrefresh(window);
		}
	}
}
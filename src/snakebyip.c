/*
 ============================================================================
 Name        : snakebyip.c
 Author      : InfinitePain
 Version     : 0.1.0
 License     : Apache License 2.0
 Description : Snake game made by InfinitePain
 ============================================================================
 */

#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "config.h"
#include "input.h"
#include <pthread.h>
#include "error_message.h"
#include "terminal.h"
#include <setjmp.h>
#include <stdbool.h>
#include "appdata.h"
#include "thread.h"
#include "mymenu.h"

 /*
jmp_buffer1 for create_alphabet
jmp_buffer2  for create_letter_
jmp_buffer3  for copy_list
jmp_buffer4  for create_mymenu_options
jmp_buffer5  for create_mymenu_highlight
jmp_buffer6  for mymenu
jmp_buffer7  for init_wall
jmp_buffer8  for move_snake
jmp_buffer9  for single_player
jmp_buffer10 for main
jmp_buffer11 for create_item_list
 */

jmp_buf jmp_buffer10;



int main(void) {
	if (setjmp(jmp_buffer10) != 1) {
		// init_screen();
		init_appData();
	}
	else {
		destroy_appData();
		mvprintw(error_line, 0, "ERROR: A critical error occurred. Press any key to close this window . . .");
		refresh();
		getchar();
		destroy_screen();
		return EXIT_FAILURE;
	}
	
	resume_thread(thr_menu);
	
	pthread_mutex_lock(&appArgs.mutex_main);
	if (appArgs.pause_flag_main == true) {
		pthread_cond_wait(&appArgs.cond_main, &appArgs.mutex_main);
	}
	pthread_mutex_unlock(&appArgs.mutex_main);

	destroy_appData();
	destroy_screen();
	return EXIT_SUCCESS;
}
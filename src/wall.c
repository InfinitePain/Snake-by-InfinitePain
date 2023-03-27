/*
 ============================================================================
 Name        : wall.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include "wall.h"
#include <setjmp.h>
#include "error_message.h"
#include "appdata.h"
#include "terminal.h"

jmp_buf jmp_buffer7;
extern jmp_buf jmp_buffer10;

List* create_wall()
{
	int x_start = 0;
	int x_end = getmaxx(appWindows[GAME_WIN]);
	int y_start = 0;
	int y_end = getmaxy(appWindows[GAME_WIN]);
	List* pList = create_list();
	if (pList == NULL)
	{
		error_message("ERROR: func create_wall(): create_list() failed");
		longjmp(jmp_buffer10, 1);
	}

	if (setjmp(jmp_buffer7) != 1)
	{
		for (int x = x_start; x < x_end; x++)
		{
			add_element_to_tail(pList, create_element(x, y_start, 7));
			add_element_to_tail(pList, create_element(x, y_end - 1, 7));

		}
		for (int y = y_start; y < y_end; y++)
		{
			add_element_to_tail(pList, create_element(x_start, y, 7));
			add_element_to_tail(pList, create_element(x_end - 1, y, 7));
		}
	}
	else
	{
		error_message("ERROR: func create_wall(): create_element() failed");
		longjmp(jmp_buffer10, 1);
	}
	return pList;
}






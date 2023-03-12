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

jmp_buf jmp_buffer7;
extern jmp_buf jmp_buffer10;

int get_wall_start_x() {
	int posx = appArgs.pConfig->SCREEN_OFFSET_X;
	return posx;
}

int get_wall_end_x() {
	int posx = COLS - get_wall_start_x() - 1;
	return posx;
}

int get_wall_start_y() {
	int posy = appArgs.pConfig->SCREEN_OFFSET_Y;
	return posy;
}

int get_wall_end_y() {
	int posy = LINES - get_wall_start_y() - 1;
	return posy;
}

List* create_wall()
{
	List* pList = create_list();
	if (pList == NULL)
	{
		error_message("ERROR: Can't initialize walls");
		longjmp(jmp_buffer10, 1);
	}
	
	if (setjmp(jmp_buffer7) != 1)
	{
		for (int x = 0 + get_wall_start_x(); x <= get_wall_end_x(); x++)
		{
			add_element_to_tail(pList, create_element(x, get_wall_start_y(), 7));
			add_element_to_tail(pList, create_element(x, get_wall_end_y(), 7));

		}
		for ( int y = 0 + get_wall_start_y(); y <= get_wall_end_y(); y++)
		{
			add_element_to_tail(pList, create_element(get_wall_start_x(), y, 7));
			add_element_to_tail(pList, create_element(get_wall_end_x(), y, 7));
		}
	}
	else
	{
		error_message("ERROR: Can't initialize walls");
		longjmp(jmp_buffer10, 1);
	}
	return pList;
}






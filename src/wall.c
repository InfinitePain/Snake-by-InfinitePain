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

jmp_buf jmp_buffer7;
extern jmp_buf jmp_buffer10;

int get_wall_start_x() {
	int posx = appArgs.pConfig->configs[SCREEN_OFFSET_X];
	return posx;
}

int get_wall_end_x() {
	int posx = appArgs.pConfig->configs[SCREEN_WIDTH]- 3 - get_wall_start_x();
	return posx;
}

int get_wall_start_y() {
	int posy = appArgs.pConfig->configs[SCREEN_OFFSET_Y];
	return posy;
}

int get_wall_end_y() {
	int posy = appArgs.pConfig->configs[SCREEN_HEIGHT]- 3 - get_wall_start_y();
	return posy;
}

List* create_wall()
{
	int x_start = get_wall_start_x();
	int x_end = get_wall_end_x();
	int y_start = get_wall_start_y();
	int y_end = get_wall_end_y();
	List* pList = create_list();
	if (pList == NULL)
	{
		error_message("ERROR: func create_wall(): create_list() failed");
		longjmp(jmp_buffer10, 1);
	}
	
	if (setjmp(jmp_buffer7) != 1)
	{
		for (int x = 0 + x_start; x <= x_end; x++)
		{
			add_element_to_tail(pList, create_element(x, y_start, 7));
			add_element_to_tail(pList, create_element(x, y_end, 7));

		}
		for ( int y = 0 + y_start; y <= y_end; y++)
		{
			add_element_to_tail(pList, create_element(x_start, y, 7));
			add_element_to_tail(pList, create_element(x_end, y, 7));
		}
	}
	else
	{
		error_message("ERROR: func create_wall(): create_element() failed");
		longjmp(jmp_buffer10, 1);
	}
	return pList;
}






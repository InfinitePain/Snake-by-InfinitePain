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
#include "app_status.h"

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
		GAME_STATE = CRITICAL_ERROR;
		return NULL;
	}

	Element* curr1, * curr2;
	for (int x = x_start; x < x_end; x++)
	{
		curr1 = create_element(x, y_start);
		curr2 = create_element(x, y_end - 1);
		if (curr1 == NULL || curr2 == NULL)
		{
			error_message("ERROR: func create_wall(): create_element() failed");
			GAME_STATE = CRITICAL_ERROR;
			return NULL;
		}
		add_element_to_head(pList, curr1);
		add_element_to_head(pList, curr2);

	}
	for (int y = y_start; y < y_end; y++)
	{
		curr1 = create_element(x_start, y);
		curr2 = create_element(x_end - 1, y);
		if (curr1 == NULL || curr2 == NULL)
		{
			error_message("ERROR: func create_wall(): create_element() failed");
			GAME_STATE = CRITICAL_ERROR;
			return NULL;
		}
		add_element_to_head(pList, curr1);
		add_element_to_head(pList, curr2);
	}
	return pList;
}

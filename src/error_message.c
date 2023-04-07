/*
 ============================================================================
 Name        : error_message.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
 ============================================================================
 */

#include <unistd.h>
#include <stdio.h>
#define NCURSES_STATIC
#include <ncurses.h>
#include <errno.h>
#include <string.h>

int error_line = 0;

void error_message(const char* message) {
	error_line == 0 ? clear() : 0;
	char buf[BUFSIZ];
	snprintf(buf, sizeof(buf), "%s: %s", message, strerror(errno));
	strerror(errno);
	mvprintw(error_line, 0, buf);
	refresh();
	error_line++;
	sleep(1);
}
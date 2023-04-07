/*
 ============================================================================
 Name        : error_message.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
 ============================================================================
 */

#ifndef ERROR_MESSAGE_H_
#define ERROR_MESSAGE_H_

#include <unistd.h>
#define NCURSES_STATIC
#include <ncurses.h>

extern int error_line;

void error_message(const char* message);

#endif /* ERROR_MESSAGE_H_ */
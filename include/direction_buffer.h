/*
 ============================================================================
 Name        : direction_buffer.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef DIRECTION_BUFFER_H_
#define DIRECTION_BUFFER_H_

#include <stdbool.h>
#define BUFFER_SIZE 4

typedef struct {
    int buffer[BUFFER_SIZE];
    int head;
    int tail;
} DirectionBuffer;

void buffer_init(DirectionBuffer* buffer);
bool buffer_is_empty(DirectionBuffer* buffer);
bool buffer_is_full(DirectionBuffer* buffer);
void buffer_push(DirectionBuffer* buffer, int direction);
int buffer_pop(DirectionBuffer* buffer);

#endif /*DIRECTION_BUFFER_H_*/
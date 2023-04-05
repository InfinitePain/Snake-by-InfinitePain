/*
 ============================================================================
 Name        : direction_buffer.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include "direction_buffer.h"

void buffer_init(DirectionBuffer *buffer) {
    buffer->head = 0;
    buffer->tail = 0;
}

bool buffer_is_empty(DirectionBuffer *buffer) {
    return buffer->head == buffer->tail;
}

bool buffer_is_full(DirectionBuffer *buffer) {
    return (buffer->head + 1) % BUFFER_SIZE == buffer->tail;
}

void buffer_push(DirectionBuffer *buffer, int direction) {
    if (!buffer_is_full(buffer)) {
        buffer->buffer[buffer->head] = direction;
        buffer->head = (buffer->head + 1) % BUFFER_SIZE;
    }
}

int buffer_pop(DirectionBuffer *buffer) {
    if (!buffer_is_empty(buffer)) {
        int direction = buffer->buffer[buffer->tail];
        buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
        return direction;
    }
    return -1;
}

int buffer_peek(DirectionBuffer* buffer) {
    if (buffer_is_empty(buffer)) {
        return -1;
    }
    return buffer->buffer[buffer->tail];
}

void buffer_clear(DirectionBuffer *buffer) {
    buffer->head = 0;
    buffer->tail = 0;
}

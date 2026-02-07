/*
 * RingBuffer.c
 *
 *  Created on: Dec 29, 2025
 *      Author: NGOC CAM
 */

#include "RingBuffer.h"

uint8_t ring_buffer_push(ring_buffer *ring_buff, uint8_t data)
{
    uint16_t next;
    next = ring_buff->head + 1;

    if (next >= ring_buff->maxlen)
    {
        next = 0;
    }
    if (next == ring_buff->tail) // ring buffer is full
    {
        return -1;
    }

    ring_buff->buffer[ring_buff->head] = data;
    ring_buff->head                    = next;
    return 0; // push successful
}

uint8_t ring_buffer_pop(ring_buffer *ring_buff, uint8_t *data)
{
    uint16_t next;
    next = ring_buff->tail + 1;

    if (ring_buff->head == ring_buff->tail) // don't have any data to pop
    {
        return -1;
    }
    if (next >= ring_buff->maxlen)
    {
        next = 0;
    }

    *data           = ring_buff->buffer[ring_buff->tail];
    ring_buff->tail = next;
    return 0;
}

void ring_buffer_init(ring_buffer *ring_buff, uint8_t *buff, uint16_t len)
{
    ring_buff->buffer = buff;
    ring_buff->head   = 0;
    ring_buff->tail   = 0;
    ring_buff->maxlen = len;
}

uint16_t ring_buffer_available(ring_buffer *ring_buff) // the number of data available to pop
{
    if (ring_buff->head < ring_buff->tail)
    {
        return ring_buff->maxlen - (ring_buff->tail - ring_buff->head);
    }
    return ring_buff->head - ring_buff->tail;
}

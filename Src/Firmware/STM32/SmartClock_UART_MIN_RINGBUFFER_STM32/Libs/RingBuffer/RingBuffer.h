#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

typedef struct {
	uint8_t *buffer;
	uint16_t head;
	uint16_t tail;
	uint16_t maxlen;
} ring_buffer;

void ring_buffer_init(ring_buffer *ring_buff, uint8_t *buff, uint16_t len);
uint8_t ring_buffer_push(ring_buffer *ring_buff, uint8_t data);
uint8_t ring_buffer_pop(ring_buffer *ring_buff, uint8_t *data);
uint16_t ring_buffer_available(ring_buffer *ring_buff); // the number of data available to pop

#endif

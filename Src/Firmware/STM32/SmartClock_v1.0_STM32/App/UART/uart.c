/*
 * uart.c
 *
 *  Created on: Dec 30, 2025
 *      Author: NGOC CAM
 */

#include "uart.h"
#include "RingBuffer.h"

#define RING_UART_LEN 64
uint8_t buff_uart[RING_UART_LEN];
ring_buffer ring_buff;

void uart_init(void)
{
	ring_buffer_init (&ring_buff, buff_uart, RING_UART_LEN);
}

void receive_rx(uint8_t data_rx)
{
	ring_buffer_push(&ring_buff, data_rx);
}

uint16_t uart_available()
{
	return ring_buffer_available(&ring_buff);
}

uint8_t uart_read()
{
	uint8_t data;
	ring_buffer_pop(&ring_buff, &data);
	return data;
}






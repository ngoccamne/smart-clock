/*
 * uart.h
 *
 *  Created on: Dec 30, 2025
 *      Author: NGOC CAM
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void uart_init(void);
void receive_rx(uint8_t data_rx);
uint16_t uart_available(); // the number of data available to pop
uint8_t uart_read();

#endif /* UART_H_ */

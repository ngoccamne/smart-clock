/*
 * ili9341_display.h
 *	Content:
 *  Created on: Jun 28, 2025
 *      Author: NGOC CAM
 */

#ifndef ILI9341_DISPLAY_H_
#define ILI9341_DISPLAY_H_

#include <main.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ILI9341_GFX.h"
#include "ILI9341_STM32_Driver.h"
#include "ds3231_i2c.h"

void ILI9341_DrawthickHline(uint16_t x, uint16_t y, uint16_t width, uint16_t color, uint8_t thickness);
void ILI9341_DrawthickVline(uint16_t x, uint16_t y, uint16_t height, uint16_t color, uint8_t thickness);
void send_time_to_ILI9341(uint8_t a, uint8_t b, uint8_t c, uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
void send_date_to_ILI9341(uint8_t a, uint8_t b, uint8_t c, uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
void send_day_to_ILI9341(uint8_t thu, uint16_t color, uint16_t bgcolor);
void ILI9341_DrawCursor(uint16_t x, uint16_t y, uint16_t size_x, uint16_t size_y, uint16_t color);
void ILI9341_move_cursor(uint16_t *x, uint16_t *y);
void draw_cursor_in_repeat_alarm(uint16_t x, uint16_t y, uint16_t size_x, uint16_t size_y, uint16_t color);
void move_cursor_down_in_repeat_alarm(uint16_t *x, uint16_t *y);
void move_cursor_up_in_repeat_alarm(uint16_t *x, uint16_t *y);
void adjust_time_up(uint8_t *gio, uint8_t *phut, uint8_t *giay, uint8_t *thu, uint8_t *ngay, uint8_t *thang, uint8_t *nam,
		            uint8_t x, uint8_t y);
void adjust_time_down(uint8_t *gio, uint8_t *phut, uint8_t *giay, uint8_t *thu, uint8_t *ngay, uint8_t *thang, uint8_t *nam,
		              uint8_t x, uint8_t y);
void select_unselect_the_day(uint8_t x_cursor, uint8_t y_cursor, uint8_t tick[], uint16_t remove_tick_x, uint16_t remove_tick_y);
void reset_in_set_repeat_alarm(uint8_t tick[], uint16_t remove_tick_x, uint16_t remove_tick_y);

#endif /* ILI9341_DISPLAY_H_ */

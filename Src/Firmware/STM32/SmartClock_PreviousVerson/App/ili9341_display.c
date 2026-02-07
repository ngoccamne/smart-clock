/*
 * ili9341_display.c
 *
 *  Created on: Jun 28, 2025
 *      Author: NGOC CAM
 */

#include "ili9341_display.h"

void ILI9341_DrawthickHline(uint16_t x, uint16_t y, uint16_t width, uint16_t color, uint8_t thickness)
{
	for (int i = 0; i < thickness; i++)
	{
		ILI9341_DrawHLine(x, y + i, width, color);
	}
}

void ILI9341_DrawthickVline(uint16_t x, uint16_t y, uint16_t height, uint16_t color, uint8_t thickness)
{
	for (int i = 0; i < thickness; i++)
	{
		ILI9341_DrawVLine(x + i, y, height, color);
	}
}

// GUI GIO, PHUT, GIAY DEN MAN HINH
void send_time_to_ILI9341(uint8_t a, uint8_t b, uint8_t c, uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor)
{
	char thoigian[100];
	sprintf(thoigian, "%d%d:%d%d:%d%d",
	  		a/10, a%10, b/10, b%10, c/10, c%10);
	ILI9341_DrawText(thoigian, FONT6, X, Y, color, bgcolor);
}

// GUI NGAY, THANG, NAM DEN MAN HINH
void send_date_to_ILI9341(uint8_t a, uint8_t b, uint8_t c, uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor)
{
	char thoigian[100];
	sprintf(thoigian, "%d%d/%d%d/%d%d",
	  		a/10, a%10, b/10, b%10, c/10, c%10);
	ILI9341_DrawText(thoigian, FONT6, X, Y, color, bgcolor);
}

// GUI THU DEN MAN HINH
void send_day_to_ILI9341(uint8_t thu, uint16_t color, uint16_t bgcolor)
{
	 char day[15];
	 ds3231_day_to_text(thu, day);
	 if (thu == 1)
	 {
		 ILI9341_DrawText(day, FONT6, 68, 195, color, bgcolor);
	 }
	 else if (thu == 2)
	 {
		 ILI9341_DrawText(day, FONT6, 65, 195, color, bgcolor);
	 }
	 else if (thu == 3)
	 {
		 ILI9341_DrawText(day, FONT6, 58, 195, color, bgcolor);
	 }
	 else if (thu == 4)
	 {
		 ILI9341_DrawText(day, FONT6, 27, 195, color, bgcolor);
	 }
	 else if (thu == 5)
	 {
		 ILI9341_DrawText(day, FONT6, 45, 195, color, bgcolor);
	 }
	 else if (thu == 6)
	 {
		 ILI9341_DrawText(day, FONT6, 72, 195, color, bgcolor);
	 }
	 else if (thu == 7)
	 {
		 ILI9341_DrawText(day, FONT6, 43, 195, color, bgcolor);
	 }
}

void ILI9341_DrawCursor(uint16_t x, uint16_t y, uint16_t size_x, uint16_t size_y, uint16_t color) {
	if (x == 80 && y == 218)
	{
		for (uint16_t i = 0; i < 92; i++)
		{
			for (uint16_t j = 0; j < 5; j++)
		    {
				ILI9341_DrawPixel(x + i, y + j, color);
		    }
		}
	}
	else if (x == 105 && y == 293)
	{
		for (uint16_t i = 0; i < 45; i++)
		{
			for (uint16_t j = 0; j < 5; j++)
		    {
				ILI9341_DrawPixel(x + i, y + j, color);
		    }
		}
	}
	else
	{
		for (uint16_t i = 0; i < size_x; i++)
		{
			for (uint16_t j = 0; j < size_y; j++)
			{
				ILI9341_DrawPixel(x + i, y + j, color);
			}
		}
	}
}

void ILI9341_move_cursor(uint16_t *x, uint16_t *y)
{
	if (*y == 120)
	{
		if (*x == 68 || *x == 109 || *x == 150)
		{
			*x = *x + 18;
		}
		else if (*x == 86 || *x == 127)
		{
			*x = *x + 23;
		}
		else if (*x == 168)
		{
			*x = *x + 1;
		}
	 }
	 else if (*y == 240)
	 {
		 if (*x == 68 || *x == 112 || *x == 156)
		 {
			 *x = *x + 18;
		 }
		 else if (*x == 86 || *x == 130)
		 {
			 *x = *x + 26;
		 }
		 else if (*x == 174)
		 {
			 *x = *x + 2;
		 }
	  }

	  if (*x > 168 && *y == 120)
	  {
		  *x = 80;
		  *y = 218;
	  }
	  else if (*x == 80 && *y == 218)
	  {
		  *x = 68;
		  *y = 240;
	  }
	  else if (*x > 175 && *y == 240)
	  {
		  *x = 105;
	  	  *y = 293;
	  }
	  else if (*x == 105 && *y == 293)
	  {
		  *x = 68;
		  *y = 120;
	  }
}

void draw_cursor_in_repeat_alarm(uint16_t x, uint16_t y, uint16_t size_x, uint16_t size_y, uint16_t color)
{
	for (uint16_t i = 0; i < size_x + 1; i++)
			{
				for (uint16_t j = 0; j < size_y + 1; j++)
				{
					ILI9341_DrawPixel(x + i, y + j, color);
				}
			}
}

void move_cursor_down_in_repeat_alarm(uint16_t *x, uint16_t *y)
{
	draw_cursor_in_repeat_alarm(*x, *y, 92, 3, WHITE);
	if (*y == 269)
	{
		*y = 119;
	}
	else
	{
		*y = *y + 25;
	}
}

void move_cursor_up_in_repeat_alarm(uint16_t *x, uint16_t *y)
{
	draw_cursor_in_repeat_alarm(*x, *y, 92, 3, WHITE);
	if (*y == 119)
	{
		*y = 269;
	}
	else
	{
		*y = *y - 25;
	}
}

void adjust_time_up(uint8_t *gio, uint8_t *phut, uint8_t *giay, uint8_t *thu, uint8_t *ngay, uint8_t *thang, uint8_t *nam,
		         uint8_t x, uint8_t y)
{
	  			  if (x == 68 && y == 120)
	  			  {
	  				  if (*gio/10 == 0 || *gio/10 == 1)
	  				  {
	  					  *gio = *gio + 10;
	  				  }
	  				  else if (*gio/10 == 2)
	  				  {
	  					  *gio = *gio - 20;
	  				  }
	  			  }
	  			  if (x == 86 && y == 120)
	  			  {
	  				  *gio = (*gio + 1)%24;
	  			  }
	  			  if (x == 109 && y == 120)
	  			  {
	  				  if (*phut/10 == 0 || *phut/10 == 1 || *phut/10 == 2 || *phut/10 == 3 || *phut/10 == 4)
	  				  {
	  				  *phut = *phut + 10;
	  				  }
	  				  else if (*phut/10 == 5)
	  				  {
	  					  *phut = *phut - 50;
	  				  }
	  			  }
	  			  if (x == 127 && y == 120)
	  			  {
	  				  *phut = (*phut + 1)%60;
	  			  }
	  			  if (x == 150 && y == 120)
	  			  {
	  				  if (*giay/10 == 0 || *giay/10 == 1 || *giay/10 == 2 || *giay/10 == 3 || *giay/10 == 4)
	  				  {
	  					  *giay = *giay + 10;
	  				  }
	  				  else if (*giay/10 == 5)
	  				  {
	  					  *giay = *giay - 50;
	  				  }
	  			  }
	  			  if (x == 168 && y == 120)
	  			  {
	  				  *giay = (*giay + 1)%60;
	  			  }
	  			  if (y == 218)
	  			  {
	  				  *thu = (*thu%7) + 1;
	  			  }
	  			  if (x == 68 && y == 240)
	  			  {
	  				  if (*ngay/10 == 0 || *ngay/10 == 1 || *ngay/10 == 2)
	  				  {
	  					  *ngay = *ngay + 10;
	  				  }
	  				  else if (*ngay/10 == 3)
	  				  {
	  					  *ngay = *ngay - 30;
	  				  }
	  			  }
	  			  if (x == 86 && y == 240)
	  			  {
	  				  *ngay = (*ngay%31) + 1;
	  			  }
	  			  if (x == 112 && y == 240)
	  			  {
	  				  if (*thang/10 == 0)
	  				  {
	  					  *thang = *thang + 10;
	  				  }
	  				  else if (*thang/10 == 1)
	  				  {
	  					  *thang = *thang - 10;
	  				  }
	  			  }
	  			  if (x == 130 && y == 240)
	  			  {
	  				  *thang = (*thang%12) + 1;
	  			  }
	  			  if (x == 156 && y == 240)
	  			  {
	  				  if (*nam/10 == 0 || *nam/10 == 1 || *nam/10 == 2 || *nam/10 == 3 || *nam/10 == 4 || *nam/10 == 5 || *nam/10 == 6 || *nam/10 == 7 || *nam/10 == 8)
	  				  {
	  					  *nam = *nam + 10;
	  				  }
	  				  else if (*nam/10 == 9)
	  				  {
	  					  *nam = *nam - 90;
	  				  }
	  			  }
	  			  if (x == 174 && y == 240)
	  			  {
	  				  *nam = (*nam + 1)%100;
	  			  }
}

void adjust_time_down(uint8_t *gio, uint8_t *phut, uint8_t *giay, uint8_t *thu, uint8_t *ngay, uint8_t *thang, uint8_t *nam,
		         uint8_t x, uint8_t y)
{
	  			  if (x == 68 && y == 120)
	  			  {
	  				  if (*gio/10 == 0 || *gio/10 == 1)
	  				  {
	  					  *gio = *gio - 10;
	  				  }
	  				  else if (*gio/10 == 2)
	  				  {
	  					  *gio = *gio + 20;
	  				  }
	  			  }
	  			  if (x == 86 && y == 120)
	  			  {
	  				  *gio = (*gio - 1)%24;
	  			  }
	  			  if (x == 109 && y == 120)
	  			  {
	  				  if (*phut/10 == 0 || *phut/10 == 1 || *phut/10 == 2 || *phut/10 == 3 || *phut/10 == 4)
	  				  {
	  				  *phut = *phut - 10;
	  				  }
	  				  else if (*phut/10 == 5)
	  				  {
	  					  *phut = *phut + 50;
	  				  }
	  			  }
	  			  if (x == 127 && y == 120)
	  			  {
	  				  *phut = (*phut - 1)%60;
	  			  }
	  			  if (x == 150 && y == 120)
	  			  {
	  				  if (*giay/10 == 0 || *giay/10 == 1 || *giay/10 == 2 || *giay/10 == 3 || *giay/10 == 4)
	  				  {
	  					  *giay = *giay - 10;
	  				  }
	  				  else if (*giay/10 == 5)
	  				  {
	  					  *giay = *giay + 50;
	  				  }
	  			  }
	  			  if (x == 168 && y == 120)
	  			  {
	  				  *giay = (*giay - 1)%60;
	  			  }
	  			  if (y == 218)
	  			  {
	  				  *thu = (*thu%7) - 1;
	  			  }
	  			  if (x == 68 && y == 240)
	  			  {
	  				  if (*ngay/10 == 0 || *ngay/10 == 1 || *ngay/10 == 2)
	  				  {
	  					  *ngay = *ngay - 10;
	  				  }
	  				  else if (*ngay/10 == 3)
	  				  {
	  					  *ngay = *ngay + 30;
	  				  }
	  			  }
	  			  if (x == 86 && y == 240)
	  			  {
	  				  *ngay = (*ngay%31) - 1;
	  			  }
	  			  if (x == 112 && y == 240)
	  			  {
	  				  if (*thang/10 == 0)
	  				  {
	  					  *thang = *thang - 10;
	  				  }
	  				  else if (*thang/10 == 1)
	  				  {
	  					  *thang = *thang + 10;
	  				  }
	  			  }
	  			  if (x == 130 && y == 240)
	  			  {
	  				  *thang = (*thang%12) - 1;
	  			  }
	  			  if (x == 156 && y == 240)
	  			  {
	  				  if (*nam/10 == 0 || *nam/10 == 1 || *nam/10 == 2 || *nam/10 == 3 || *nam/10 == 4 || *nam/10 == 5 || *nam/10 == 6 || *nam/10 == 7 || *nam/10 == 8)
	  				  {
	  					  *nam = *nam - 10;
	  				  }
	  				  else if (*nam/10 == 9)
	  				  {
	  					  *nam = *nam + 90;
	  				  }
	  			  }
	  			  if (x == 174 && y == 240)
	  			  {
	  				  *nam = (*nam - 1)%100;
	  			  }
}

void select_unselect_the_day(uint8_t x_cursor, uint8_t y_cursor, uint8_t tick[], uint16_t remove_tick_x, uint16_t remove_tick_y)
{
		//MONDAY
		if (y_cursor == 119)
		{
			if (tick[2] == 0)
			{
				ILI9341_DrawText("*", FONT5, 180, 100, BLACK, WHITE);
				tick[2]=1;
			}
			else if (tick[2] == 1)
			{
				ILI9341_DrawRectangle(180, 100, remove_tick_x, remove_tick_y, WHITE);
				tick[2]=0;
			}
		}
		//TUESDAY
		else if (y_cursor == 144)
		{
			if (tick[3] == 0)
			{
				ILI9341_DrawText("*", FONT5, 186, 125, BLACK, WHITE);
				tick[3]=1;
			}
			else if (tick[3] == 1)
			{
				ILI9341_DrawRectangle(186, 125, remove_tick_x, remove_tick_y, WHITE);
				tick[3]=0;
			}
		}
		//WEDNESDAY
		else if (y_cursor == 169)
		{
			if (tick[4] == 0)
			{
				ILI9341_DrawText("*", FONT5, 199, 150, BLACK, WHITE);
				tick[4]=1;
			}
			else if (tick[4] == 1)
			{
				ILI9341_DrawRectangle(199, 150, remove_tick_x, remove_tick_y, WHITE);
				tick[4]=0;
			}
		}
		//THURSDAY
		else if (y_cursor == 194)
		{
			if (tick[5] == 0)
			{
				ILI9341_DrawText("*", FONT5, 197, 175, BLACK, WHITE);
				tick[5]=1;
			}
			else if (tick[5] == 1)
		    {
				ILI9341_DrawRectangle(197, 175, remove_tick_x, remove_tick_y, WHITE);
				tick[5]=0;
		    }
		}
		//FRIDAY
		else if (y_cursor == 219)
		{
			if (tick[6] == 0)
			{
				ILI9341_DrawText("*", FONT5, 175, 200, BLACK, WHITE);
				tick[6]=1;
			}
			else if (tick[6] == 1)
			{
				ILI9341_DrawRectangle(175, 200, remove_tick_x, remove_tick_y, WHITE);
				tick[6]=0;
			}
		}
		//SATURDAY
		else if (y_cursor == 244)
		{
			if (tick[7] == 0)
			{
				ILI9341_DrawText("*", FONT5, 195, 225, BLACK, WHITE);
				tick[7]=1;
			}
			else if (tick[7] == 1)
			{
				ILI9341_DrawRectangle(195, 225, remove_tick_x, remove_tick_y, WHITE);
				tick[7]=0;
			}
		}
		//SUNDAY
		else if (y_cursor == 269)
		{
			if (tick[8] == 0)
			{
				ILI9341_DrawText("*", FONT5, 178, 250, BLACK, WHITE);
				tick[8]=1;
			}
			else if (tick[8] == 1)
			{
				ILI9341_DrawRectangle(178, 250, remove_tick_x, remove_tick_y, WHITE);
				tick[8]=0;
			}
		}
}

void reset_in_set_repeat_alarm(uint8_t tick[], uint16_t remove_tick_x, uint16_t remove_tick_y)
{
	for (int i = 0; i < 9; i++)
	{
		 tick[i] = 0;
	}
	ILI9341_DrawRectangle(180, 100, remove_tick_x, remove_tick_y, WHITE);
	ILI9341_DrawRectangle(186, 125, remove_tick_x, remove_tick_y, WHITE);
	ILI9341_DrawRectangle(199, 150, remove_tick_x, remove_tick_y, WHITE);
	ILI9341_DrawRectangle(197, 175, remove_tick_x, remove_tick_y, WHITE);
	ILI9341_DrawRectangle(175, 200, remove_tick_x, remove_tick_y, WHITE);
	ILI9341_DrawRectangle(195, 225, remove_tick_x, remove_tick_y, WHITE);
	ILI9341_DrawRectangle(178, 250, remove_tick_x, remove_tick_y, WHITE);
}

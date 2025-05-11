/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdlib.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include <stdint.h>
#include "ds3231_i2c.h"





/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
DS3231_I2C_HandleTypeDef p_DS3231;

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

void move_cursor_in_repeat_alarm(uint16_t *x, uint16_t *y)
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

void adjust_time(uint8_t *gio, uint8_t *phut, uint8_t *giay, uint8_t *thu, uint8_t *ngay, uint8_t *thang, uint8_t *nam,
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

#define SELECT 1
#define UNSELECT 0
uint8_t remove_tick_x = 14;
uint8_t remove_tick_y = 10;
uint8_t tick[8]={0};
void select_unselect_the_day(uint8_t x_cursor, uint8_t y_cursor)
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

void reset_in_set_repeat_alarm(void)
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
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

typedef enum {
	TIME_DISPLAY,
	SET_ALARM,
	SET_TIME,
	ALARM,
	SET_REPEAT_ALARM
} clock_state;

typedef enum {
	BUTTON_OFF,
	BUTTON_ON
} button_state;

#define	BUZZER_ON 0
#define BUZZER_OFF 1
#define SELECT 1
#define UNSELECT 0
#define CURSOR_SIZE_X 15
#define CURSOR_SIZE_Y 23

typedef struct
{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}Time;

#define ON 1
#define OFF 0
Time time_now;
Time time_alarm;
Time time_adjust;

uint8_t updatetime;
uint8_t clear;
uint16_t x_cursor;
uint16_t y_cursor;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_USB_DEVICE_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  ILI9341_Init();
  ILI9341_FillScreen(WHITE);

  ds3231_i2c_init(&p_DS3231, &hi2c1, 0x68);


    time_now.hour = 10;
    time_now.minute = 30;
    time_now.second = 30;
    time_now.day = 1;
    time_now.date = 20;
    time_now.month = 02;
    time_now.year = 25;
    ds3231_set_time(&p_DS3231, time_now.hour, time_now.minute, time_now.second, time_now.day,
  		                     time_now.date, time_now.month, time_now.year);

    time_alarm.hour = 00;
    time_alarm.minute = 00;
    time_alarm.second = 00;
    time_alarm.day = 1;
    time_alarm.date = 00;
    time_alarm.month = 00;
    time_alarm.year = 00;

    time_adjust.hour = 00;
        time_adjust.minute = 00;
        time_adjust.second = 00;
        time_adjust.day = 1;
        time_adjust.date = 00;
        time_adjust.month = 00;
        time_adjust.year = 00;

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, BUZZER_OFF);

    clock_state mode = TIME_DISPLAY;
    clear = ON;



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  button_state modebutton = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
	 	  	  		    	if (modebutton == BUTTON_ON)
	 	  	  		    	{
	 	  	  		    		if (mode == TIME_DISPLAY)
	 	  	  		    	  	{
	 	  	  		    			mode = SET_ALARM;
	 	  	  		    			clear = ON;
	 	  	  		    			x_cursor = 68;
	 	  	  		    			y_cursor = 120;
	 	  	  		    	  	}
	 	  	  		    		else if (mode == SET_ALARM)
	 	  	  		    	    {
	 	  	  		    	  		mode = SET_TIME;
	 	  	  		    	  		updatetime = ON;
	 	  	  		    	  		clear = ON;
	 	  	  		    	  	    x_cursor = 68;
	 	  	  		    	  		y_cursor = 120;
	 	  	  		    	  	}
	 	  	  		    		else if (mode == SET_TIME)
	 	  	  		    	  	{
	 	  	  		    	  		mode = TIME_DISPLAY;
	 	  	  		    	  		clear = ON;
	 	  	  		    	  	}
	 	  	  		    	 }

	 switch (mode)
	 {
	 	 case TIME_DISPLAY:

	 		 ds3231_read_time(&p_DS3231, &time_now.hour, &time_now.minute, &time_now.second,
	 	  	  		  	  	  &time_now.day, &time_now.date, &time_now.month, &time_now.year);

	 		 if (clear == ON)
	 		 {
	 			 ILI9341_FillScreen(WHITE);
	 			 //VE KHUNG
	 			 ILI9341_DrawthickHline(15, 47, 212, NAVY, 5);   // Canh tren
	 			 ILI9341_DrawthickHline(15, 280, 212, NAVY, 5); // Canh duoi
	 			 ILI9341_DrawthickVline(15, 47, 233, RED, 5);   // Canh ben trai
	 			 ILI9341_DrawthickVline(223, 47, 238, NAVY, 5); // Canh ben phai
	 			 ILI9341_DrawthickHline(15, 165, 212, NAVY, 5); // Canh o giua

	 			 ILI9341_DrawText("TIME", FONT6, 89, 77, RED, WHITE);
	 			 clear = OFF;
	 		 }
	 		 send_time_to_ILI9341(time_now.hour, time_now.minute, time_now.second, 68, 120, RED, WHITE);
	 		 send_date_to_ILI9341(time_now.date, time_now.month, time_now.year, 68, 240, RED, WHITE);
	 		 ILI9341_DrawRectangle(23, 195, 195, 23, WHITE);
	 		 send_day_to_ILI9341(time_now.day, RED, WHITE);


	 		 HAL_Delay(1000);

	      //ALARM STATE
	 		 if
			 (
					 time_now.second == time_alarm.second &&
					 time_now.minute == time_alarm.minute &&
					 time_now.hour == time_alarm.hour &&
					 time_now.day == time_alarm.day &&
					 time_now.date == time_alarm.date &&
					 time_now.month == time_alarm.month &&
					 time_now.year == time_alarm.year
			 )
	 		 {
	 			 	 mode = ALARM;
	 			 	 clear = ON;
	 		 }
	 		 else if
			 (
					 time_now.second == time_alarm.second &&
					 time_now.minute == time_alarm.minute &&
					 time_now.hour == time_alarm.hour
			 )
	 		 {
	 			 if (tick[8] == 1 && time_now.day == 1)
	 			 {
	 				 mode = ALARM;
	 				 clear = ON;
	 			 }
	 			 else if (tick[2] == 1 && time_now.day == 2)
	 			 {
	 				 mode = ALARM;
	 			 }
	 			 else if (tick[3] == 1 && time_now.day == 3)
	 			 {
	 				 mode = ALARM;
	 			 }
	 			 else if (tick[4] == 1 && time_now.day == 4)
	 			 {
	 				 mode = ALARM;
	 			 }
	 			 else if (tick[5] == 1 && time_now.day == 5)
	 			 {
	 				 mode = ALARM;
	 			 }
	 			 else if (tick[6] == 1 && time_now.day == 6)
	 			 {
	 				 mode = ALARM;
	 			 }
	 			 else if (tick[7] == 1 && time_now.day == 7)
	 			 {
	 				 mode = ALARM;
	 			 }
	 			 else
	 		     {
	 				 mode = TIME_DISPLAY;
	 			 }
	 		 }

	 		 break;

	 	 case SET_ALARM:

	 		if (clear == ON)
	 		{
	 		ILI9341_FillScreen(WHITE);
	 		//VE KHUNG
	 		ILI9341_DrawthickHline(15, 47, 212, NAVY, 5);   // Canh tren
	 		ILI9341_DrawthickHline(15, 280, 212, NAVY, 5); // Canh duoi
	 		ILI9341_DrawthickVline(15, 47, 233, NAVY, 5);   // Canh ben trai
	 		ILI9341_DrawthickVline(223, 47, 238, NAVY, 5); // Canh ben phai
	 		ILI9341_DrawthickHline(15, 165, 212, NAVY, 5); // Canh o giua

	 		ILI9341_DrawText("SET ALARM", FONT6, 32, 77, RED, WHITE);
	 		clear = OFF;
	 		}

	 		send_time_to_ILI9341(time_alarm.hour, time_alarm.minute, time_alarm.second, 68, 120, RED, WHITE);
	 		send_date_to_ILI9341(time_alarm.date, time_alarm.month, time_alarm.year, 68, 240, RED, WHITE);
	 		ILI9341_DrawRectangle(23, 195, 195, 23, WHITE);
	 		send_day_to_ILI9341(time_alarm.day, RED, WHITE);

	 		button_state set_alarm_cursor = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
	 		if(set_alarm_cursor == BUTTON_ON)
	 		{
	 			ILI9341_move_cursor(&x_cursor, &y_cursor);
	 		}

	 		ILI9341_DrawCursor(x_cursor, y_cursor, CURSOR_SIZE_X, CURSOR_SIZE_Y, GREEN);

	 		button_state adjustbutton = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
	 		if(adjustbutton == BUTTON_ON)
	 		{
	 			adjust_time(&time_alarm.hour, &time_alarm.minute, &time_alarm.second, &time_alarm.day,
	 				  	  	&time_alarm.date, &time_alarm.month, &time_alarm.year, x_cursor, y_cursor);
	 		}


	 		button_state resetbutton = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
	 		if (resetbutton == BUTTON_ON)
	 		{
	 			ds3231_read_time(&p_DS3231, &time_now.hour, &time_now.minute, &time_now.second,
	 			  	         	 	 	 	&time_now.day, &time_now.date, &time_now.month, &time_now.year);
	 			time_alarm.second = time_now.second;
	 			time_alarm.minute = time_now.minute;
	 			time_alarm.hour = time_now.hour;
	 			time_alarm.day = time_now.day;
	 			time_alarm.date = time_now.date;
	 			time_alarm.month = time_now.month;
	 			time_alarm.year = time_now.year;
	 		}

	 		button_state enterbutton = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
	 		if (enterbutton == BUTTON_ON)
	 		{
	 			mode = SET_REPEAT_ALARM;
	 			clear = ON;
	 			x_cursor = 78;
	 			y_cursor = 119;
	 			HAL_Delay(200);
	 		}

	 		HAL_Delay(1000);
	 		break;

	 	 case SET_REPEAT_ALARM:
	 		//clear = ON;
	 		if (clear == ON)
	 		{
	 			x_cursor = 78;
	 			y_cursor = 119;
	 			ILI9341_FillScreen(WHITE);
	 			//VE KHUNG
	 			ILI9341_DrawthickHline(13, 47, 212, NAVY, 5);   // Canh tren
	 			ILI9341_DrawthickHline(13, 280, 212, NAVY, 5); // Canh duoi
	 			ILI9341_DrawthickVline(13, 47, 233, NAVY, 5);   // Canh ben trai
	 			ILI9341_DrawthickVline(223, 47, 238, NAVY, 5); // Canh ben phai
	 			ILI9341_DrawText("REPEAT", FONT6, 64, 65, RED, WHITE);
	 			ILI9341_DrawText("MONDAY", FONT5, 60, 100, RED, WHITE);
	 			ILI9341_DrawText("TUESDAY", FONT5, 55, 125, RED, WHITE);
	 			ILI9341_DrawText("WEDNESDAY", FONT5, 19, 150, RED, WHITE);
	 			ILI9341_DrawText("THURSDAY", FONT5, 45, 175, RED, WHITE);
	 			ILI9341_DrawText("FRIDAY", FONT5, 67, 200, RED, WHITE);
	 			ILI9341_DrawText("SATURDAY", FONT5, 49, 225, RED, WHITE);
	 			ILI9341_DrawText("SUNDAY", FONT5, 66, 250, RED, WHITE);

	 			clear = OFF;
	 		}

	 		button_state set_repeat_alarm_cursor = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
	 			 		if(set_repeat_alarm_cursor == BUTTON_ON)
	 			 		{
	 			 			move_cursor_in_repeat_alarm(&x_cursor, &y_cursor);
	 			 		}

	 		draw_cursor_in_repeat_alarm(x_cursor, y_cursor, 92, 3, GREEN);

	 		button_state selectbutton = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
	 			if (selectbutton == BUTTON_ON)
	 			{
	 				select_unselect_the_day(x_cursor, y_cursor);
	 			}

	 		resetbutton = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
	 		if (resetbutton == BUTTON_ON)
	 		{
	 			reset_in_set_repeat_alarm();
	 		}

	 		enterbutton = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
	 		if (enterbutton == BUTTON_ON)
	 		{
	 			mode = SET_TIME;
	 			updatetime = ON;
	 			clear = ON;
	 			x_cursor = 68;
	 			y_cursor = 120;
	 		}

	 		HAL_Delay(1000);
	 		break;

	 	 case SET_TIME:

	 		if (clear == ON)
	 		{
	 		ILI9341_FillScreen(WHITE);
	 		//VE KHUNG
	 		ILI9341_DrawthickHline(15, 47, 212, NAVY, 5);   // Canh tren
	 		ILI9341_DrawthickHline(15, 280, 212, NAVY, 5); // Canh duoi
	 		ILI9341_DrawthickVline(15, 47, 233, NAVY, 5);   // Canh ben trai
	 		ILI9341_DrawthickVline(223, 47, 238, NAVY, 5); // Canh ben phai
	 		ILI9341_DrawthickHline(15, 165, 212, NAVY, 5); // Canh o giua

	 		ILI9341_DrawText("SET TIME", FONT6, 47, 77, RED, WHITE);
	 		clear = OFF;
	 		}

	 		/*if (updatetime == ON)
	 		{
	 			ds3231_read_time(&p_DS3231, &time_adjust.hour, &time_adjust.minute, &time_adjust.second,
	 			  				 &time_adjust.day, &time_adjust.date, &time_adjust.month, &time_adjust.year);
	 			updatetime = OFF;
	 		}*/

	 		send_time_to_ILI9341(time_adjust.hour, time_adjust.minute, time_adjust.second, 68, 120, RED, WHITE);
	 		send_date_to_ILI9341(time_adjust.date, time_adjust.month, time_adjust.year, 68, 240, RED, WHITE);
	 		ILI9341_DrawRectangle(23, 195, 195, 23, WHITE);
	 		send_day_to_ILI9341(time_adjust.day, RED, WHITE);

	 		button_state set_time_cursor = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
	 		if(set_time_cursor == BUTTON_ON)
	 		{
	 			ILI9341_move_cursor(&x_cursor, &y_cursor);
	 		}

	 		ILI9341_DrawCursor(x_cursor, y_cursor, CURSOR_SIZE_X, CURSOR_SIZE_Y, GREEN);

	 		adjustbutton = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
	 		if(adjustbutton == BUTTON_ON)
	 		{
	 			adjust_time(&time_adjust.hour, &time_adjust.minute, &time_adjust.second, &time_adjust.day,
	 			 			&time_adjust.date, &time_adjust.month, &time_adjust.year, x_cursor, y_cursor);
	 		}


	 		resetbutton = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
	 		if (resetbutton == BUTTON_ON)
	 		{
	 			ds3231_read_time(&p_DS3231, &time_now.hour, &time_now.minute, &time_now.second,
	 			 			  	         	&time_now.day, &time_now.date, &time_now.month, &time_now.year);
	 			time_adjust.second = time_now.second;
	 			time_adjust.minute = time_now.minute;
	 			time_adjust.hour = time_now.hour;
	 			time_adjust.day = time_now.day;
	 			time_adjust.date = time_now.date;
	 			time_adjust.month = time_now.month;
	 			time_adjust.year = time_now.year;
	 		}

	 		enterbutton = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
	 		if (enterbutton == BUTTON_ON)
	 		{
	 			ds3231_set_time(&p_DS3231, time_adjust.hour, time_adjust.minute, time_adjust.second,
	 			  			               time_adjust.day, time_adjust.date, time_adjust.month, time_adjust.year);
	 			mode = TIME_DISPLAY;
	 			clear = ON;
	 		}

	 		HAL_Delay(1000);
	 		break;
	 	 case ALARM:

	 		if (clear == ON)
	 		{
	 			ILI9341_FillScreen(WHITE);
	 			//VE KHUNG
	 			ILI9341_DrawthickHline(15, 47, 212, NAVY, 5);   // Canh tren
	 			ILI9341_DrawthickHline(15, 280, 212, NAVY, 5); // Canh duoi
	 			ILI9341_DrawthickVline(15, 47, 233, NAVY, 5);   // Canh ben trai
	 			ILI9341_DrawthickVline(223, 47, 238, NAVY, 5); // Canh ben phai
	 			ILI9341_DrawText("WAKE UP!", FONT6, 44, 155, RED, WHITE);
	 			clear = OFF;
	 		}

	 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, BUZZER_ON);

	 		button_state alarmbutton = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
	 			if(alarmbutton == BUTTON_ON)
	 			{
	 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, BUZZER_OFF);
	 			mode = TIME_DISPLAY;
	 			clear = ON;
	 			}
	 			HAL_Delay(1000);
	 		break;
	 }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|DHT11_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin|LCD_TCS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_DC_Pin|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PC13 DHT11_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_13|DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RST_Pin LCD_TCS_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_TCS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_DC_Pin PB11 LCD_CS_Pin */
  GPIO_InitStruct.Pin = LCD_DC_Pin|GPIO_PIN_11|LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

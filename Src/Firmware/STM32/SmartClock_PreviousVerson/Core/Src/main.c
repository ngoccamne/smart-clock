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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Delay_us.h"
#include "DHT.h"
#include "ds3231_i2c.h"
#include "pir_am312.h"
#include "ili9341_display.h"
#include "ILI9341_GFX.h"
#include "ILI9341_STM32_Driver.h"
//#include "ili9341_touch.h"
//#include "touch.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SELECT 				1
#define UNSELECT 			0
#define	BUZZER_ON  			0
#define BUZZER_OFF 			1
#define CURSOR_SIZE_X 		15
#define CURSOR_SIZE_Y 		23
#define ON 					1
#define OFF 				0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_tx;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
DS3231_I2C_HandleTypeDef p_DS3231;
DHT_HandleTypeDef hdht;

typedef enum {
	TIME_DISPLAY,
	SET_ALARM,
	SET_TIME,
	ALARM,
	SET_REPEAT_ALARM,
	MODE_MENU
} display_mode;

typedef enum {
	BUTTON_ON,
	BUTTON_OFF
} button_state;

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

Time time_now;
Time time_alarm;
Time time_adjust;

uint8_t updatetime;
uint8_t clear = ON;
uint16_t x_cursor;
uint16_t y_cursor;

uint8_t dht[5];
uint8_t hum;
uint8_t temp;

uint8_t remove_tick_x = 14;
uint8_t remove_tick_y = 10;
uint8_t tick[8]={0};

uint32_t blink_delay = 200;  //Default delay

// Bien dem cho Rotary Encoder
uint32_t rawCounter = 0;
uint16_t encoder_pos = 0;
uint16_t pre_encoder_pos = 0;

display_mode mode = TIME_DISPLAY;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	rawCounter = __HAL_TIM_GET_COUNTER(htim);
	encoder_pos  = (rawCounter/2)%21;  // Moi lan xoay tang 2 don vi; Dem tu 0-20
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == SW_Pin)
	{
	    if (HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin) == GPIO_PIN_RESET)
	    {
	    	if (mode == TIME_DISPLAY)
	    	{
	    		mode = MODE_MENU;
	    		clear = ON;
	    	}
	    	if (mode == MODE_MENU)
	    	{
	    		if (y_cursor == 119)
	    		{
	    			mode = TIME_DISPLAY;
	    			clear = ON;
	    		}
	    		if (y_cursor == 144)
	    		{
	    			mode = SET_ALARM;
	    			clear = ON;
	    			x_cursor = 68;
	    			y_cursor = 120;
//	    			updatetime = ON;
	    		}
	    		if (y_cursor == 169)
	    		{
	    			mode = SET_TIME;
	    			clear = ON;
	    		}
	    	}
	    	if (mode == SET_ALARM)
	    	{
	    		if (x_cursor == 105 && y_cursor == 293)
	    		{
	    			mode = SET_REPEAT_ALARM;
	    			clear = ON;
	    			HAL_Delay(200);
	    		}
	    		else
	    		{
	    			ILI9341_move_cursor(&x_cursor, &y_cursor);
	    		}
	    	}
	    	if (mode == SET_REPEAT_ALARM)
	    	{
	    		select_unselect_the_day(x_cursor, y_cursor, tick, remove_tick_x, remove_tick_y);
	    	}
	    }
	 }
}
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
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  ds3231_i2c_init(&p_DS3231, &hi2c1, 0x68);
  PIR_AM312_Init(PIR_GPIO_Port, PIR_Pin);
  DELAY_US_Init(&htim3);
  DHT_Init(&hdht, DHT11, DHT11_GPIO_Port, DHT11_Pin);


  // Start Timer in Encoder mode
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

  ILI9341_Init();
  ILI9341_FillScreen(WHITE);

  time_now.hour = 10;
  time_now.minute = 30;
  time_now.second = 30;
  time_now.day = 1;
  time_now.date = 20;
  time_now.month = 02;
  time_now.year = 25;

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

  ds3231_set_time(&p_DS3231, time_now.hour, time_now.minute, time_now.second, time_now.day,
     		                 time_now.date, time_now.month, time_now.year);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, BUZZER_OFF);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	// Ham doc Rotary Encoder
	HAL_TIM_IC_CaptureCallback(&htim2);

	HAL_GPIO_EXTI_Callback(SW_Pin);

//	  if ( pir_am312_get_data() == 1)
//	  {
//		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
//	  }
//	  else
//	  {
//		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
//	  }



	switch (mode)
    {
	 	case MODE_MENU:
	 		if (clear == ON)
	 		{
	 			 x_cursor = 75;
	 			 y_cursor = 119;
	 			 ILI9341_FillScreen(WHITE);
	 			 //VE KHUNG
	 			 ILI9341_DrawthickHline(13, 47, 212, NAVY, 5);   // Canh tren
	 			 ILI9341_DrawthickHline(13, 280, 212, NAVY, 5); // Canh duoi
	 			 ILI9341_DrawthickVline(13, 47, 233, NAVY, 5);   // Canh ben trai
	 			 ILI9341_DrawthickVline(223, 47, 238, NAVY, 5); // Canh ben phai
	 			 ILI9341_DrawText("MENU", FONT6, 78, 65, RED, WHITE);
	 			 ILI9341_DrawText("TIME", FONT5, 82, 100, RED, WHITE);
	 			 ILI9341_DrawText("SET_ALARM", FONT5, 34, 125, RED, WHITE);
	 			 ILI9341_DrawText("SET_TIME", FONT5, 42, 150, RED, WHITE);
	 			 ILI9341_DrawText("", FONT5, 45, 175, RED, WHITE);
	 			 ILI9341_DrawText("", FONT5, 67, 200, RED, WHITE);
	 			 ILI9341_DrawText("", FONT5, 49, 225, RED, WHITE);
	 			 ILI9341_DrawText("", FONT5, 66, 250, RED, WHITE);

	 			 clear = OFF;
	 		}

	 		if (encoder_pos == pre_encoder_pos + 1)
	 		{
	 			move_cursor_down_in_repeat_alarm(&x_cursor, &y_cursor);
	 		}
	 		else if (encoder_pos == pre_encoder_pos - 1)
	 		{
	 			move_cursor_up_in_repeat_alarm(&x_cursor, &y_cursor);
	 		}
	 		pre_encoder_pos = encoder_pos;

	 		draw_cursor_in_repeat_alarm(x_cursor, y_cursor, 92, 3, GREEN);

	 	break;
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

	 	break;

		case SET_ALARM:

			if (clear == ON)
			{
				ILI9341_FillScreen(WHITE);
				//VE KHUNG
				ILI9341_DrawthickHline(15, 47, 212, NAVY, 5);   // Canh tren
				ILI9341_DrawthickHline(15, 302, 212, NAVY, 5); // Canh duoi
				ILI9341_DrawthickVline(15, 47, 255, NAVY, 5);   // Canh ben trai
				ILI9341_DrawthickVline(223, 47, 260, NAVY, 5); // Canh ben phai
				ILI9341_DrawthickHline(15, 165, 212, NAVY, 5); // Canh o giua

				ILI9341_DrawText("SET ALARM", FONT6, 32, 77, RED, WHITE);
				clear = OFF;
			}

			send_time_to_ILI9341(time_alarm.hour, time_alarm.minute, time_alarm.second, 68, 120, RED, WHITE);
			send_date_to_ILI9341(time_alarm.date, time_alarm.month, time_alarm.year, 68, 240, RED, WHITE);
			ILI9341_DrawRectangle(23, 195, 195, 23, WHITE);
			send_day_to_ILI9341(time_alarm.day, RED, WHITE);
			ILI9341_DrawText("OK", FONT6, 105, 270, RED, WHITE);

			if (encoder_pos == pre_encoder_pos + 1)
			{
				adjust_time_up(&time_alarm.hour, &time_alarm.minute, &time_alarm.second, &time_alarm.day,
					    	&time_alarm.date, &time_alarm.month, &time_alarm.year, x_cursor, y_cursor);
			}

			if (encoder_pos == pre_encoder_pos - 1)
			{
				adjust_time_down(&time_alarm.hour, &time_alarm.minute, &time_alarm.second, &time_alarm.day,
							   &time_alarm.date, &time_alarm.month, &time_alarm.year, x_cursor, y_cursor);
			}

			ILI9341_DrawCursor(x_cursor, y_cursor, CURSOR_SIZE_X, CURSOR_SIZE_Y, GREEN);

			HAL_Delay(1000);
		break;

		case SET_REPEAT_ALARM:

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

			if (encoder_pos == pre_encoder_pos + 1)
			{
				move_cursor_down_in_repeat_alarm(&x_cursor, &y_cursor);
			}
			else if (encoder_pos == pre_encoder_pos - 1)
			{
				move_cursor_up_in_repeat_alarm(&x_cursor, &y_cursor);
			}
			pre_encoder_pos = encoder_pos;

			draw_cursor_in_repeat_alarm(x_cursor, y_cursor, 92, 3, GREEN);

//				 		enterbutton = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
//				 		if (enterbutton == BUTTON_ON)
//				 		{
//				 			mode = SET_TIME;
//				 			updatetime = ON;
//				 			clear = ON;
//				 			x_cursor = 68;
//				 			y_cursor = 120;
//				 		}

			 HAL_Delay(1000);

		break;

		case SET_TIME:

			ILI9341_FillScreen(WHITE);

		break;

		case ALARM:

			ILI9341_FillScreen(WHITE);

		break;
		}


	     DHT_Read_Value(&hdht);
	     hum = hdht.huminity;
	     temp = hdht.temperature;

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
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
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
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
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 71;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 71;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

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
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_TCS_GPIO_Port, LCD_TCS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_DC_Pin|LCD_RST_Pin|LCD_CS_Pin|DHT11_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SW_Pin */
  GPIO_InitStruct.Pin = SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SW_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_TCS_Pin */
  GPIO_InitStruct.Pin = LCD_TCS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_TCS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_DC_Pin LCD_RST_Pin LCD_CS_Pin DHT11_Pin */
  GPIO_InitStruct.Pin = LCD_DC_Pin|LCD_RST_Pin|LCD_CS_Pin|DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB8 PIR_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_8|PIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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

#include "DHT.h"
#include "Delay_us.h"

#define DHT11_STARTTIME 18000
#define DHT22_STARTTIME 12000

#define TIME_OUT		10

static void DHT_Set_Pin_Output(DHT_HandleTypeDef* p_DHT){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = p_DHT->DHT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(p_DHT->DHT_Port, &GPIO_InitStruct);
}

static void DHT_Set_Pin_Input(DHT_HandleTypeDef* p_DHT){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = p_DHT->DHT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(p_DHT->DHT_Port, &GPIO_InitStruct);
}

static void DHT_Start(DHT_HandleTypeDef* p_DHT){
	DHT_Set_Pin_Output(p_DHT);
	HAL_GPIO_WritePin(p_DHT->DHT_Port, p_DHT->DHT_Pin, 0);
	DELAY_US_Run((p_DHT->sensor_type == DHT11) ? DHT11_STARTTIME : DHT22_STARTTIME);
	HAL_GPIO_WritePin(p_DHT->DHT_Port, p_DHT->DHT_Pin, 1);
}

static uint8_t DHT_Check_Response(DHT_HandleTypeDef* p_DHT){
	DHT_Set_Pin_Input(p_DHT);
	DELAY_US_Run(40);
	if(!HAL_GPIO_ReadPin(p_DHT->DHT_Port, p_DHT->DHT_Pin)){
		DELAY_US_Run(80);
		if(!HAL_GPIO_ReadPin(p_DHT->DHT_Port, p_DHT->DHT_Pin)){
			return DHT_FAIL;
		}
	} else{
		return DHT_FAIL;
	}

	uint32_t timer_out = HAL_GetTick();
	while(HAL_GPIO_ReadPin(p_DHT->DHT_Port, p_DHT->DHT_Pin)){
		if(HAL_GetTick() - timer_out > TIME_OUT){
			return DHT_FAIL;
		}
	}

	return DHT_OK;
}

static uint8_t DHT_Read(DHT_HandleTypeDef* p_DHT, uint8_t p_data[5]){
	DHT_Set_Pin_Input(p_DHT);
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 8; j++)
		{
			uint32_t timer_out = HAL_GetTick();
			while(!HAL_GPIO_ReadPin(p_DHT->DHT_Port, p_DHT->DHT_Pin)){
				if(HAL_GetTick() - timer_out > TIME_OUT){
					return DHT_FAIL;
				}
			}
			DELAY_US_Run(40);
			if(!HAL_GPIO_ReadPin(p_DHT->DHT_Port, p_DHT->DHT_Pin))
			{
				*(p_data + i) &= ~(1 << (7-j));
			} else {
				*(p_data + i) |= (1 << (7-j));
			}
			timer_out = HAL_GetTick();
			while(HAL_GPIO_ReadPin(p_DHT->DHT_Port, p_DHT->DHT_Pin)){
				if(HAL_GetTick() - timer_out > TIME_OUT){
					return DHT_FAIL;
				}
			}
		}
	}
	return DHT_OK;
}

void DHT_Init(DHT_HandleTypeDef* p_DHT, uint8_t p_sensor_type, GPIO_TypeDef* p_Port, uint16_t p_Pin){
	p_DHT->sensor_type = p_sensor_type;
	p_DHT->DHT_Port = p_Port;
	p_DHT->DHT_Pin = p_Pin;
	p_DHT->temperature = 0.0;
	p_DHT->huminity = 0.0;
}

uint8_t DHT_Read_Value(DHT_HandleTypeDef* p_DHT){
	uint8_t t_data[5] = {};
	DHT_Start(p_DHT);
	if(DHT_Check_Response(p_DHT) == DHT_FAIL)
		return DHT_FAIL;
	if(DHT_Read(p_DHT, t_data) == DHT_FAIL)
		return DHT_FAIL;
	if(((t_data[0] + t_data[1] + t_data[2] + t_data[3]) & 0xFF) != t_data[4])
		return DHT_FAIL;

	if(t_data[0] < 100 && t_data[0] > 0)
		p_DHT->huminity = t_data[0] / 1.0;
	else
		return DHT_FAIL;

	if(t_data[2] < 100 && t_data[2] > 0)
		p_DHT->temperature = t_data[2] / 1.0;
	else
		return DHT_FAIL;

	return DHT_OK;
}

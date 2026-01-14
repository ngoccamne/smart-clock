#ifndef DHT11_H
#define DHT11_H

#include "main.h"
#include "stdint.h"

enum{
	DHT11,
	DHT22,
};

enum{
	DHT_FAIL,
	DHT_OK,
};

typedef struct{
	uint8_t sensor_type;
	GPIO_TypeDef *DHT_Port;
	uint16_t DHT_Pin;
	float temperature;
	float huminity;
} DHT_HandleTypeDef;

void DHT_Init(DHT_HandleTypeDef* p_DHT, uint8_t p_sensor_type, GPIO_TypeDef* p_Port, uint16_t p_Pin);
uint8_t DHT_Read_Value(DHT_HandleTypeDef* p_DHT);

#endif

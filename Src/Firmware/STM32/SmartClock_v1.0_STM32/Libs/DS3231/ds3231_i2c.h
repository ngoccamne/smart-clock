/*
 * ds3231_i2c.h
 *
 *  Created on: Feb 19, 2025
 *      Author: NGOC CAM
 */

#ifndef DS3231_I2C_H_
#define DS3231_I2C_H_

#include "main.h"

#include "stdio.h"
#include "stdint.h"
#include "string.h"

typedef struct
{
	uint8_t SLAVE_ADDRESS;
	I2C_HandleTypeDef *hi2c;
}DS3231_I2C_HandleTypeDef;

void ds3231_i2c_init(DS3231_I2C_HandleTypeDef *p_DS3231, I2C_HandleTypeDef *p_hi2c, uint8_t p_Slave_Address);
void ds3231_read_time(DS3231_I2C_HandleTypeDef *p_DS3231, uint8_t *hour, uint8_t *minute, uint8_t *second,
		              uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year); // read time from DS3231
void ds3231_set_time(DS3231_I2C_HandleTypeDef *p_DS3231, uint8_t hour, uint8_t minute, uint8_t second,
		             uint8_t day, uint8_t date, uint8_t month, uint8_t year);      // set time on DS3231
void ds3231_day_to_text(uint8_t day, char *day_to_text);             // convert from ds3231_day to text
uint8_t DecimalToBCD(uint8_t dec);                                   // convert from Decimal to BCD
uint8_t BCDToDecimal(uint8_t bcd);                                   // convert from BCD to Decimal

#endif /* DS3231_I2C_H_ */

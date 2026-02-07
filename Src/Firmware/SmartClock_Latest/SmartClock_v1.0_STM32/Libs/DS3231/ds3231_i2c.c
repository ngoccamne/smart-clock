/*
 * ds3231_i2c.c
 *
 *  Created on: Feb 19, 2025
 *      Author: NGOC CAM
 */

#include "ds3231_i2c.h"

void ds3231_i2c_init(DS3231_I2C_HandleTypeDef *p_DS3231, I2C_HandleTypeDef *p_hi2c, uint8_t p_Slave_Address)
{
    p_DS3231->SLAVE_ADDRESS = p_Slave_Address;
    p_DS3231->hi2c          = p_hi2c;
}

uint8_t DecimalToBCD(uint8_t dec)
{
    return ((dec / 10) << 4) | (dec % 10);
}

uint8_t BCDToDecimal(uint8_t bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void ds3231_day_to_text(uint8_t day, char *day_to_text)
{
    char *day_text[] = {"0", "CN", "T2", "T3", "T4", "T5", "T6", "T7"};
    strcpy(day_to_text, day_text[day]);
}
void ds3231_read_time(DS3231_I2C_HandleTypeDef *p_DS3231,
                      uint8_t                  *hour,
                      uint8_t                  *minute,
                      uint8_t                  *second,
                      uint8_t                  *day,
                      uint8_t                  *date,
                      uint8_t                  *month,
                      uint8_t                  *year)
{
    uint8_t data[7];

    HAL_I2C_Mem_Read(p_DS3231->hi2c, p_DS3231->SLAVE_ADDRESS << 1, 0X00, 1, data, 7, 100);

    *second = BCDToDecimal(data[0]);
    *minute = BCDToDecimal(data[1]);
    *hour   = BCDToDecimal(data[2]);
    *day    = BCDToDecimal(data[3]);
    *date   = BCDToDecimal(data[4]);
    *month  = BCDToDecimal(data[5]);
    *year   = BCDToDecimal(data[6]);
}

void ds3231_set_time(DS3231_I2C_HandleTypeDef *p_DS3231,
                     uint8_t                   hour,
                     uint8_t                   minute,
                     uint8_t                   second,
                     uint8_t                   day,
                     uint8_t                   date,
                     uint8_t                   month,
                     uint8_t                   year)
{
    uint8_t data[7];

    data[0] = DecimalToBCD(second);
    data[1] = DecimalToBCD(minute);
    data[2] = DecimalToBCD(hour);
    data[3] = DecimalToBCD(day);
    data[4] = DecimalToBCD(date);
    data[5] = DecimalToBCD(month);
    data[6] = DecimalToBCD(year);

    HAL_I2C_Mem_Write(p_DS3231->hi2c, p_DS3231->SLAVE_ADDRESS << 1, 0X00, 1, data, 7, 100);
}

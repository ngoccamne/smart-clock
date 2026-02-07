#ifndef MQTT_CLIENT_TCP_H_
#define MQTT_CLIENT_TCP_H_

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>
#include <stddef.h>
#include "mqtt_client.h"
#include "driver/gpio.h"

/**********************
 *       DEFINES
 **********************/

#define URI      "mqtts://c3b542cb563643909107a4fb1b8da0e0.s1.eu.hivemq.cloud:8883"
#define USERNAME "camne"
#define PASSWORD "Ngoccam123"

/**********************
 *      TYPEDEFS
 **********************/

typedef void (*mqtt_event_data)(char *data, int len);

/**********************
 *  EXTERN VARIABLES
 **********************/

extern char data_mqtt[10];

/**********************
 *      FUNCTIONS
 **********************/

esp_mqtt_client_handle_t mqtt_app_start(void);
void                     mqtt_led_stm32_callback(mqtt_event_data fc);

#endif

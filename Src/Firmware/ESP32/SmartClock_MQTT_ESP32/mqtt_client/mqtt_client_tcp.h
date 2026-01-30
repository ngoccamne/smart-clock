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

#define URI "mqtts://1e1754ba0fa642abb6b9b7b6d5e191a9.s1.eu.hivemq.cloud:8883"
#define USERNAME "camne"
#define PASSWORD "Ngoccam123"

#define LED_PIN 2

/**********************
 *      TYPEDEFS
 **********************/


/**********************
 *  EXTERN VARIABLES
 **********************/

extern char data_mqtt [10];

/**********************
 *      TYPEDEFS
 **********************/

esp_mqtt_client_handle_t mqtt_app_start(void);

#endif
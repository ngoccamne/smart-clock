#include <stdio.h>
#include <string.h>
#include "wifi.h"
#include "uart.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "mqtt.h"


static const char *TAG = "MQTT";

mqtt_client my_client;
void app_main(void)
{
    my_client.id = sock;
    mqtt_connect(&my_client, "ESP32_Test");
    mqtt_subscribe(&my_client, "smart_clock/led");

}

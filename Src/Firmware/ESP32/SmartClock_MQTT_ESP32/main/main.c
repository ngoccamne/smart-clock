/*********************
 *      INCLUDES
 *********************/

#include <stdio.h>
#include <string.h>
#include "mqtt_client_tcp.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "wifi.h"

/*********************
 *      DEFINES
 *********************/


/**********************
 *     VARIABLES
 **********************/

static const char *TAG = "MAIN_APP";

/**********************
 *  STATIC VARIABLES
 **********************/



/**********************
 *     FUNCTION
 **********************/



/********************
 *  MAIN FUNCTION
 ********************/

void app_main(void)
{
    // INIT
    nvs_flash_init();

    // CONNECT TO WIFI
    ESP_LOGI(TAG, "Start to connect Wifi...");
    wifi_init_sta();

    // MQTT 
    esp_mqtt_client_handle_t client = mqtt_app_start();


    // PUBLISH
    // char *topic = "smartclock/led";
    // char *data = "Hello from ESP32";

    // while (1) {
    //     int msg_id = esp_mqtt_client_publish(client, topic, data, 0, 1, 0);
    //     if (msg_id != -1) {
    //         ESP_LOGI(TAG, "Sent publish successful from main, msg_id=%d", msg_id);
    //     } else {
    //         ESP_LOGE(TAG, "Failed to publish (Client not connected yet?)");
    //     }
    //     vTaskDelay(pdMS_TO_TICKS(5000));
    // }
}


/********************
 *  STATIC FUNCTION
 ********************/




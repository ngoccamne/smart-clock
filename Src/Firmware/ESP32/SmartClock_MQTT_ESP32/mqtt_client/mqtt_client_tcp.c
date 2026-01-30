/*********************
 *      INCLUDES
 *********************/

#include <string.h>
#include "esp_log.h"
#include "mqtt_client_tcp.h"
#include "esp_crt_bundle.h"

/*********************
 *      DEFINES
 *********************/



/**********************
 *  EXTERN VARIABLES
 **********************/

/**********************
 *     VARIABLES
 **********************/


/**********************
 *  STATIC VARIABLES
 **********************/

static const char *TAG = "MQTT";

static int led_id = -1;

/**********************
 *   STATIC FUNCTIONS
 **********************/

void led_control(char *data, int len)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    char cmd[16];
    memcpy(cmd, data, len);
    cmd[len] = '\0';

    int led_state = atoi(cmd);

    if (led_state == 1) 
    {
        gpio_set_level(LED_PIN, 1);
        ESP_LOGI(TAG, "LED ON");
    } 
    else if (led_state == 0) 
    {
        gpio_set_level(LED_PIN, 0);
        ESP_LOGI(TAG, "LED OFF");
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT Connected! Subscribe topic...");
        led_id = esp_mqtt_client_subscribe(event->client, "smartclock/led", 0);
        break;

    case MQTT_EVENT_SUBSCRIBED:
        if (event->msg_id == led_id) 
        {
            ESP_LOGI(TAG, "Topic subscribed successfully: LED");
        }
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT Disconnected!");
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "Received data:");
        printf("TOPIC: %.*s\n", event->topic_len, event->topic);
        printf("DATA:  %.*s\n", event->data_len, event->data);

        if (event->topic_len == 14 && strncmp(event->topic, "smartclock/led", 14) == 0) 
        {
            // TOPIC: LED
            led_control(event->data, event->data_len);
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "MQTT Error");
        break;
    default:
        break;
    }
}

esp_mqtt_client_handle_t mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = URI,
        .credentials.username = USERNAME,
        .credentials.authentication.password = PASSWORD,
        
        .broker.verification.crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

    return client;
}



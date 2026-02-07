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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "min.h"

#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/

#define UART_PORT_NUM      UART_NUM_1
#define TXD_PIN           (GPIO_NUM_17)
#define RXD_PIN           (GPIO_NUM_16)
#define LED_PIN           (GPIO_NUM_2)
#define UART_BAUD_RATE    (115200)
#define BUF_SIZE          (1024)

#define LED_CONTROL_ID    0x05
#define DS3231_ID         0x00
#define PIR_ID               0x01
#define DHT11_ID            0x02

/**********************
 *     VARIABLES
 **********************/

struct min_context min_ctx; 
esp_mqtt_client_handle_t client;

/**********************
 *  STATIC VARIABLES
 **********************/

static const char *TAG = "MAIN_APP";

/**********************
 *     FUNCTION
 **********************/
void led_control_stm32(char *data, int len);
void min_tx_start(uint8_t port);
void min_tx_finished(uint8_t port);
uint16_t min_tx_space(uint8_t port);
void min_tx_byte(uint8_t port, uint8_t byte);
void init_uart();
void uart_receive_task(void *arg);
uint32_t min_time_ms(void);

void min_application_handler(uint8_t min_id, uint8_t const *payload, uint8_t len, uint8_t port) {
    printf("ID: %d, Len: %d\n", min_id, len);

    switch (min_id) 
    {
        case DS3231_ID: 
            if (len >= 7) 
            {
                char ds3231[100];

                sprintf(ds3231, "{\"hour\":%d,\"minute\":%d,\"second\":%d,\"day\":%d,\"date\":%d,\"month\":%d,\"year\":%d}", 
                payload[0], payload[1], payload[2], payload[3], payload[4], payload[5], payload[6]);

                esp_mqtt_client_publish(client, "smartclock/stm32/ds3231", ds3231, 0, 1, 0);
            }
            break;

        case PIR_ID:
            if (len >= 1) 
            {
                char str_pir[10];
                uint8_t pir = payload[0];
            
                printf("PIR:  %d", pir);

                sprintf(str_pir, "%d", pir);
                esp_mqtt_client_publish(client, "smartclock/stm32/pir", str_pir, 0, 1, 0);
            }
            break;
        
        case DHT11_ID:
            if (len >= 2) 
            {
                char dht11[30];
                sprintf(dht11, "{\"hum\":%d,\"temp\":%d}", payload[0], payload[1]);
                printf("DHT11: Huminity: %d%%, Temperature: %d*C\n", payload[0], payload[1]);

                esp_mqtt_client_publish(client, "smartclock/stm32/dht11", dht11, 0, 1, 0);
            }
            break;
        
        case 0x05: // ID tự gửi (nếu có loopback test)
            // printf("ESP32 nhan lai goi tin cua chinh mình\n");
            break;
            
        default:
            printf("Nhan goi tin la ID: %d\n", min_id);
            break;
    }
}

/********************
 *  MAIN FUNCTION
 ********************/

void app_main(void)
{
    // INIT
    nvs_flash_init();
    init_uart();
    min_init_context(&min_ctx, 0);

    // CONNECT TO WIFI
    ESP_LOGI(TAG, "Start to connect Wifi...");
    wifi_init_sta();

    // MQTT
    client = mqtt_app_start();

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

    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    xTaskCreate(uart_receive_task, "uart_rx_task", 4096, NULL, 10, NULL);

    mqtt_led_stm32_callback(led_control_stm32);
}

/********************
 *  STATIC FUNCTION
 ********************/

void led_control_stm32(char *data, int len)
{
    char cmd[16];
    memcpy(cmd, data, len);
    cmd[len] = '\0';

    uint8_t led_state = atoi(cmd);

    min_send_frame(&min_ctx, LED_CONTROL_ID, &led_state, 1);
    printf("Da gui lenh dieu khien led: %d sang STM32\n", led_state);
}

void min_tx_start(uint8_t port)
{

}

void min_tx_finished(uint8_t port)
{

}

uint16_t min_tx_space(uint8_t port)
{
	return 512;
}

void min_tx_byte(uint8_t port, uint8_t byte) {
    uart_write_bytes(UART_PORT_NUM, (const char*)&byte, 1);
}

void init_uart() {
    const uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    uart_driver_install(UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_PORT_NUM, &uart_config);
    uart_set_pin(UART_PORT_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void uart_receive_task(void *arg) {
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    while (1) {
        int len = uart_read_bytes(UART_PORT_NUM, data, BUF_SIZE, pdMS_TO_TICKS(10));
        
        if (len > 0) {
            min_poll(&min_ctx, data, (uint32_t)len);
        }
    }
}

uint32_t min_time_ms(void)
{
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}
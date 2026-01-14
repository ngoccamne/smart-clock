/*********************
 *      INCLUDES
 *********************/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "min.h"

/**********************
 *  DEFINES / MACROS
 **********************/

#define UART_PORT_NUM      UART_NUM_1
#define TXD_PIN           (GPIO_NUM_17)
#define RXD_PIN           (GPIO_NUM_16)
#define UART_BAUD_RATE    (115200)
#define BUF_SIZE          (1024)

/**********************
 *     VARIABLES
 **********************/

struct min_context min_ctx;

/**********************
 *     FUNCTION
 **********************/
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

void min_application_handler(uint8_t min_id, uint8_t const *payload, uint8_t len, uint8_t port) {
    printf("ID: %d, Len: %d\n", min_id, len);
    switch (min_id) 
    {
        case 0x02: // ID nhận từ STM32
            printf("NHAN TU STM32: Gia tri = %d\n", payload[0]);
            break;
            
        case 0x01: // ID tự gửi (nếu có loopback test)
            // printf("ESP32 nhan lai goi tin cua chinh mình\n");
            break;
            
        default:
            printf("Nhan goi tin la ID: %d\n", min_id);
            break;
    }
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

/********************
 *  MAIN FUNCTION
 ********************/

void app_main(void)
{
    init_uart();
    min_init_context(&min_ctx, 0);
    xTaskCreate(uart_receive_task, "uart_rx_task", 4096, NULL, 10, NULL);
    
    uint8_t count = 0;
    while(1) {
        count++;
        
        //min_send_frame(&min_ctx, 0x01, &count, 1);
        //printf("Da gui count: %d sang STM32\n", count);
        
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}


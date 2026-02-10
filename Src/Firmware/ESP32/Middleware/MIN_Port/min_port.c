#include "min_port.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

struct min_context min_ctx;

void min_tx_byte(uint8_t port, uint8_t byte)
{
    uart_write_bytes(MIN_UART_NUM, (const char *)&byte, 1);
}

uint16_t min_time_ms(void)
{
    return (uint16_t)(esp_timer_get_time() / 1000);
}

void min_port_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = MIN_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_param_config(MIN_UART_NUM, &uart_config);
    uart_set_pin(MIN_UART_NUM, MIN_TX_PIN, MIN_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(MIN_UART_NUM, 1024, 0, 0, NULL, 0);

    min_init_context(&min_ctx, 1);
}

void min_port_task(void *pvParameters)
{
    uint8_t *data = (uint8_t *)malloc(128);
    while (1)
    {

        int len = uart_read_bytes(MIN_UART_NUM, data, 128, 20 / portTICK_PERIOD_MS);

        if (len > 0)
        {

            min_poll(&min_ctx, data, len);
        }
    }
    free(data);
}

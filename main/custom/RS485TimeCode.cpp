#include "RS485TimeCode.h"
#include "main/ESP32TimeServerSettings.h"

#if RS485_TIMECODE_ENABLED

#include <driver/uart.h>
#include <esp_log.h>

static const char *TAG = "RS485";

void RS485TimeCode_Init()
{
    uart_config_t uart_config = {
        .baud_rate = RS485_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_driver_install(RS485_UART_NUM, 0, 0, 0, nullptr, 0);

    uart_param_config(RS485_UART_NUM, &uart_config);

    uart_set_pin(
        RS485_UART_NUM,
        RS485_TX_PIN,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE);

    ESP_LOGI(TAG, "RS485 Time Code Output Enabled");
}

void RS485TimeCode_SendCurrentTime()
{
    time_t now;
    struct tm localTime;

    time(&now);
    localtime_r(&now, &localTime);

    char buffer[16];

    snprintf(
        buffer,
        sizeof(buffer),
        "X%02d%02d%02d0000\r\n",
        localTime.tm_hour,
        localTime.tm_min,
        localTime.tm_sec);

    uart_write_bytes(
        RS485_UART_NUM,
        buffer,
        strlen(buffer));
}

#else

void RS485TimeCode_Init() {}
void RS485TimeCode_SendCurrentTime() {}

#endif
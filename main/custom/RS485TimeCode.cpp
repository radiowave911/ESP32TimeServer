#include "RS485TimeCode.h"
#include "ESP32TimeServerSettings.h"
#include "Arduino.h"


#if RS485_TIMECODE_ENABLED

#include <driver/uart.h>
#include <esp_log.h>

static const char *TAG = "RS485";

static HardwareSerial RS485Serial(RS485UartNum);

void RS485TimeCode_Init()
{
    RS485Serial.begin(RS485BaudRate, SERIAL_8N1, RS485RxPin, RS485TxPin);

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

    RS485Serial.write(buffer);
}

#else

void RS485TimeCode_Init() {}
void RS485TimeCode_SendCurrentTime() {}

#endif
#include "stm32f10x.h"
#include "delay.h"
#include "uart.h"
#include "dht11.h"
#include <stdio.h>

int main(void) {
    uint8_t temp, humi, status;
    char buffer[50];  // luu chuoi ki tu gui qua UART

    TIM2_Init();
    UART1_Init(9600);
    DHT11_Init();

    UART1_SendString("DHT11 Test Start\r\n");

    while (1) {
        status = DHT11_ReadData(&temp, &humi);
        if (status == 0) {
            sprintf(buffer, "Temperature: %d°C, Humidity: %d%%\r\n", temp, humi);
        } else {
            sprintf(buffer, "DHT11 Read Error: %d\r\n", status);
        }
        UART1_SendString(buffer);
        delay_ms(2000);
    }
}

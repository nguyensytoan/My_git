#include "main.h"
#include "i2c.h"
#include "uart.h"
#include "bh1750.h"
#include <stdio.h>

int main(void) {
    I2C_Config();
    UART_Init_Config();
    BH1750_Init();

    char buffer[50];

    while (1) {
        uint16_t light = BH1750_ReadLight();
        sprintf(buffer, "Light Intensity: %d lux\r\n", light);
        UART_SendString(buffer);
        for (int i = 0; i < 15000000; i++); // Delay
    }
}

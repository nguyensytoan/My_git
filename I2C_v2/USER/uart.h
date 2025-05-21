#ifndef UART_H
#define UART_H

#include "stm32f10x.h"
#include <stdio.h>

void UART_Init_Config(void);
void UART_SendString(char *str);

#endif

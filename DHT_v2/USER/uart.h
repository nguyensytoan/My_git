#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"
#include <stdio.h>

void UART1_Init(uint32_t baudrate);
void UART1_SendChar(char c);
void UART1_SendString(char* str);

#endif

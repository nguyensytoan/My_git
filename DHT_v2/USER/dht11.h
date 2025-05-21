#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"
#include "delay.h"

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_Pin_1
#define DHT11_PIN_SOURCE GPIO_PinSource1

void DHT11_Init(void);
uint8_t DHT11_ReadData(uint8_t* temp, uint8_t* humi);

#endif

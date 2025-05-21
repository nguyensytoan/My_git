#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

void TIM2_Init(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

#endif

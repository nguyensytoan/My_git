#include "delay.h"

void TIM2_Init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  // khai bao 1 bien TimeBaseStructure
    
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;  // F(timer)= 1MHz ==> 1us,  F(timer) = ( F sytem / ( prescaler + 1) ) , prescaler : bo chia tan
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;     // gia tri thanh ghi dem lai 65535
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // chia clock he so dau vao ,TIM_CKD_DIV1 (mac dinh la ko chia)
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_Cmd(TIM2, ENABLE);
}

void delay_us(uint16_t us) {
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < us);
}

void delay_ms(uint16_t ms) {
    while (ms--) {
        delay_us(1000);
    }
}

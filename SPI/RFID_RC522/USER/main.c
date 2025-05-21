#include "stm32f10x.h"
#include "Delay.h"
#include "RFID.h"
#include <stdio.h>
#include "My_usart.h"

void My_GPIO_Init(void);

uint8_t CardID[5];
	/*
	Chân PB11 – SS
	Chân PB13 – SCK
	Chân PB14 – MISO
	Chân PB15 – MOSI
	*/
int main(){
	SysTick_Init();
	TM_MFRC522_Init();
	usart1_cfg_A9A10(BAUD_9600);

	while(1) {
		if (TM_MFRC522_Check(CardID) == MI_OK) {
			printf("ID: 0x%02X%02X%02X%02X%02X \n", CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
			delay_ms(2000);
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		}
	}
}

void My_GPIO_Init(void) {
	GPIO_InitTypeDef gpioInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpioInit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpioInit.GPIO_Speed=GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14;
	GPIO_Init(GPIOC, &gpioInit);
}
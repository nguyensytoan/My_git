//#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

#define DHT11_SUCCESS         1
#define DHT11_ERROR_CHECKSUM  2
#define DHT11_ERROR_TIMEOUT   3

typedef struct {
	uint8_t temparature;
	uint8_t humidity;
	GPIO_TypeDef* port;
	uint16_t pin;
} DHT11_Dev;

void delay_us(uint16_t us) {
	TIM2->CNT = 0;
	while (TIM2->CNT < us);
}

void delay_ms(uint16_t ms) {
	while (ms--) delay_us(1000);
}

void Timer2_Init(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;  // 1MHz clock (1us per tick)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM2, ENABLE);
}

int DHT11_init(DHT11_Dev* dev, GPIO_TypeDef* port, uint16_t pin) {
	GPIO_InitTypeDef GPIO_InitStructure;
	dev->port = port;
	dev->pin = pin;

	// Enable GPIO clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// Configure GPIO pin as Output
	GPIO_InitStructure.GPIO_Pin = dev->pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // Push-Pull Output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(dev->port, &GPIO_InitStructure);

	return 0;
}

int DHT11_read(DHT11_Dev* dev) {
	uint8_t i, j, temp;
	uint8_t data[5] = {0, 0, 0, 0, 0};
	GPIO_InitTypeDef GPIO_InitStructure;

	// Send Start Signal
	GPIO_InitStructure.GPIO_Pin = dev->pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(dev->port, &GPIO_InitStructure);

	GPIO_ResetBits(dev->port, dev->pin);
	delay_ms(18);
	GPIO_SetBits(dev->port, dev->pin);
	delay_us(40);

	// Set pin as input
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(dev->port, &GPIO_InitStructure);

	// Wait for DHT11 response (LOW 80us)
	TIM2->CNT = 0;
	while (!GPIO_ReadInputDataBit(dev->port, dev->pin)) {
		if (TIM2->CNT > 100) return DHT11_ERROR_TIMEOUT;
	}

	// Wait for HIGH signal (80us)
	TIM2->CNT = 0;
	while (GPIO_ReadInputDataBit(dev->port, dev->pin)) {
		if (TIM2->CNT > 100) return DHT11_ERROR_TIMEOUT;
	}

	// Read 40 bits
	for (j = 0; j < 5; j++) {
		for (i = 0; i < 8; i++) {
			// Wait for LOW (50us)
			while (!GPIO_ReadInputDataBit(dev->port, dev->pin));

			TIM2->CNT = 0;
			while (GPIO_ReadInputDataBit(dev->port, dev->pin));

			// If HIGH > 40us, it's a 1
			temp = TIM2->CNT;
			data[j] = data[j] << 1;
			if (temp > 40) data[j] |= 1;
		}
	}

	// Verify checksum
	if (data[4] != (data[0] + data[1] + data[2] + data[3])) return DHT11_ERROR_CHECKSUM;

	dev->humidity = data[0];
	dev->temparature = data[2];

	return DHT11_SUCCESS;
}

int main(void) {
	SystemCoreClockUpdate();
	Timer2_Init();

	DHT11_Dev dev;
	DHT11_init(&dev, GPIOB, GPIO_Pin_6);

	while (1) {
		int res = DHT11_read(&dev);
		if (res == DHT11_SUCCESS) {
// Print data (có th? g?i qua UART n?u c?n)
		}

		delay_ms(1000);
	}
}
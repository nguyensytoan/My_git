#include "DHT.h"
#include <stdint.h>
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h" 
#include "Delay.h"
#include "My_usart.h"

//************************** Low Level Layer ********************************************************//
uint16_t u16Tim = 0;
void Timer_Init(TIM_TypeDef* TIMx) 
{ 
	// Enable clock for the specified timer 
	if (TIMx == TIM2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	else if (TIMx == TIM3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	else if (TIMx == TIM4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	// Init timer 
	TIM_TimeBaseInitTypeDef timerInit; 
	// mode up counter 
	timerInit.TIM_CounterMode = TIM_CounterMode_Up; 
	// set period 
	timerInit.TIM_Period = 0xFFFF; 
	// set prescaler 
	timerInit.TIM_Prescaler = 72 - 1; 
	// clock division 
	TIM_TimeBaseInit(TIMx, &timerInit); 
	// Enable the timer 
	TIM_Cmd(TIMx, ENABLE);
	//f  = fclock(bus)/ (prescaller * period)
}
static void DHT_SetPinOut(DHT_Name* DHT)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT->Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DHT->Port, &GPIO_InitStructure);
}
static void DHT_SetPinIn(DHT_Name* DHT)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT->Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DHT->Port, &GPIO_InitStructure);
}
static void DHT_WritePin(DHT_Name* DHT, uint8_t Value)
{
	GPIO_WriteBit(DHT->Port, DHT->Pin, Value);
}
static uint8_t DHT_ReadPin(DHT_Name* DHT)
{
	uint8_t Value;
	Value =  GPIO_ReadInputDataBit(DHT->Port, DHT->Pin);
	return Value;
}
//********************************* Middle level Layer ****************************************************//
void DHT_Start(DHT_Name* DHT)
{
	DHT_SetPinOut(DHT);
	DHT_WritePin(DHT, 0);
	delay_us(DHT->Type);
	DHT_WritePin(DHT, 1);
	TIM_SetCounter(DHT->Timer, 0);
	while (TIM_GetCounter(DHT->Timer) < 10) {
		if (GPIO_ReadInputDataBit(DHT->Port, DHT->Pin)) {
			break;
		}
	}
	u16Tim = TIM_GetCounter(DHT->Timer);
	if (u16Tim >= 10) {
		while (1) {
			printf("WHILE ONE,%d\n", u16Tim);
		}
	}

	TIM_SetCounter(DHT->Timer, 0);
	while (TIM_GetCounter(DHT->Timer) < 45) {
		if (!GPIO_ReadInputDataBit(DHT->Port, DHT->Pin)) {
			break;
		}
	}
	u16Tim = TIM_GetCounter(DHT->Timer);
	if ((u16Tim >= 45) || (u16Tim < 1)) {
		while (1) {
			printf("WHILE TWO, %d\n", u16Tim);
		}
	}

	TIM_SetCounter(DHT->Timer, 0);
	while (TIM_GetCounter(DHT->Timer) < 90) {
		if (GPIO_ReadInputDataBit(DHT->Port, DHT->Pin)) {
			break;
		}
	}
	u16Tim = TIM_GetCounter(DHT->Timer);
	if (u16Tim >= 90 || u16Tim <= 71) {
		while (1) {
			printf("WHILE THREE,%d\n", u16Tim);
		}
	}

	TIM_SetCounter(DHT->Timer, 0);
	while (TIM_GetCounter(DHT->Timer) < 95) {
		if (!GPIO_ReadInputDataBit(DHT->Port, DHT->Pin)) {
			break;
		}
	}
	u16Tim = TIM_GetCounter(DHT->Timer);
	if (u16Tim >= 95 || u16Tim <= 75) {
		while (1) {
			printf("WHILE FOUR,%d\n", u16Tim);
		}
	}
}

uint8_t DHT_Read(DHT_Name* DHT)
{
	uint8_t data = 0;
	uint32_t timeout;

	for(int i = 0; i < 8; i++)
	{
		// kiem tra bit thap
		TIM_SetCounter(DHT->Timer, 0);
		while (TIM_GetCounter(DHT->Timer) < 65) { 
			if (GPIO_ReadInputDataBit(DHT->Port, DHT->Pin)) { 
				break; 
			} 
		} 
		u16Tim = TIM_GetCounter(DHT->Timer); 
		
		if ((u16Tim >= 65) || (u16Tim <= 45)){ 
			while (1) { 
				printf("WHILE READ 1,%d\n", u16Tim);
			} 
		}
		// kiem tra bit cao
		TIM_SetCounter(DHT->Timer, 0);
		while (TIM_GetCounter(DHT->Timer) < 85) { 
			if (!GPIO_ReadInputDataBit(DHT->Port, DHT->Pin)) { 
				break; 
			} 
		} 
		u16Tim = TIM_GetCounter(DHT->Timer); 
		
		if ((u16Tim >= 85) || (u16Tim <= 10)){ 
			while (1) { 
				printf("WHILE READ 2 %d\n", u16Tim);
			} 
		}
		data <<= 1;
		if(u16Tim > 45){
			data |= 1;
		}else {
			data &= ~1;
		}
	}
	return data;
}


//************************** High Level Layer ********************************************************//
void DHT_Init(DHT_Name* DHT, uint8_t DHT_Type, GPIO_TypeDef* DH_PORT, uint16_t DH_Pin, TIM_TypeDef* TIMx)
{
	Timer_Init(TIMx);
	if (DHT_Type == DHT11)
	{
		DHT->Type = DHT11_STARTTIME;
	}
	else if (DHT_Type == DHT22)
	{
		DHT->Type = DHT22_STARTTIME;
	}
	DHT->Port = DH_PORT;
	DHT->Pin = DH_Pin;
	DHT->Timer = TIMx;
}

uint8_t DHT_ReadTempHum(DHT_Name* DHT)
{
	DHT->Respone =0;
	uint8_t Temp1, Temp2, RH1, RH2;
	uint16_t Temp, Humi, SUM = 0;
	DHT_Start(DHT);
	RH1 = DHT_Read(DHT);
	
	RH2 = DHT_Read(DHT);
	Temp1 = DHT_Read(DHT);
	Temp2 = DHT_Read(DHT);
	SUM = DHT_Read(DHT);
	Temp = (Temp1<<8)|Temp2;
	Humi = (RH1<<8)|RH2;
	DHT->Temp = (float)(Temp1);
	DHT->Humi = (float)(RH1);
	if (SUM != (RH1 + RH2 + Temp1 + Temp2)) {
    printf("Checksum error\n");
    return 0; // Tránh dùng giá tr? sai
	}
	else
		return SUM;
}
#include "spi.h"

uint8_t TM_SPI_Send(uint8_t data)
{
	
	SPI_I2S_SendData(SPI2, data);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET) {
	}
	return SPI_I2S_ReceiveData(SPI2);
}

void TM_SPI_Init(void)
{
	GPIO_InitTypeDef gpioInit;
	SPI_InitTypeDef   SPI_InitStructure;
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*
	Chân PB11 – SS
	Chân PB13 – SCK
	Chân PB14 – MISO
	Chân PB15 – MOSI
	*/
	gpioInit.GPIO_Mode=GPIO_Mode_AF_PP;
	gpioInit.GPIO_Speed=GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin=GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &gpioInit);
	
	gpioInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpioInit.GPIO_Speed=GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin=GPIO_Pin_14;
	GPIO_Init(GPIOB, &gpioInit);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//15,14
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//2
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//DFF11
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // page 704 datasheet
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// high -> select slave 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	SPI_Cmd(SPI2, ENABLE);
}

void TM_MFRC522_InitPins(void)
{
	GPIO_InitTypeDef gpioInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpioInit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpioInit.GPIO_Speed=GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB, &gpioInit);
	MFRC522_CS_HIGH;
}
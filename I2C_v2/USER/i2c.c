#include "i2c.h"

void I2C_Config(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;   // dia chi slave, nhung dang dung che do MASTER nen ko quan tam
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;
    
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
}

void I2C_Write(uint8_t addr, uint8_t data) {
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));   // cho bus i2c san sang
	
    // gui tin hieu START bat dau giao tiep
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  // cho den khi STM32 vao che do MASTER
	
		// gui dia chi slave che do Write
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); // cho SLAVE xac nhan tra ve ACK
	
		// gui du lieu
    I2C_SendData(I2C1, data);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); // cho SLAVE nhan data tra ve ACK
		
		// gui tin hieu STOP
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void I2C_Read(uint8_t addr, uint8_t *buffer, uint8_t length) {
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // cho bus i2c san sang

    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); // cho den khi STM32 vao che do MASTER

		// gui dia chi slave che do Read
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); // cho SLAVE xac nhan tra ve ACK

		// Read 2 byte data
    while (length--) {
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)); // cho SLAVE gui data
			*buffer++ = I2C_ReceiveData(I2C1);  // nhan data tu SLAVE và luu vào buffer ( buffer ++ : di chuyen con tro de luu vào byte tiep theo)
    }

    I2C_GenerateSTOP(I2C1, ENABLE);  // gui tin hieu STOP
}

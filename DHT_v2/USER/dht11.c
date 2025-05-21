#include "dht11.h"

 // static : chi duoc goi tu file nay, ko duoc goi tu file 
 
 static void DHT11_SetPinOutput(void) {   //cau hinh PA1 cua stm32 : output
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

static void DHT11_SetPinInput(void) {    //cau hinh PA1 cua stm32 : input
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

void DHT11_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}

uint8_t DHT11_ReadData(uint8_t* temp, uint8_t* humi) { // ham nay de doc data tu DHT11 va lua vao gia tri tam
    uint8_t buffer[5] = {0};
    uint8_t i, j;
    // gui tin hieu start den DHT11
    DHT11_SetPinOutput();
    GPIO_ResetBits(DHT11_PORT, DHT11_PIN);
    delay_ms(18);
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);
    delay_us(30);
    DHT11_SetPinInput();

    // DHT11 gui phan hoi
    if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) return 1;
    while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)); // bit 0 : cho 80us
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN));  // bit 1 : cho 80us

    // DHT11 bat dau gui data  5 byte = 40 bit = 2 byte humidity + 2 byte temperature  + 1 byte checksum
    for (j = 0; j < 5; j++) {
        for (i = 0; i < 8; i++) {
					  while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN));   // bit 0 : cho 50us
            delay_us(40);
            if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) buffer[j] |= (1 << (7 - i));
            while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN));
        }
    }

    
    if ((buffer[0] + buffer[1] + buffer[2] + buffer[3]) != buffer[4]) return 2;

    *humi = buffer[0];
    *temp = buffer[2];

    return 0;
}

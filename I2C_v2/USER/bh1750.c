#include "bh1750.h"

void BH1750_Init(void) {
    I2C_Write(BH1750_ADDRESS, 0x10); // Cau hinh ch do do liên tuc, de phân giai cao
}

uint16_t BH1750_ReadLight(void) {
    uint8_t data[2];
    I2C_Read(BH1750_ADDRESS, data, 2);
    return ((data[0] << 8) | data[1])/1.2;
}

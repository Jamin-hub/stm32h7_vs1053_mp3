#include "sht30.h"

#define SHT30_ADDR 0x44 << 1  // SHT30 I2C地址左移1位

// 假设你使用的是 hi2c1，如果不是，请替换为你的 I2C 句柄（如 hi2c1）


/**
 * @brief  CRC校验（保持不变）
 */
uint8_t SHT3x_CRC_CAL(uint16_t DAT)
{
    uint8_t i, t, temp;
    uint8_t CRC_BYTE = 0xFF;
    temp = (DAT >> 8) & 0xFF;

    for(t = 0; t < 2; t++)
    {
        CRC_BYTE ^= temp;
        for(i = 0; i < 8; i++)
        {
            if(CRC_BYTE & 0x80)
            {
                CRC_BYTE <<= 1;
                CRC_BYTE ^= 0x31;
            }
            else
            {
                CRC_BYTE <<= 1;
            }
        }
        if(t == 0)
        {
            temp = DAT & 0xFF;
        }
    }
    return CRC_BYTE;
}



void SHT30_Init(void) {
    // 发送软件复位命令
    uint8_t cmd[2] = {0x30, 0xA2};
    HAL_I2C_Master_Transmit(&hi2c1, SHT30_ADDR, cmd, 2, 100);
    HAL_Delay(15);  // 等待传感器复位
}

uint8_t SHT30_Read(float *temperature, float *humidity) {
    uint8_t data[6];
    uint16_t temp_raw, humi_raw;

    // // 发送测量命令
    // HAL_I2C_Mem_Write(&hi2c1, SHT30_ADDR, 0x2C10, I2C_MEMADD_SIZE_16BIT, 0, 0, 1000);
    uint8_t fetch_cmd[2] = {0x2C, 0x06};
    HAL_I2C_Master_Transmit(&hi2c1, SHT30_ADDR, fetch_cmd, 2, 100);

    // 等待数据准备好
    HAL_Delay(15);

    // 读取6个字节的数据
    HAL_I2C_Master_Receive(&hi2c1, SHT30_ADDR, data, 6, 1000);

    // 解析数据
    temp_raw = (data[0] << 8) | data[1];
    uint8_t  TempCRC = data[2];
    humi_raw = (data[3] << 8) | data[4];
    uint8_t  HumCRC = data[5];
    if (SHT3x_CRC_CAL(temp_raw) != TempCRC || SHT3x_CRC_CAL(humi_raw) != HumCRC)
    {
        return 1; // CRC 错误
    }
    // 计算实际温湿度值
    *temperature = (175.0 / 65535) * temp_raw - 45.0;
    *humidity = (100.0 / 65535) * humi_raw;
    return 0;
}
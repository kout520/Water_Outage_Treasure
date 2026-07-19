#include "headfile.h"
#include "MYI2C.h"

// ============================================================
// AHT20温湿度传感器 (I2C地址: 0x38)
// ============================================================

static MYI2C_Struct aht20_dev;

void AHT20_Init(void)
{
    MYI2C_Init(&aht20_dev, 2000, 0x38);  // AHT20地址0x38, 每2秒读取一次
}

void AHT20_Task(void)
{
    MYI2C_Handle(&aht20_dev);
}

float AHT20_GetTemperature(void)
{
    return aht20_dev.T;
}

float AHT20_GetHumidity(void)
{
    return aht20_dev.RH;
}

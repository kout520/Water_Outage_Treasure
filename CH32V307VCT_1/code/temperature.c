#include "headfile.h"
#include "aht20.h"

void Temperature_Init(void)
{
    // AHT20初始化在AHT20_Init()中完成
}

float Temperature_GetCelsius(void)
{
    return AHT20_GetTemperature();
}

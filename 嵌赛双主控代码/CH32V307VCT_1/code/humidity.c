#include "headfile.h"
#include "aht20.h"

void Humidity_Init(void)
{
    // AHT20初始化在AHT20_Init()中完成
}

float Humidity_GetPercent(void)
{
    return AHT20_GetHumidity();
}

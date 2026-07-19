#ifndef _aht20_
#define _aht20_

// ============================================================
// AHT20温湿度传感器 (I2C: PB6=SCL, PB7=SDA, Addr=0x38)
// 与OLED共享I2C总线
// ============================================================

void AHT20_Init(void);
void AHT20_Task(void);              // 每10ms调用一次(主循环)
float AHT20_GetTemperature(void);   // 获取温度(℃)
float AHT20_GetHumidity(void);      // 获取湿度(%)

#endif

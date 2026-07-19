#ifndef _temperature_
#define _temperature_

// ============================================================
// 温度传感器
// 预留接口, 支持DS18B20(1-Wire) / NTC / DHT22等
// 当前使用默认值25.0度
// ============================================================

void  Temperature_Init(void);
float Temperature_GetCelsius(void);    // 获取温度(摄氏度)

// 如使用DS18B20, 去掉下面注释:
// void Temperature_Init_OneWire(GPIO_TypeDef* port, uint16_t pin);
// float Temperature_Read_DS18B20(void);

#endif

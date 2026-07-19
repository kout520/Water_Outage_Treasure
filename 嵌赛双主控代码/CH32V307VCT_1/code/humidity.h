#ifndef _humidity_
#define _humidity_

// ============================================================
// 湿度传感器 (PA5 = ADC_CH5)
// 输出: 模拟电压, 范围: 0~100% RH
// ============================================================

void  Humidity_Init(void);
float Humidity_GetPercent(void);       // 获取湿度(%)

#endif

#ifndef __ADC_GOING_H_
#define __ADC_GOING_H_

// ============================================================
// ADC DMA采集: 水位 + 湿度
// PA4 = ADC_CH4 = 水位传感器
// PA5 = ADC_CH5 = 湿度传感器
// ============================================================

#define ADC_BUF_LEN 1

extern u16 ADC_ConvertedValue[ADC_BUF_LEN];

void ADC_DMA_CONF(void);
float ADC_GetHumidity(void);      // 获取湿度(%), 0~100%

#endif

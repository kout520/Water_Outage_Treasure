#ifndef __ADC_GOING_H_
#define __ADC_GOING_H_

#define length 2

void adc_Init(void);
void DMA_Tx_Init( void );
void ADC_DMA_CONF(void);

extern u16 ADC_ConvertedValue[length];


#endif 

#ifndef _MYI2C_h_
#define _MYI2C_h_

#include "headfile.h"

// ============================================================
// 软件I2C (PA1=SCL, PA2=SDA) — AHT20传感器专用
// OLED使用 PB6/PB7, 独立I2C总线互不干扰
// ============================================================

#define IIC_SDA_PORT   GPIOA
#define SDA_Pin        GPIO_Pin_2
#define IIC_SCL_PORT   GPIOA
#define SCL_Pin        GPIO_Pin_1

#define MYI2C_delay_us_cnt   16
#define MYI2C_Tick           10     // 定时器tick间隔(ms)
#define MYI2C_Buffer_Size    20

#define Wait_Ack_time        199
#define MinReadTim           500
#define PowerOnTim           10
#define MeasureTim           150    // 等待测量完成(ms)

#define SENSOR_IDLE      0
#define SENSOR_MEASURE   1
#define SENSOR_COMPLETE  2

typedef struct
{
    unsigned char Adrr;
    unsigned int  timcnt;
    unsigned char ErrFlag;
    unsigned char Step;
    unsigned int  SetRTim;
    unsigned char SendByte[MYI2C_Buffer_Size];
    unsigned char ReadByte[MYI2C_Buffer_Size];
    float RH;   // 湿度 %
    float T;    // 温度 ℃
} MYI2C_Struct;

extern MYI2C_Struct SENx;

void  MYI2C_Init(MYI2C_Struct *pst, unsigned int ReadTimMS, unsigned char xAddr);
void  MYI2C_Handle(MYI2C_Struct *pst);
void  MYI2C_Delay_us(unsigned long nTim);
unsigned char CheckCrc8(unsigned char *pDat, unsigned char Lenth);

#endif

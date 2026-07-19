#ifndef _water_quality_
#define _water_quality_

#include "ch32v30x.h"

/* ============================================================
   水质传感器协议
   发送: A0 00 00 00 00 A0
   响应: AA + 14字节数据 + checksum
   checksum = 前15字节累加和的低8位
   数据区 7 x uint16 大端:
     [0] TDS (ppm)
     [1] 盐度x100 (%)
     [2] 电导率 (us/cm)
     [3] 保留
     [4] 比重x10000
     [5] 温度x10 (C)
     [6] 硬度 (ppm)
   ============================================================ */

#define WQ_FRAME_LEN     16       // 响应帧总长度
#define WQ_FRAME_HEADER  0xAA     // 帧头

// 水质数据结构体
typedef struct {
    uint16_t tds;           // TDS (ppm)
    uint16_t salinity_x100; // 盐度x100 (%)
    uint16_t conductivity;  // 电导率 (us/cm)
    uint16_t reserved;      // 保留
    uint16_t sg_x10000;     // 比重x10000
    uint16_t temp_x10;      // 温度x10 (C)
    uint16_t hardness;      // 硬度 (ppm)
} WaterQuality_t;

// 外部变量
extern WaterQuality_t g_water_quality;
extern uint8_t g_wq_data_ready;

// 函数声明
void WQ_Init(uint32_t baud_rate);
void WQ_SendRequest(void);
void WQ_ParseResponse(uint8_t *data);
void WQ_UpdateDisplay(void);
void WQ_Task(void);
void WQ_ResetMonthlyCost(void);
float WQ_GetMonthlyCost(void);
void WQ_SetRate(float rate);
float WQ_GetRate(void);

// 辅助获取浮点值
static inline float WQ_GetTDS(void)         { return (float)g_water_quality.tds; }
static inline float WQ_GetSalinity(void)    { return (float)g_water_quality.salinity_x100 / 100.0f; }
static inline float WQ_GetConductivity(void){ return (float)g_water_quality.conductivity; }
static inline float WQ_GetSG(void)          { return (float)g_water_quality.sg_x10000 / 10000.0f; }
static inline float WQ_GetTemp(void)        { return (float)g_water_quality.temp_x10 / 10.0f; }
static inline float WQ_GetHardness(void)    { return (float)g_water_quality.hardness; }

#endif

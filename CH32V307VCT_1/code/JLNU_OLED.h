#ifndef __JLNU_OLED_H
#define __JLNU_OLED_H

#include "headfile.h"

// ============================================================
// SSD1306 0.96" OLED (I2C: PB6=SCL, PB7=SDA)
// 驱动: JLNU_OLED (移植自STM32, 已适配CH32V307)
// ============================================================

#define OLED_ADDRESS  0x78   // OLED I2C地址

void OLED_I2C_Init(void);                              // I2C GPIO初始化
void OLED_Init(void);                                  // OLED上电初始化
void OLED_SetPos(uint8_t x, uint8_t y);                // 设置光标位置
void OLED_Clear(void);                                 // 清屏
void OLED_ON(void);                                    // 点亮屏幕
void OLED_OFF(void);                                   // 关闭屏幕
void OLED_ShowStr(uint8_t x, uint8_t y, uint8_t ch[], uint8_t TextSize);  // 显示字符串(6x8/8x16)
void OLED_ShowCN(uint8_t x, uint8_t y, uint8_t n);     // 显示中文(16x16)
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);        // 显示图片

#endif

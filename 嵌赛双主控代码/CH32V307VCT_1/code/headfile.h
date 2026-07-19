#ifndef _headfile_h_
#define _headfile_h_

#include "ch32v30x.h"
#include "debug.h"
#include "ch32v30x_conf.h"

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"

// 公共模块

#include "timer.h"

// ADC核心 (DMA采集, 供传感器模块共用)
#include "adc_going.h"

// 传感器模块 (每个传感器独立.h/.c)
#include "hc_sr04.h"          // HC-SR04超声波
#include "humidity.h"          // 湿度 (AHT20)
#include "temperature.h"       // 温度 (AHT20)
#include "aht20.h"             // AHT20温湿度传感器
#include "MYI2C.h"             // 软件I2C (AHT20用)

// 执行器模块
#include "pump_ctrl.h"         // TB6612水泵 (PB0 PWM)
#include "encoder.h"           // 旋转编码器 (PA6/PA7)

// 通信模块
#include "zigbee.h"            // Zigbee (UART4)

// 显示模块
#include "JLNU_OLED.h"         // 0.96" OLED (I2C PB6/PB7)

#endif

#ifndef _water_speed_
#define _water_speed_

// 流量计输入捕获初始化
void Input_Capture_Init(u16 arr, u16 psc);

// 全局变量
extern u8 water_speedflage;
extern float flow_rate;            // 瞬时流量(L/min)
extern float total_flow;           // 累计流量(L)
extern uint32_t total_pulses;      // 累计脉冲数

// 漏水检测
extern uint8_t g_leak_alarm_active;
void Leak_Reset(void);
void Leak_SetSystemActive(uint8_t active);
void Leak_Task(void);

#endif

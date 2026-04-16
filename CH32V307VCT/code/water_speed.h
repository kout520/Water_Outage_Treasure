#ifndef _water_speed_
#define _water_speed_

void Input_Capture_Init( u16 arr, u16 psc );

extern u8 water_speedflage;
extern float total_flow;           // 总流量(L)
extern uint32_t total_pulses ;        // 总脉冲计数

#endif


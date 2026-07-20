#ifndef _water_speed_
#define _water_speed_

void Input_Capture_Init(u16 arr, u16 psc);

extern u8 water_speedflage;
extern float flow_rate;
extern float total_flow;
extern uint32_t total_pulses;

extern uint8_t g_leak_alarm_active;
void Leak_Reset(void);
void Leak_SetSystemActive(uint8_t active);
void Leak_Task(void);
void Leak_SetNormalFlow(float f);
float Leak_GetNormalFlow(void);

#endif

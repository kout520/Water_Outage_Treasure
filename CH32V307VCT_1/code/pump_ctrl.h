#ifndef _pump_ctrl_
#define _pump_ctrl_

// ============================================================
// TB6612 水泵驱动模块
// PWMA: PB0 = TIM3_CH3 (PWM调速 0~100%)
// AIN1: PC1 = 方向 (高=正转)
// AIN2: PE9 = 方向 (低=正转)
// STBY: PE2 = 待机 (高=使能, 低=待机)
// ============================================================

void Pump_Init(void);
void Pump_SetSpeed(uint8_t speed);
void Pump_Start(void);
void Pump_Stop(void);
uint8_t Pump_GetSpeed(void);

#endif

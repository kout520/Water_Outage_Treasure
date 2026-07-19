#ifndef __HC_SR04_H__
#define __HC_SR04_H__

#define TANK_HEIGHT_M   0.27f  // 水箱深度27cm

#define TRIG_PORT   GPIOA
#define TRIG_PIN    GPIO_Pin_4
#define ECHO_PORT   GPIOA
#define ECHO_PIN    GPIO_Pin_3

#define TRIG_High()  GPIO_SetBits(TRIG_PORT, TRIG_PIN)
#define TRIG_Low()   GPIO_ResetBits(TRIG_PORT, TRIG_PIN)
#define ECHO_Read()  GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN)

void HC_SR04_Init(void);
float HC_SR04_GetDistance_cm(void);
float HC_SR04_GetWaterLevel_m(void);

#endif

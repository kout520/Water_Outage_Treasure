#include "headfile.h"

// ============================================================
// TB6612 水泵驱动
// PWMA=PB0(TIM3_CH3), AIN1=PC1, AIN2=PE9, STBY=PE2
// ============================================================

#define AIN1_PORT  GPIOC
#define AIN1_PIN   GPIO_Pin_1
#define AIN2_PORT  GPIOE
#define AIN2_PIN   GPIO_Pin_9
#define STBY_PORT  GPIOE
#define STBY_PIN   GPIO_Pin_2

static uint8_t g_pump_speed = 0;

void Pump_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    // PWM: PB0 = TIM3_CH3
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM3, ENABLE);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);

    // TB6612 GPIO: AIN1(PC1), AIN2(PE9), STBY(PE2)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = AIN1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(AIN1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = AIN2_PIN | STBY_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(STBY_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(AIN1_PORT, AIN1_PIN);
    GPIO_ResetBits(AIN2_PORT, AIN2_PIN);
    GPIO_ResetBits(STBY_PORT, STBY_PIN);
    g_pump_speed = 0;
}

void Pump_SetSpeed(uint8_t speed)
{
    if (speed > 100) speed = 100;
    g_pump_speed = speed;
    if (speed < 45)
        TIM_SetCompare3(TIM3, 0);       // <45%直接断电
    else
        TIM_SetCompare3(TIM3, (uint16_t)speed * 10);
}

void Pump_Start(void)
{
    GPIO_SetBits(AIN1_PORT, AIN1_PIN);
    GPIO_ResetBits(AIN2_PORT, AIN2_PIN);
    GPIO_SetBits(STBY_PORT, STBY_PIN);
}

void Pump_Stop(void)
{
    Pump_SetSpeed(0);
    GPIO_ResetBits(STBY_PORT, STBY_PIN);
}

uint8_t Pump_GetSpeed(void)
{
    return g_pump_speed;
}

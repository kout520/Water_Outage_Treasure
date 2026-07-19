#include "headfile.h"

// ============================================================
// TIM2: 1ms定时中断
// 提供全局时间基准 timer_cnt
// ============================================================

uint32_t timer_cnt = 0;
uint32_t speed_flag = 0;
char test1[100];

void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void TIM2_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

// 1ms定时中断
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        timer_cnt++;
        speed_flag++;

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        // 1000ms时间片: 发送水质检测请求
        if (timer_cnt % 1000 == 0)
        {
            WQ_SendRequest();
        }

        // 1000ms时间片: 超时无脉冲则显示0流量
        if (timer_cnt % 1000 == 0)
        {
            time_flag = 1;

            if (++water_speedflage == 1)
            {
                sprintf(test1, "t10.txt=\"0L/min\"");
                tjc_send_string(test1);
            }
        }
    }
}

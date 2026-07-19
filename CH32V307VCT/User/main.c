/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Description        : 水宝 - 主控 (CH32V307VCT)
*******************************************************************************/

#include "debug.h"
#include "headfile.h"

void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

static void UpdateTowerHMI(void)
{
    char buf[64];
    sprintf(buf, "t18.txt=\"%.0fcm\"", g_tower_water_level);
    tjc_send_string(buf);

}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    __enable_irq();

    GPIO_Toggle_INIT();
    UART_HIM_Init();
    UART_ESP32_Init();
    Zigbee_Init(115200);
    TIM2_Init(960, 100);
    initRingBuffer();
    Input_Capture_Init(0xFFFF, 48000 - 1);
    WQ_Init(9600);

    Leak_Reset();
    Leak_SetSystemActive(0);
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);

    while (1)
    {
        // 非阻塞: 无数据立即返回
        HMI_task();
        esp_received();

        // 1秒定时任务
        static uint32_t t1s = 0;
        if (timer_cnt - t1s >= 1000) {
            t1s = timer_cnt;
            lcd_time();
            Leak_Task();        // 内部自限1s, 此处再限1s确保不频繁
        }

        // 水质数据(仅在有新数据时处理)
        WQ_Task();

        // 塔楼数据更新
        if (g_tower_data_updated) {
            g_tower_data_updated = 0;
            UpdateTowerHMI();
        }

        // flag处理(不阻塞)
        if (flag) {
            flag = 0;
        }

        Delay_Ms(1);  // 1ms轻量化, 不再100%空转
    }
}

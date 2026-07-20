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
    // 用整数*10避免浮点sprintf问题: 24.7cm → 显示 "24.7cm"
    int wl_int = (int)(g_tower_water_level * 10.0f);
    if (wl_int < 0 || wl_int > 5000) wl_int = 0;
    sprintf(buf, "t18.txt=\"%d.%dcm\"", wl_int / 10, wl_int % 10);
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
        HMI_task();
        esp_received();

        static uint32_t t1s = 0;
        if (timer_cnt - t1s >= 1000) {
            t1s = timer_cnt;
            lcd_time();
            Leak_Task();
        }

        WQ_Task();

        if (g_tower_data_updated) {
            g_tower_data_updated = 0;
            UpdateTowerHMI();
        }

        if (flag) flag = 0;

        Delay_Ms(1);
    }
}

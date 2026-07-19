/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Description        : 水宝 - 主控 (CH32V307VCT)
*******************************************************************************/

/*
 * 水质监测 + 流量计 + 漏水检测 + HMI + ESP32 + Zigbee(塔楼通信)
 */

#include "debug.h"
#include "headfile.h"

/*********************************************************************
 * GPIO初始化
 *********************************************************************/
void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    // PE9, PE2 = 输出
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // PC1 = 输出
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // PA0 = 按键
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA4 = 漏水报警输出
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);

    // PC12 = 按键
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*********************************************************************
 * 塔楼数据显示到HMI
 *********************************************************************/
static void UpdateTowerHMI(void)
{
    char buf[64];

    // 水位 t25
    sprintf(buf, "t25.txt=\"%.2fm\"", g_tower_water_level);
    tjc_send_string(buf);

    // 湿度 t26
    sprintf(buf, "t26.txt=\"%.1f%%\"", g_tower_humidity);
    tjc_send_string(buf);

    // 塔楼温度 t27
    sprintf(buf, "t27.txt=\"%.1f\"", g_tower_temperature);
    tjc_send_string(buf);

    // 水泵转速 t28
    sprintf(buf, "t28.txt=\"%d%%\"", g_tower_pump_speed);
    tjc_send_string(buf);
}

/*********************************************************************
 * 主函数
 *********************************************************************/
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    __enable_irq();

    GPIO_Toggle_INIT();                     // GPIO
    UART_HIM_Init();                        // USART2: HMI
    UART_ESP32_Init();                      // USART3: ESP32
    Zigbee_Init(115200);                    // UART4: Zigbee(塔楼通信)
    TIM2_Init(960, 100);                    // 1ms时间基准
    initRingBuffer();
    Input_Capture_Init(0xFFFF, 48000 - 1);  // 流量计
    WQ_Init(9600);                          // USART1: 水质传感器

    Leak_Reset();
    Leak_SetSystemActive(0);

    GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);

    while (1)
    {
        HMI_task();
        esp_received();
        lcd_time();
        WQ_Task();              // 水质数据 + 上传ESP32
        Leak_Task();            // 漏水检测

        // 塔楼Zigbee数据处理 → HMI显示
        if (g_tower_data_updated) {
            g_tower_data_updated = 0;
            UpdateTowerHMI();
        }

        switch (flag) {
            case 0: break;
            case 1: break;
            case 2: Delay_Ms(1000); break;
            case 3: break;
            default: break;
        }
    }
}

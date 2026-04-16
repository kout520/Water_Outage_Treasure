/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 GPIO routine:
 PA0 push-pull output.

*/

#include "debug.h"
#include "headfile.h"
/* Global define */

#define FRAME_LENGTH 7
/* Global Variable */

/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIOA.0
 *
 * @return  none
 */
void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    //lcd初始
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_Init(GPIOA, &GPIO_InitStructure);
    //KEY初始
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //pwm初始

}



/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	//时钟频率默认96000000
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);	
    __enable_irq();   // 开启全局中断，确保中断响应


    ADC_DMA_CONF();//温敏电阻采集
    GPIO_Toggle_INIT();//板载小灯泡
    // initPwm();//pwm初始化
    UART_HIM_Init();//串口2_串口屏通信
    UART_ESP32_Init();//串口3_esp通信
    TIM2_Init(960,100);//定时器中断1ms
    initRingBuffer();		//初始化环形缓冲区
	Input_Capture_Init( 0xFFFF, 48000-1 );//water_speed初始化
    printf("INit sucessful!");
    //int j;
    while(1)
    {
        HMI_task();
        esp_received();
        lcd_time();
        // GPIO_WriteBit(GPIOE, GPIO_Pin_2, (j==0) ? (j=Bit_SET):(j=Bit_RESET));
        switch (flag) {
            case 0:

            break;
            case 1:
            GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_RESET);
            
            break;
            case 2:

            break;
            case 3:

            break;
            default:
            break;
        
        }
    }
}

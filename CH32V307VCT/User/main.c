/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
<<<<<<< HEAD
* Description        : æ°´å® - ä¸»æŽ§ (CH32V307VCT)
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
        // éžé˜»å¡ž: æ— æ•°æ®ç«‹å³è¿”å›ž
        HMI_task();
        esp_received();

        // 1ç§’å®šæ—¶ä»»åŠ¡
        static uint32_t t1s = 0;
        if (timer_cnt - t1s >= 1000) {
            t1s = timer_cnt;
            lcd_time();
            Leak_Task();        // å†…éƒ¨è‡ªé™1s, æ­¤å¤„å†é™1sç¡®ä¿ä¸é¢‘ç¹
        }

        // æ°´è´¨æ•°æ®(ä»…åœ¨æœ‰æ–°æ•°æ®æ—¶å¤„ç†)
        WQ_Task();

        // å¡”æ¥¼æ•°æ®æ›´æ–°
        if (g_tower_data_updated) {
            g_tower_data_updated = 0;
            UpdateTowerHMI();
        }

        // flagå¤„ç†(ä¸é˜»å¡ž)
        if (flag) {
            flag = 0;
        }

        Delay_Ms(1);  // 1msè½»é‡åŒ–, ä¸å†100%ç©ºè½¬
=======
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
    //lcd³õÊ¼
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
    //KEY³õÊ¼
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //pwm³õÊ¼

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
	//Ê±ÖÓÆµÂÊÄ¬ÈÏ96000000
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);	
    __enable_irq();   // ¿ªÆôÈ«¾ÖÖÐ¶Ï£¬È·±£ÖÐ¶ÏÏìÓ¦


    ADC_DMA_CONF();//ÎÂÃôµç×è²É¼¯
    GPIO_Toggle_INIT();//°åÔØÐ¡µÆÅÝ
    // initPwm();//pwm³õÊ¼»¯
    UART_HIM_Init();//´®¿Ú2_´®¿ÚÆÁÍ¨ÐÅ
    UART_ESP32_Init();//´®¿Ú3_espÍ¨ÐÅ
    TIM2_Init(960,100);//¶¨Ê±Æ÷ÖÐ¶Ï1ms
    initRingBuffer();		//³õÊ¼»¯»·ÐÎ»º³åÇø
	Input_Capture_Init( 0xFFFF, 48000-1 );//water_speed³õÊ¼»¯
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
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    }
}

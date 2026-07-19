<<<<<<< HEAD
#include "headfile.h"

// ============================================================
// TIM2: 1mså®šæ—¶ä¸­æ–­
// æä¾›å…¨å±€æ—¶é—´åŸºå‡† timer_cnt
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

// 1mså®šæ—¶ä¸­æ–­
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        timer_cnt++;
        speed_flag++;

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        // 1000msæ—¶é—´ç‰‡: å‘é€æ°´è´¨æ£€æµ‹è¯·æ±‚
        if (timer_cnt % 1000 == 0)
        {
            WQ_SendRequest();
        }

        // 1000msæ—¶é—´ç‰‡: è¶…æ—¶æ— è„‰å†²åˆ™æ˜¾ç¤º0æµé‡
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
=======

#include "headfile.h"

uint32_t timer_cnt=0;

uint32_t speed_flag =0;
char test1[100];


void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_Init(u16 arr,u16 psc)
{

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;


  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //Ê¹ÄÜTIM2Ê±ÖÓ

  TIM_TimeBaseStructure.TIM_Period = arr - 1;   //Ö¸¶¨ÏÂ´Î¸üÐÂÊÂ¼þÊ±Òª¼ÓÔØµ½»î¶¯×Ô¶¯ÖØÐÂ¼ÓÔØ¼Ä´æÆ÷ÖÐµÄÖÜÆÚÖµ¡£
  TIM_TimeBaseStructure.TIM_Prescaler =psc - 1; //Ö¸¶¨ÓÃÓÚ»®·ÖTIMÊ±ÖÓµÄÔ¤·ÖÆµÆ÷Öµ¡£
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //Ê±ÖÓ·ÖÆµÒò×Ó
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM¼ÆÊýÄ£Ê½£¬ÏòÉÏ¼ÆÊýÄ£Ê½
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î»

  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //Ê¹ÄÜTIM2ÖÐ¶Ï£¬ÔÊÐí¸üÐÂÖÐ¶Ï

  //³õÊ¼»¯TIM NVIC£¬ÉèÖÃÖÐ¶ÏÓÅÏÈ¼¶·Ö×é
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //TIM2ÖÐ¶Ï
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //ÉèÖÃÇÀÕ¼ÓÅÏÈ¼¶0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //ÉèÖÃÏìÓ¦ÓÅÏÈ¼¶3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //Ê¹ÄÜÍ¨µÀ1ÖÐ¶Ï
  NVIC_Init(&NVIC_InitStructure); //³õÊ¼»¯NVIC

  TIM_Cmd(TIM2, ENABLE); //TIM2Ê¹ÄÜ

}

void TIM2_IRQHandler(void)//1ms¶¨Ê±ÖÐ¶Ï
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //¼ì²éTIM2ÖÐ¶ÏÊÇ·ñ·¢Éú¡£
    {
      timer_cnt++;
      speed_flag++;
      
      TIM_ClearITPendingBit(TIM2,TIM_IT_Update);    //Çå³ýTIM2µÄÖÐ¶Ï¹ÒÆðÎ»¡£
      if(timer_cnt%5==0)//5msÇÐÆ¬ÈÎÎñÒª¶Ì
      {
        
      }
      if (timer_cnt%80==0)//1000msÇÐÆ¬ÈÎÎñÒª¶Ì
      {
        time_flag = 1;
        if(++water_speedflage ==1)
        {
          
          sprintf(test1, "t10.txt=\"0L/min\"");
          tjc_send_string(test1);

        }
      }


    }

}

>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb

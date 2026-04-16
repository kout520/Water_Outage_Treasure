#include "headfile.h"

// 水流传感器参数：450个脉冲=1升水
#define PULSES_PER_LITER 450

// 全局变量用于计算流速和流量
uint32_t total_pulses = 0;        // 总脉冲计数
uint32_t last_capture_time = 0;   // 上一次捕获时间
float flow_rate = 0.0;            // 流速(L/min)
float total_flow = 0.0;           // 总流量(L)
u8 water_speedflage = 0;
uint32_t time_interval;


char test[100];
void Input_Capture_Init( u16 arr, u16 psc )
{
	GPIO_InitTypeDef GPIO_InitStructure={0};
	TIM_ICInitTypeDef TIM_ICInitStructure={0};
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};
	NVIC_InitTypeDef NVIC_InitStructure={0};

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits( GPIOA, GPIO_Pin_8 );

	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter =  0x00;
	TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;

	TIM_PWMIConfig( TIM1, &TIM_ICInitStructure );

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig( TIM1, TIM_IT_CC1 | TIM_IT_CC2, ENABLE );

	TIM_SelectInputTrigger( TIM1, TIM_TS_TI1FP1 );
	TIM_SelectSlaveMode( TIM1, TIM_SlaveMode_Reset );
	TIM_SelectMasterSlaveMode( TIM1, TIM_MasterSlaveMode_Enable );
	TIM_Cmd( TIM1, ENABLE );
}



/*********************************************************************
 * @fn      TIM1_CC_IRQHandler
 *
 * @brief   This function handles TIM1  Capture Compare Interrupt exception.
 *
 * @return  none
 */
void TIM1_CC_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_CC_IRQHandler(void)
{
    if( TIM_GetITStatus( TIM1, TIM_IT_CC1 ) != RESET )
    {
        uint32_t current_time = TIM_GetCapture1(TIM1);
        
        // 使用timer_cnt计算时间间隔（考虑timer_cnt溢出）
        static uint32_t last_timer_cnt = 0;
        uint32_t current_timer_cnt = timer_cnt;
        uint32_t timer_interval;
        
        if(current_timer_cnt >= last_timer_cnt)
        {
            timer_interval = current_timer_cnt - last_timer_cnt;
        }
        else
        {
            // 处理timer_cnt溢出
            timer_interval = (0xFFFFFFFF - last_timer_cnt) + current_timer_cnt;
        }
        
        // 计算时间间隔（考虑定时器溢出）
  
        if(current_time >= last_capture_time)
        {
            time_interval = current_time - last_capture_time;
        }
        else
        {
            time_interval = (0xFFFF - last_capture_time) + current_time;
        }
        
        // 更新脉冲计数
        total_pulses++;
        
        // 计算总流量（升）
        total_flow = (float)total_pulses / PULSES_PER_LITER;
        
        // 计算流速（升/分钟）- 使用timer_cnt提供的时间基准
        if(timer_interval > 0)
        {
            water_speedflage = 0;
            // timer_cnt每ms加1，所以timer_interval的单位是ms
            float time_seconds = (float)timer_interval / 1000.0; // 转换为秒
            
            // 单个脉冲对应的水量（升）
            float water_per_pulse = 1.0 / PULSES_PER_LITER;

            // 流速 = 水量 / 时间 * 60（转换为升/分钟）
            flow_rate = (water_per_pulse / time_seconds) * 60.0;
            
            //printf("流速计算: %d L\r\n", timer_interval);
            sprintf(test, "t10.txt=\"%.2fL/min\"", flow_rate);
		    tjc_send_string(test);
            sprintf(test, "t11.txt=\"%.2fL\"", total_flow);
		    tjc_send_string(test);
        }
        
        last_capture_time = current_time;
        last_timer_cnt = current_timer_cnt;
               

    }

    if( TIM_GetITStatus( TIM1, TIM_IT_CC2 ) != RESET )
    {
    }

    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1 | TIM_IT_CC2);
}


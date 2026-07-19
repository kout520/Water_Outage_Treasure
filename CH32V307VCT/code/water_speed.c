#include "headfile.h"

<<<<<<< HEAD
// ============================================================
// 流量计: F=98*Q (F:频率Hz, Q:流量L/min)
// 每升脉冲数 = 98*60 = 5880, 流量范围: 0.3~5 L/min
// ============================================================
#define PULSES_PER_LITER 5880

// 全局变量 - 流量计量
uint32_t total_pulses      = 0;    // 累计脉冲数
uint32_t last_capture_time = 0;    // 上一次捕获时间
float    flow_rate         = 0.0f; // 瞬时流量(L/min)
float    total_flow        = 0.0f; // 累计流量(L)
u8       water_speedflage  = 0;
uint32_t time_interval;

// ============================================================
// 漏水检测参数
// ============================================================
#define LEAK_CHECK_INTERVAL_MS    60000   // 检测窗口(60秒)
#define LEAK_THRESHOLD_PULSES     3       // 阈值: 60秒内超过3个脉冲判定为漏水(更灵敏)
#define LEAK_FLOW_THRESHOLD       0.05f   // 流量阈值(L/min)
#define LEAK_CLEAR_COUNT          2       // 连续2次干净窗口才解除警报(滞回)
#define LEAK_COOLDOWN_MS          600000  // 告警冷却: 10分钟内不重复通知

// 漏水状态枚举
typedef enum {
    LEAK_STATUS_NORMAL  = 0,   // 正常
    LEAK_STATUS_WARNING = 1,   // 疑似漏水
    LEAK_STATUS_ALARM   = 2,   // 确认漏水
} LeakStatus_t;

// 漏水检测数据结构
typedef struct {
    uint32_t check_start_time;       // 当前检测窗口起始时间(ms)
    uint32_t pulse_count_in_window;  // 窗口内脉冲累计数
    uint8_t  system_active;          // 系统是否正在用水(1=用水中, 0=静默)
    LeakStatus_t status;             // 当前漏水状态
    uint32_t leak_duration_ms;       // 累计漏水时长(ms)
    float    leak_total_flow;        // 累计漏水量(L)
} LeakDetect_t;

static LeakDetect_t g_leak = {0};

// 流速法漏水检测 (正常流速: 0.2~0.4 L/min)
#define FLOW_MICRO_THRESHOLD      0.03f   // <0.03 = 微漏/滴水
#define FLOW_MICRO_DURATION_MS    30000   // 持续30秒判定
#define FLOW_SLOW_MIN             0.05f   // 慢漏下限
#define FLOW_SLOW_MAX             0.20f   // 慢漏上限(0.05~0.20之间)
#define FLOW_SLOW_DURATION_MS     900000  // 持续15分钟判定
static float g_flow_normal = 0.20f;        // 正常流速阈值, HMI可调
#define FLOW_IDLE_TIMEOUT_MS      30000   // 停水30秒后恢复静默检测

void Leak_SetNormalFlow(float f) { if (f >= 0.05f) g_flow_normal = f; }
float Leak_GetNormalFlow(void)    { return g_flow_normal; }

static uint32_t s_micro_start = 0;
static uint8_t  s_micro_timing = 0;
static uint32_t s_slow_start = 0;        // 慢漏计时起点
static uint8_t  s_slow_timing = 0;       // 慢漏计时中标志

char test[100];

// ============================================================
// TIM1输入捕获初始化
// PA8 = 流量计脉冲输入
// ============================================================
void Input_Capture_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_ICInitTypeDef TIM_ICInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x00;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;

    TIM_PWMIConfig(TIM1, &TIM_ICInitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM1, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

    TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
    TIM_Cmd(TIM1, ENABLE);
}

// ============================================================
// TIM1捕获中断: 每个脉冲触发一次
// 计算瞬时流量 + 累计流量 + 漏水检测脉冲累计
// ============================================================
void TIM1_CC_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_CC_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
    {
        uint32_t current_time = TIM_GetCapture1(TIM1);

        // 使用timer_cnt提供毫秒级时间基准
        static uint32_t last_timer_cnt = 0;
        uint32_t current_timer_cnt = timer_cnt;
        uint32_t timer_interval;

        if (current_timer_cnt >= last_timer_cnt)
            timer_interval = current_timer_cnt - last_timer_cnt;
        else
            timer_interval = (0xFFFFFFFF - last_timer_cnt) + current_timer_cnt; // 溢出处理

        // 计算定时器捕获间隔
        if (current_time >= last_capture_time)
            time_interval = current_time - last_capture_time;
        else
            time_interval = (0xFFFF - last_capture_time) + current_time;

        // 累计脉冲数
        total_pulses++;

        // 漏水检测: 系统不活跃时累计窗口内脉冲
        if (!g_leak.system_active) {
            g_leak.pulse_count_in_window++;
        }

        // 累计流量(L)
        total_flow = (float)total_pulses / PULSES_PER_LITER;

        // 计算瞬时流量(L/min)
        if (timer_interval > 0)
        {
            water_speedflage = 0;
            // timer_cnt每ms+1, timer_interval单位是ms
            float time_seconds = (float)timer_interval / 1000.0f;

            // 每个脉冲对应的水量(L)
            float water_per_pulse = 1.0f / PULSES_PER_LITER;

            // 流量 = 水量 / 时间 * 60 (转换为L/min)
            flow_rate = (water_per_pulse / time_seconds) * 60.0f;

            // 更新HMI显示
            sprintf(test, "t10.txt=\"%.2fL/min\"", flow_rate);
            tjc_send_string(test);
            sprintf(test, "t11.txt=\"%.2fL\"", total_flow);
            tjc_send_string(test);
        }

        last_capture_time = current_time;
        last_timer_cnt = current_timer_cnt;
    }

    if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
=======
// ˮ��������������450������=1��ˮ
#define PULSES_PER_LITER 450

// ȫ�ֱ������ڼ������ٺ�����
uint32_t total_pulses = 0;        // ���������
uint32_t last_capture_time = 0;   // ��һ�β���ʱ��
float flow_rate = 0.0;            // ����(L/min)
float total_flow = 0.0;           // ������(L)
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
        
        // ʹ��timer_cnt����ʱ����������timer_cnt�����
        static uint32_t last_timer_cnt = 0;
        uint32_t current_timer_cnt = timer_cnt;
        uint32_t timer_interval;
        
        if(current_timer_cnt >= last_timer_cnt)
        {
            timer_interval = current_timer_cnt - last_timer_cnt;
        }
        else
        {
            // ����timer_cnt���
            timer_interval = (0xFFFFFFFF - last_timer_cnt) + current_timer_cnt;
        }
        
        // ����ʱ���������Ƕ�ʱ�������
  
        if(current_time >= last_capture_time)
        {
            time_interval = current_time - last_capture_time;
        }
        else
        {
            time_interval = (0xFFFF - last_capture_time) + current_time;
        }
        
        // �����������
        total_pulses++;
        
        // ����������������
        total_flow = (float)total_pulses / PULSES_PER_LITER;
        
        // �������٣���/���ӣ�- ʹ��timer_cnt�ṩ��ʱ���׼
        if(timer_interval > 0)
        {
            water_speedflage = 0;
            // timer_cntÿms��1������timer_interval�ĵ�λ��ms
            float time_seconds = (float)timer_interval / 1000.0; // ת��Ϊ��
            
            // ���������Ӧ��ˮ��������
            float water_per_pulse = 1.0 / PULSES_PER_LITER;

            // ���� = ˮ�� / ʱ�� * 60��ת��Ϊ��/���ӣ�
            flow_rate = (water_per_pulse / time_seconds) * 60.0;
            
            //printf("���ټ���: %d L\r\n", timer_interval);
            sprintf(test, "t10.txt=\"%.2fL/min\"", flow_rate);
		    tjc_send_string(test);
            sprintf(test, "t11.txt=\"%.2fL\"", total_flow);
		    tjc_send_string(test);
        }
        
        last_capture_time = current_time;
        last_timer_cnt = current_timer_cnt;
               

    }

    if( TIM_GetITStatus( TIM1, TIM_IT_CC2 ) != RESET )
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    {
    }

    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1 | TIM_IT_CC2);
}

<<<<<<< HEAD
// ============================================================
// 漏水检测逻辑
// ============================================================

// 设置系统用水状态(上层业务调用)
// active: 1=正在用水(不检测漏水), 0=静默(开启漏水检测)
void Leak_SetSystemActive(uint8_t active)
{
    g_leak.system_active = active;
    if (!active) {
        // 进入静默期, 重置检测窗口
        g_leak.check_start_time      = timer_cnt;
        g_leak.pulse_count_in_window = 0;
    }
}

// 全局漏水警报标志(供main.c查询)
uint8_t g_leak_alarm_active = 0;
static uint8_t  leak_clear_cnt = 0;      // 滞回计数
static uint32_t leak_last_alert = 0xFFFFFFFF - 600000; // 初始化确保首次可触发

// 前置声明
static uint8_t Leak_DetectByFlowRate(void);

LeakStatus_t Leak_Detect(void)
{
    if (g_leak.system_active) {
        if (g_leak_alarm_active) {
            g_leak_alarm_active = 0;
            leak_clear_cnt = 0;
            GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
            send_leak_to_esp32(0);
            tjc_send_string("t4.txt=\"安全\"");
        }
        g_leak.status = LEAK_STATUS_NORMAL;
        return LEAK_STATUS_NORMAL;
    }

    uint32_t elapsed = timer_cnt - g_leak.check_start_time;

    if (elapsed >= LEAK_CHECK_INTERVAL_MS) {
        if (g_leak.pulse_count_in_window > LEAK_THRESHOLD_PULSES) {
            // 检测到漏水!
            leak_clear_cnt = 0;
            float leak_flow_in_window =
                (float)g_leak.pulse_count_in_window / PULSES_PER_LITER;
            g_leak.leak_total_flow += leak_flow_in_window;
            g_leak.leak_duration_ms += elapsed;
            g_leak.status = LEAK_STATUS_ALARM;

            if (!g_leak_alarm_active) {
                g_leak_alarm_active = 1;
                GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);

                // 冷却期内不发通知, 避免重复骚扰
                if (timer_cnt - leak_last_alert > LEAK_COOLDOWN_MS) {
                    leak_last_alert = timer_cnt;
                    send_leak_to_esp32(1);
                }

                char buf[64];
                sprintf(buf, "t4.txt=\"警告漏水!\"");
                tjc_send_string(buf);
                sprintf(buf, "t5.txt=\"%.2fL\"", g_leak.leak_total_flow);
                tjc_send_string(buf);
            }
        } else {
            // 干净窗口 → 滞回计数
            if (g_leak_alarm_active) {
                leak_clear_cnt++;
                if (leak_clear_cnt >= LEAK_CLEAR_COUNT) {
                    g_leak_alarm_active = 0;
                    leak_clear_cnt = 0;
                    GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
                    send_leak_to_esp32(0);
                    tjc_send_string("t4.txt=\"安全\"");
                    g_leak.leak_duration_ms = 0;
                    g_leak.leak_total_flow  = 0.0f;
                }
            }
            g_leak.status = LEAK_STATUS_NORMAL;
        }

        g_leak.check_start_time      = timer_cnt;
        g_leak.pulse_count_in_window = 0;
    }

    return g_leak.status;
}

void Leak_Task(void)
{
    static uint32_t last_check = 0;
    if (timer_cnt - last_check >= 1000) {
        last_check = timer_cnt;

        // 自动判断是否在用水: 流量>0.20 = 有人用水
        static uint32_t idle_start = 0;
        if (flow_rate > g_flow_normal) {
            Leak_SetSystemActive(1);
            idle_start = 0;
        } else if (g_leak.system_active && idle_start == 0) {
            idle_start = timer_cnt;
        } else if (g_leak.system_active && (timer_cnt - idle_start) > FLOW_IDLE_TIMEOUT_MS) {
            Leak_SetSystemActive(0);  // 停水30秒, 恢复漏水监控
        }

        Leak_Detect();

        if (Leak_DetectByFlowRate()) {
            if (!g_leak_alarm_active) {
                g_leak_alarm_active = 1;
                GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
                if (timer_cnt - leak_last_alert > LEAK_COOLDOWN_MS) {
                    leak_last_alert = timer_cnt;
                    send_leak_to_esp32(1);
                }
                tjc_send_string("t4.txt=\"微漏警告!\"");
            }
        }
    }
}

// 获取漏水统计信息
void Leak_GetInfo(float *leak_flow, uint32_t *duration_ms)
{
    if (leak_flow)   *leak_flow   = g_leak.leak_total_flow;
    if (duration_ms) *duration_ms = g_leak.leak_duration_ms;
}

// 重置漏水检测(清零所有统计数据)
void Leak_Reset(void)
{
    g_leak.pulse_count_in_window = 0;
    g_leak.leak_total_flow       = 0.0f;
    g_leak.leak_duration_ms      = 0;
    g_leak.status                = LEAK_STATUS_NORMAL;
    g_leak.check_start_time      = timer_cnt;
}

// 流速法漏水检测: 两个判定区间
//   A: <0.03 L/min 持续30s → 微漏(滴水)
//   B: 0.05~0.20 L/min 持续15min → 慢漏(渗漏)
//   正常: 0.20~0.40 L/min → 不告警
static uint8_t Leak_DetectByFlowRate(void)
{
    // 条件A: 极低流量 → 微漏
    if (flow_rate > 0.001f && flow_rate < FLOW_MICRO_THRESHOLD) {
        if (!s_micro_timing) {
            s_micro_start  = timer_cnt;
            s_micro_timing = 1;
        } else if ((timer_cnt - s_micro_start) >= FLOW_MICRO_DURATION_MS) {
            return 1;
        }
    } else {
        s_micro_timing = 0;
    }

    // 条件B: 低于正常但非零 → 慢漏
    if (flow_rate > FLOW_SLOW_MIN && flow_rate < FLOW_SLOW_MAX) {
        if (!s_slow_timing) {
            s_slow_start  = timer_cnt;
            s_slow_timing = 1;
        } else if ((timer_cnt - s_slow_start) >= FLOW_SLOW_DURATION_MS) {
            return 1;
        }
    } else {
        s_slow_timing = 0;
    }

    return 0;
}
=======
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb

#include "headfile.h"

// 流量计: F=98*Q, 5880脉冲/L
#define PULSES_PER_LITER 5880

// 全局变量
uint32_t total_pulses = 0;
uint32_t last_capture_time = 0;
float flow_rate = 0.0f;
float total_flow = 0.0f;
u8 water_speedflage = 0;
uint32_t time_interval;

// 漏水检测参数
#define LEAK_CHECK_INTERVAL_MS    60000
#define LEAK_THRESHOLD_PULSES     3
#define LEAK_CLEAR_COUNT          2
#define LEAK_COOLDOWN_MS          600000

// 流速法检测
#define FLOW_MICRO_THRESHOLD      0.03f
#define FLOW_MICRO_DURATION_MS    30000
#define FLOW_SLOW_MIN             0.05f
#define FLOW_SLOW_MAX             0.20f
#define FLOW_SLOW_DURATION_MS     900000
#define FLOW_IDLE_TIMEOUT_MS      30000

static float g_flow_normal = 0.20f;

// 微漏/慢漏计时
static uint32_t s_micro_start = 0;
static uint8_t  s_micro_timing = 0;
static uint32_t s_slow_start = 0;
static uint8_t  s_slow_timing = 0;

// 漏水数据
typedef enum { LEAK_NORMAL = 0, LEAK_WARNING = 1, LEAK_ALARM = 2 } LeakStatus_t;
typedef struct {
    uint32_t check_start_time;
    uint32_t pulse_count_in_window;
    uint8_t  system_active;
    LeakStatus_t status;
    uint32_t leak_duration_ms;
    float    leak_total_flow;
} LeakDetect_t;

static LeakDetect_t g_leak = {0};
uint8_t g_leak_alarm_active = 0;
static uint8_t  leak_clear_cnt = 0;
static uint32_t leak_last_alert = 0xFFFFFFFF - 600000;

char test[100];

// 前置声明
static uint8_t Leak_DetectByFlowRate(void);

void Input_Capture_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    TIM_ICInitTypeDef TIM_ICInitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};
    NVIC_InitTypeDef NVIC_InitStructure={0};
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

void TIM1_CC_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_CC_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
    {
        uint32_t current_time = TIM_GetCapture1(TIM1);
        static uint32_t last_timer_cnt = 0;
        uint32_t current_timer_cnt = timer_cnt;
        uint32_t timer_interval;

        if (current_timer_cnt >= last_timer_cnt)
            timer_interval = current_timer_cnt - last_timer_cnt;
        else
            timer_interval = (0xFFFFFFFF - last_timer_cnt) + current_timer_cnt;

        if (current_time >= last_capture_time)
            time_interval = current_time - last_capture_time;
        else
            time_interval = (0xFFFF - last_capture_time) + current_time;

        total_pulses++;

        // 漏水检测: 静默时累计脉冲
        if (!g_leak.system_active)
            g_leak.pulse_count_in_window++;

        total_flow = (float)total_pulses / PULSES_PER_LITER;

        if (timer_interval > 0)
        {
            water_speedflage = 0;
            float time_seconds = (float)timer_interval / 1000.0f;
            float water_per_pulse = 1.0f / PULSES_PER_LITER;
            flow_rate = (water_per_pulse / time_seconds) * 60.0f;

            sprintf(test, "t10.txt=\"%.2fL/min\"", flow_rate);
            tjc_send_string(test);
            sprintf(test, "t11.txt=\"%.2fL\"", total_flow);
            tjc_send_string(test);
        }

        last_capture_time = current_time;
        last_timer_cnt = current_timer_cnt;
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET) {}
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1 | TIM_IT_CC2);
}

// ============================================================
// 漏水检测
// ============================================================

void Leak_SetSystemActive(uint8_t active)
{
    g_leak.system_active = active;
    if (!active) {
        g_leak.check_start_time = timer_cnt;
        g_leak.pulse_count_in_window = 0;
    }
}

void Leak_SetNormalFlow(float f) { if (f >= 0.05f) g_flow_normal = f; }
float Leak_GetNormalFlow(void)    { return g_flow_normal; }

static LeakStatus_t Leak_Detect(void)
{
    if (g_leak.system_active) {
        if (g_leak_alarm_active) {
            g_leak_alarm_active = 0;
            leak_clear_cnt = 0;
            GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
            send_leak_to_esp32(0);
            tjc_send_string("t4.txt=\"安全\"");
        }
        return LEAK_NORMAL;
    }

    uint32_t elapsed = timer_cnt - g_leak.check_start_time;
    if (elapsed >= LEAK_CHECK_INTERVAL_MS) {
        if (g_leak.pulse_count_in_window > LEAK_THRESHOLD_PULSES) {
            leak_clear_cnt = 0;
            float leak_vol = (float)g_leak.pulse_count_in_window / PULSES_PER_LITER;
            g_leak.leak_total_flow += leak_vol;
            g_leak.leak_duration_ms += elapsed;
            if (!g_leak_alarm_active) {
                g_leak_alarm_active = 1;
                GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
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
            if (g_leak_alarm_active) {
                leak_clear_cnt++;
                if (leak_clear_cnt >= LEAK_CLEAR_COUNT) {
                    g_leak_alarm_active = 0;
                    leak_clear_cnt = 0;
                    GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
                    send_leak_to_esp32(0);
                    tjc_send_string("t4.txt=\"安全\"");
                    g_leak.leak_duration_ms = 0;
                    g_leak.leak_total_flow = 0;
                }
            }
        }
        g_leak.check_start_time = timer_cnt;
        g_leak.pulse_count_in_window = 0;
    }
    return g_leak.status;
}

static uint8_t Leak_DetectByFlowRate(void)
{
    if (flow_rate > 0.001f && flow_rate < FLOW_MICRO_THRESHOLD) {
        if (!s_micro_timing) { s_micro_start = timer_cnt; s_micro_timing = 1; }
        else if ((timer_cnt - s_micro_start) >= FLOW_MICRO_DURATION_MS) return 1;
    } else { s_micro_timing = 0; }

    if (flow_rate > FLOW_SLOW_MIN && flow_rate < FLOW_SLOW_MAX) {
        if (!s_slow_timing) { s_slow_start = timer_cnt; s_slow_timing = 1; }
        else if ((timer_cnt - s_slow_start) >= FLOW_SLOW_DURATION_MS) return 1;
    } else { s_slow_timing = 0; }

    return 0;
}

void Leak_Task(void)
{
    static uint32_t last = 0;
    if (timer_cnt - last >= 1000) {
        last = timer_cnt;

        // 自动判断用水
        static uint32_t idle_start = 0;
        if (flow_rate > g_flow_normal) {
            Leak_SetSystemActive(1);
            idle_start = 0;
        } else if (g_leak.system_active && idle_start == 0) {
            idle_start = timer_cnt;
        } else if (g_leak.system_active && (timer_cnt - idle_start) > FLOW_IDLE_TIMEOUT_MS) {
            Leak_SetSystemActive(0);
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

void Leak_Reset(void)
{
    g_leak.pulse_count_in_window = 0;
    g_leak.leak_total_flow = 0;
    g_leak.leak_duration_ms = 0;
    g_leak.check_start_time = timer_cnt;
}

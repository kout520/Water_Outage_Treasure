#include "headfile.h"
#include "hc_sr04.h"

// ============================================================
// HC-SR04 (TIM4 1MHz + 溢出中断, 测5次取平均)
// ============================================================

static volatile uint16_t msHcCount;

void HC_SR04_Init(void)
{
    GPIO_InitTypeDef s = {0};
    TIM_TimeBaseInitTypeDef ts = {0};
    NVIC_InitTypeDef ns = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    s.GPIO_Pin = TRIG_PIN;
    s.GPIO_Mode = GPIO_Mode_Out_PP;
    s.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TRIG_PORT, &s);
    TRIG_Low();

    s.GPIO_Pin = ECHO_PIN;
    s.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(ECHO_PORT, &s);

    ts.TIM_Period = 1000 - 1;
    ts.TIM_Prescaler = 72 - 1;
    ts.TIM_ClockDivision = TIM_CKD_DIV1;
    ts.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &ts);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, DISABLE);

    ns.NVIC_IRQChannel = TIM4_IRQn;
    ns.NVIC_IRQChannelPreemptionPriority = 2;
    ns.NVIC_IRQChannelSubPriority = 1;
    ns.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&ns);
}

static void Timer4_Start(void)
{
    TIM_SetCounter(TIM4, 0);
    msHcCount = 0;
    TIM_Cmd(TIM4, ENABLE);
}

static uint32_t Timer4_Stop(void)
{
    TIM_Cmd(TIM4, DISABLE);
    return msHcCount * 1000 + TIM_GetCounter(TIM4);
}

static void sortFloats(float *arr, int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (arr[i] > arr[j]) {
                float t = arr[i]; arr[i] = arr[j]; arr[j] = t;
            }
}

float HC_SR04_GetDistance_cm(void)
{
    #define N 10
    float samples[N];
    int i, valid = 0;

    // 温度补偿: 声速 = 331.3 + 0.606 * T(℃) m/s
    float temp = AHT20_GetTemperature();
    float speed = 331.3f + 0.606f * temp;  // m/s
    float factor = speed * 100.0f / 2.0f;  // cm/s / 2(往返) = cm per 1e6 us?
    // pulse_us * speed(m/s) * 100(cm/m) / 2(round) / 1000000(us->s)
    // = pulse_us * speed * 100.0f / 2000000.0f
    // = pulse_us * speed / 20000.0f
    float k = speed / 20000.0f;  // pulse_us * k = distance_cm

    for (i = 0; i < N; i++)
    {
        TRIG_High();
        Delay_Us(20);
        TRIG_Low();

        uint32_t dl = timer_cnt + 60;
        while (ECHO_Read() == 0)
            if (timer_cnt > dl) goto done;

        Timer4_Start();

        dl = timer_cnt + 60;
        while (ECHO_Read() == 1)
            if (timer_cnt > dl) { TIM_Cmd(TIM4, DISABLE); goto done; }

        samples[valid++] = (float)Timer4_Stop() * k;
        Delay_Ms(15);
    }

done:
    if (valid < 3) return -1.0f;

    // 排序去极值: 去掉最小2个和最大2个, 取中间平均
    sortFloats(samples, valid);
    int trim = valid / 5;  // 去掉20%极值
    if (trim < 1) trim = 1;
    float sum = 0;
    for (i = trim; i < valid - trim; i++)
        sum += samples[i];
    return sum / (float)(valid - 2 * trim);
}

float HC_SR04_GetWaterLevel_m(void)
{
    float d = HC_SR04_GetDistance_cm();
    if (d < 0) return 0;
    float m = TANK_HEIGHT_M - d / 100.0f;
    if (m < 0) m = 0;
    if (m > TANK_HEIGHT_M) m = TANK_HEIGHT_M;
    return m;
}

void TIM4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
        msHcCount++;
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}

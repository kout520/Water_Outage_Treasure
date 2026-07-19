#include "headfile.h"

// ============================================================
// 旋转编码器 (CLK=PA6, DT=PA7)
// 下降沿触发 + 5ms去抖 + 方向滤波
// ============================================================

#define ENC_CLK_PORT  GPIOA
#define ENC_CLK_PIN   GPIO_Pin_6
#define ENC_DT_PORT   GPIOA
#define ENC_DT_PIN    GPIO_Pin_7

static volatile int g_enc_count = 50;

void Encoder_Init(void)
{
    GPIO_InitTypeDef s = {0};
    EXTI_InitTypeDef es = {0};
    NVIC_InitTypeDef ns = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    s.GPIO_Pin = ENC_CLK_PIN | ENC_DT_PIN;
    s.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &s);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
    es.EXTI_Line = EXTI_Line6;
    es.EXTI_Mode = EXTI_Mode_Interrupt;
    es.EXTI_Trigger = EXTI_Trigger_Falling;  // 仅下降沿, 半中断量
    es.EXTI_LineCmd = ENABLE;
    EXTI_Init(&es);

    ns.NVIC_IRQChannel = EXTI9_5_IRQn;
    ns.NVIC_IRQChannelPreemptionPriority = 3;
    ns.NVIC_IRQChannelSubPriority = 0;
    ns.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&ns);
}

int Encoder_GetCount(void)  { return g_enc_count; }
void Encoder_ResetCount(void){ g_enc_count = 50; }

int Encoder_GetSpeed(void)
{
    if (g_enc_count < 0)  g_enc_count = 0;
    if (g_enc_count > 100) g_enc_count = 100;
    return g_enc_count;
}

void EXTI9_5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        static uint32_t last_t = 0;
        static int last_dir = 0, same_cnt = 0;
        uint32_t now = timer_cnt;

        // 5ms去抖
        if (now - last_t > 5)
        {
            last_t = now;
            // 下降沿: DT=高→CW, DT=低→CCW
            int dir = GPIO_ReadInputDataBit(ENC_DT_PORT, ENC_DT_PIN) ? 1 : -1;

            // 方向滤波: 连续2次同向才生效
            if (dir == last_dir) {
                same_cnt++;
                if (same_cnt >= 1) {
                    g_enc_count += dir;
                    same_cnt = 0;
                }
            } else {
                last_dir = dir;
                same_cnt = 1;
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
}

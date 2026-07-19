#include "headfile.h"

// ============================================================
// 旋转编码器 (CLK=PA6, DT=PA7)
// 双边沿触发 + 10ms去抖 + 正交解码
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
    es.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  // 双边沿
    es.EXTI_LineCmd = ENABLE;
    EXTI_Init(&es);

    ns.NVIC_IRQChannel = EXTI9_5_IRQn;
    ns.NVIC_IRQChannelPreemptionPriority = 3;
    ns.NVIC_IRQChannelSubPriority = 0;
    ns.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&ns);
}

int Encoder_GetCount(void)   { return g_enc_count; }
void Encoder_ResetCount(void) { g_enc_count = 50; }

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
        uint32_t now = timer_cnt;

        if (now - last_t > 10)  // 10ms去抖
        {
            last_t = now;

            // 正交解码: CLK == DT → 逆时针, CLK != DT → 顺时针
            uint8_t clk = GPIO_ReadInputDataBit(ENC_CLK_PORT, ENC_CLK_PIN);
            uint8_t dt  = GPIO_ReadInputDataBit(ENC_DT_PORT, ENC_DT_PIN);

            if (clk == dt)
                g_enc_count--;   // CCW
            else
                g_enc_count++;   // CW
        }
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
}

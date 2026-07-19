/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Description        : 水宝 - 塔楼水箱监测 (CH32V307VCT_1)
*******************************************************************************/

#include "debug.h"
#include "headfile.h"
#include "hc_sr04.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    __enable_irq();

    TIM2_Init(960, 100);

    OLED_I2C_Init();
    OLED_Init();
    OLED_Clear();
    OLED_ShowStr(0, 0, (uint8_t*)"Tower Monitor", 1);

    HC_SR04_Init();
    AHT20_Init();
    Pump_Init();
    Encoder_Init();
    Zigbee_Init(115200);

    printf("Tower Ready\r\n");

    int last_speed = -1;
    float g_dist = 0;

    while (1)
    {
        AHT20_Task();

        // HC-SR04测距(2秒一次) → 水位 = 27.0 - 距离
        #define TANK_DEPTH 27.0f
        static uint32_t last_sr = 0;
        if (timer_cnt - last_sr >= 2000) {
            last_sr = timer_cnt;
            float d = HC_SR04_GetDistance_cm();
            if (d > 0) {
                float level = TANK_DEPTH - d;
                g_dist = (level < 0) ? 0 : level;
            }
        }

        // OLED 200ms刷新
        static uint32_t last_oled = 0;
        if (timer_cnt - last_oled >= 200) {
            last_oled = timer_cnt;

            float temp  = AHT20_GetTemperature();
            float hum   = AHT20_GetHumidity();
            int speed   = Encoder_GetSpeed();

            if (speed != last_speed) {
                last_speed = speed;
                Pump_SetSpeed(speed);
                if (speed > 0) Pump_Start();
                else Pump_Stop();
            }

            char buf[32];
            snprintf(buf, sizeof(buf), "Temp:  %.1fC   ", temp);
            OLED_ShowStr(0, 0, (uint8_t*)buf, 1);
            snprintf(buf, sizeof(buf), "Hum:   %.1f%%   ", hum);
            OLED_ShowStr(0, 2, (uint8_t*)buf, 1);
            snprintf(buf, sizeof(buf), "Level: %.1fcm   ", g_dist);
            OLED_ShowStr(0, 4, (uint8_t*)buf, 1);
            snprintf(buf, sizeof(buf), "Pump:  %d%%   ", speed);
            OLED_ShowStr(0, 6, (uint8_t*)buf, 1);
        }

        // Zigbee 5秒发送
        static uint32_t last_zig = 0;
        if (timer_cnt - last_zig >= 5000) {
            last_zig = timer_cnt;
            Zigbee_SendSensorData();
        }

        if (g_zigbee_rx_ready) {
            Zigbee_ParseCommand(g_zigbee_rx_buf);
            g_zigbee_rx_ready = 0;
        }

        Delay_Ms(10);
    }
}

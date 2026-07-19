#include "headfile.h"

// ============================================================
// 水质传感器 (TDS/电导率/盐度/比重/温度/硬度)
// 协议: 发送 A0 00 00 00 00 A0 -> 响应 16字节帧(AA头+14数据+checksum尾)
// USART1: TX=PA9, RX=PA10
// ============================================================

// 全局变量
WaterQuality_t g_water_quality = {0};
uint8_t g_wq_data_ready = 0;

// 接收缓冲区
static uint8_t  rx_buf[WQ_FRAME_LEN];
static uint8_t  rx_index = 0;
static uint8_t  rx_in_progress = 0;

// 发送指令: A0 00 00 00 00 A0
static const uint8_t WQ_SEND_CMD[] = {0xA0, 0x00, 0x00, 0x00, 0x00, 0xA0};
#define WQ_SEND_LEN sizeof(WQ_SEND_CMD)

// ============================================================
// 月水费计量 (费率: 0.35分/L, 月初重置)
// ============================================================
#define WATER_RATE_FEN_PER_L   0.35f        // 0.35 分/升
static float g_monthly_start_flow = 0.0f;   // 月初累计流量(L)

// ============================================================
// 初始化 USART1 (PA9=TX, PA10=RX)
// ============================================================
void WQ_Init(uint32_t baud_rate)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* USART1 TX-->PA.9   RX-->PA.10 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_InitStructure.USART_BaudRate = baud_rate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// ============================================================
// 发送检测指令到传感器
// ============================================================
void WQ_SendRequest(void)
{
    uint8_t i;
    for (i = 0; i < WQ_SEND_LEN; i++)
    {
        USART_SendData(USART1, WQ_SEND_CMD[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}

// ============================================================
// 解析响应帧
// AA [14字节数据] checksum  (总计16字节)
// 数据区 7 x uint16 大端:
//   [0] TDS, [1]盐度x100, [2]电导率, [3]保留,
//   [4]比重x10000, [5]温度x10, [6]硬度
// ============================================================
void WQ_ParseResponse(uint8_t *data)
{
    // 校验帧头
    if (data[0] != WQ_FRAME_HEADER)
        return;

    // 累加和校验: 前15字节求和, 低8位 == 帧尾
    {
        uint8_t sum = 0;
        uint8_t i;
        for (i = 0; i < WQ_FRAME_LEN - 1; i++)
        {
            sum += data[i];
        }
        if (sum != data[WQ_FRAME_LEN - 1])
            return;
    }

    // data[1]~data[14] 共14字节 = 7个uint16大端
    g_water_quality.tds            = ((uint16_t)data[1]  << 8) | data[2];
    g_water_quality.salinity_x100  = ((uint16_t)data[3]  << 8) | data[4];
    g_water_quality.conductivity   = ((uint16_t)data[5]  << 8) | data[6];
    g_water_quality.reserved       = ((uint16_t)data[7]  << 8) | data[8];
    g_water_quality.sg_x10000      = ((uint16_t)data[9]  << 8) | data[10];
    g_water_quality.temp_x10       = ((uint16_t)data[11] << 8) | data[12];
    g_water_quality.hardness       = ((uint16_t)data[13] << 8) | data[14];

    g_wq_data_ready = 1;
}

// ============================================================
// 更新HMI显示 + 发送到ESP32
// ============================================================
void WQ_UpdateDisplay(void)
{
    if (!g_wq_data_ready)
        return;

    char buf[64];

    // TDS
    sprintf(buf, "t20.txt=\"%dppm\"", g_water_quality.tds);
    tjc_send_string(buf);

    // 电导率
    sprintf(buf, "t21.txt=\"%dus/cm\"", g_water_quality.conductivity);
    tjc_send_string(buf);

    // 盐度
    sprintf(buf, "t22.txt=\"%d.%02d %%\"",
            g_water_quality.salinity_x100 / 100,
            g_water_quality.salinity_x100 % 100);
    tjc_send_string(buf);

    // 比重
    sprintf(buf, "t23.txt=\"%d.%04d\"",
            g_water_quality.sg_x10000 / 10000,
            g_water_quality.sg_x10000 % 10000);
    tjc_send_string(buf);

    // 温度
    sprintf(buf, "t13.txt=\"%d.%d\"",
            g_water_quality.temp_x10 / 10,
            g_water_quality.temp_x10 % 10);
    tjc_send_string(buf);

    // 硬度
    sprintf(buf, "t24.txt=\"%dppm\"", g_water_quality.hardness);
    tjc_send_string(buf);

    // 月水费 (费率0.35分/L)
    sprintf(buf, "t12.txt=\"%.2f\"", WQ_GetMonthlyCost());
    tjc_send_string(buf);

    // 同步发送水质+流量数据到ESP32 -> 上传网站
    send_water_quality_to_esp32();

    g_wq_data_ready = 0;
}

// ============================================================
// 水质任务(主循环调用)
// ============================================================
void WQ_Task(void)
{
    if (g_wq_data_ready)
    {
        WQ_UpdateDisplay();
    }
}

// ============================================================
// 月初重置: 将当前累计流量记为起点，月水费从零开始
// ============================================================
void WQ_ResetMonthlyCost(void)
{
    g_monthly_start_flow = total_flow;
}

// ============================================================
// 获取月水费 (元)
// 月流量 = 当前累计流量 - 月初累计流量
// 水费   = 月流量(L) x 0.35分/L / 100
// ============================================================
float WQ_GetMonthlyCost(void)
{
    float monthly_flow = total_flow - g_monthly_start_flow;
    if (monthly_flow < 0)
        monthly_flow = 0;   // 防异常
    return monthly_flow * WATER_RATE_FEN_PER_L / 100.0f;
}

// ============================================================
// USART1 接收中断 (水质传感器)
// ============================================================
void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART1) & 0xFF;

        if (!rx_in_progress)
        {
            // 等待帧头 AA
            if (data == WQ_FRAME_HEADER)
            {
                rx_buf[0] = data;
                rx_index = 1;
                rx_in_progress = 1;
            }
        }
        else
        {
            if (rx_index < WQ_FRAME_LEN)
            {
                rx_buf[rx_index++] = data;

                // 收满16字节, 交给解析函数做checksum校验
                if (rx_index >= WQ_FRAME_LEN)
                {
                    WQ_ParseResponse(rx_buf);
                    rx_in_progress = 0;
                    rx_index = 0;
                }
            }
            else
            {
                // 异常保护
                rx_in_progress = 0;
                rx_index = 0;
            }
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

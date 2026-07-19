#include "headfile.h"

// ============================================================
// Zigbee模块 (UART4: PC10=TX, PC11=RX)
// 接收塔楼水位/湿度/温度/水泵状态
// 发送水泵/阀门指令到塔楼
// ============================================================

// 塔楼数据
float g_tower_water_level   = 0.0f;
float g_tower_humidity      = 0.0f;
float g_tower_temperature   = 25.0f;
int   g_tower_pump_speed    = 0;
uint8_t g_tower_data_updated = 0;

// 接收缓冲区
static char rx_buf[128];
static uint8_t rx_idx = 0;

// ============================================================
// UART4初始化
// ============================================================
void Zigbee_Init(uint32_t baud_rate)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

    /* UART4 TX-->PC.10  RX-->PC.11 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baud_rate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(UART4, &USART_InitStructure);

    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART4, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// ============================================================
// 发送
// ============================================================
static void Zigbee_SendChar(char ch)
{
    USART_SendData(UART4, (uint8_t)ch);
    while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
}

void Zigbee_SendString(char* str)
{
    while (*str != '\0' && str != 0)
        Zigbee_SendChar(*str++);
}

// 发送水泵调速指令到塔楼
void Zigbee_SendPumpSpeed(uint8_t speed)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "@PUMP:%d\r\n", speed);
    Zigbee_SendString(buf);
}

// 发送阀门指令到塔楼
void Zigbee_SendValve(uint8_t open)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "@VALVE:%d\r\n", open);
    Zigbee_SendString(buf);
}

// ============================================================
// 解析塔楼发来的数据
// 格式: @LEVEL:水位,湿度,温度,水泵转速\r\n
// ============================================================
void Zigbee_ParseCommand(char* cmd)
{
    if (strncmp(cmd, "@LEVEL:", 7) == 0)
    {
        // 解析: @LEVEL:4.32,65.0,26.5,50
        char* ptr = cmd + 7;
        float level = 0, humidity = 0, temp = 25;
        int pump = 0;

        level    = (float)atof(ptr);
        ptr = strchr(ptr, ','); if (ptr) {
            humidity = (float)atof(ptr + 1);
            ptr = strchr(ptr + 1, ','); if (ptr) {
                temp = (float)atof(ptr + 1);
                ptr = strchr(ptr + 1, ','); if (ptr) {
                    pump = atoi(ptr + 1);
                }
            }
        }

        g_tower_water_level  = level;
        g_tower_humidity     = humidity;
        g_tower_temperature  = temp;
        g_tower_pump_speed   = pump;
        g_tower_data_updated = 1;
    }
}

// ============================================================
// Zigbee任务(主循环调用)
// ============================================================
void Zigbee_Task(void)
{
    // 解析收到的命令
    if (rx_idx > 0)
    {
        // 由中断驱动, 此处无需主动读取
    }
}

// ============================================================
// UART4接收中断
// ============================================================
void UART4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART4_IRQHandler(void)
{
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(UART4) & 0xFF;

        if (data == '@') rx_idx = 0;

        if (rx_idx < sizeof(rx_buf) - 1)
        {
            rx_buf[rx_idx++] = data;

            if (data == '\n')
            {
                rx_buf[rx_idx] = '\0';
                Zigbee_ParseCommand(rx_buf);
                rx_idx = 0;
            }
        }
        else { rx_idx = 0; }

        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
    }
}

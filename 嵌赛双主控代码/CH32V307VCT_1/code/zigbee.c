#include "headfile.h"

// ============================================================
// Zigbee模块 (UART4: PC10=TX, PC11=RX)
// 与主CH32进行无线通信
// ============================================================

// 接收缓冲区
char g_zigbee_rx_buf[128];
uint8_t g_zigbee_rx_ready = 0;
static uint8_t rx_idx = 0;

void Zigbee_Init(uint32_t baud_rate)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

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
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

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

// 发送水位+温湿度+水泵状态
void Zigbee_SendSensorData(void)
{
    char buf[120];
    snprintf(buf, sizeof(buf), "@LEVEL:%.2f,%.1f,%.1f,%d\r\n",
             HC_SR04_GetWaterLevel_m(),
             Humidity_GetPercent(),
             Temperature_GetCelsius(),
             Pump_GetSpeed());
    Zigbee_SendString(buf);
}

// Zigbee接收命令解析
void Zigbee_ParseCommand(char* cmd)
{
    // @PUMP:50 水泵调速
    if (strncmp(cmd, "@PUMP:", 6) == 0)
    {
        int speed = atoi(cmd + 6);
        if (speed >= 0 && speed <= 100) {
            Pump_SetSpeed((uint8_t)speed);
            if (speed > 0) Pump_Start();
            else Pump_Stop();
        }
    }
    // @VALVE:1 阀门开, @VALVE:0 阀门关
    else if (strncmp(cmd, "@VALVE:", 7) == 0)
    {
        if (cmd[7] == '1') GPIO_SetBits(GPIOE, GPIO_Pin_2);
        else              GPIO_ResetBits(GPIOE, GPIO_Pin_2);
    }
    // @OUT:9:1 控制PE9
    else if (strncmp(cmd, "@OUT:", 5) == 0)
    {
        int pin = cmd[5] - '0';
        int val = (cmd[7] == '1');
        if (pin == 9)
            val ? GPIO_SetBits(GPIOE, GPIO_Pin_9)
                : GPIO_ResetBits(GPIOE, GPIO_Pin_9);
    }
}

// UART4接收中断
void UART4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART4_IRQHandler(void)
{
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(UART4) & 0xFF;
        if (data == '@') rx_idx = 0;
        if (rx_idx < sizeof(g_zigbee_rx_buf) - 1) {
            g_zigbee_rx_buf[rx_idx++] = data;
            if (data == '\n') {
                g_zigbee_rx_buf[rx_idx] = '\0';
                g_zigbee_rx_ready = 1;
                rx_idx = 0;
            }
        } else { rx_idx = 0; }
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
    }
}

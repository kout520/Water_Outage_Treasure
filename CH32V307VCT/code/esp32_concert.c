#include"headfile.h"
#include <stdio.h>
#include <stdbool.h>

// ����ȫ��ʱ�����
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
} TimeType;

TimeType currentTime = {2026, 4, 11, 16, 45, 20};  // ������ʼ��Ϊ���յ���ʱ��

// ������ջ�����
#define MAX_RX_BUFFER_SIZE 64

typedef struct {
    uint8_t buffer[MAX_RX_BUFFER_SIZE];
    uint16_t index;
    uint8_t frameComplete;
} UART_RxBuffer_t;

UART_RxBuffer_t uart_rx = {0};
uint16_t receivedData1 =0;
uint8_t adc = 0;
uint8_t time_flag;

void UART_ESP32_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    /* USART3 TX-->B.10  RX-->B.11 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //����PB10Ϊ�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //����PB11Ϊ��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    USART_InitTypeDef  USART_InitStructure={0};
  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART3,ENABLE);

        // ������� NVIC ����
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void uart_send_esp_char(char che)
{
    uint8_t che2 = (uint8_t)che;
    
    // ���͵����ַ�
    USART_SendData(USART3, che2);
    
    // �ȴ��������
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    
    return;
}

void uart_send_esp_string(char* stre)
{
    //��ǰ�ַ�����ַ���ڽ�β ���� �ַ����׵�ַ��Ϊ��
    while(*stre!=0&&stre!=0)
    {
        //�����ַ����׵�ַ�е��ַ��������ڷ������֮���׵�ַ����
        uart_send_esp_char(*stre++);
    }
	return;
}

void send_wifi_to_esp32(const char* ssid, const char* pwd)
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "@WIFI:%s,%s\r\n", ssid, pwd);
    uart_send_esp_string(buffer);
    printf("���͵�ESP32: %s", buffer);
}
void send_serve_to_esp32(const char* serve)
{
    char buffer1[128];
    snprintf(buffer1, sizeof(buffer1), "@SERVE:%s\r\n", serve);
    uart_send_esp_string(buffer1);
    printf("Send to ESP32: %s", buffer1);
}

// 发送水质+流量数据到ESP32
void send_water_quality_to_esp32(void)
{
    char buf[200];
    snprintf(buf, sizeof(buf),
        "@WQ:%d,%d,%.2f,%.4f,%.1f,%d,%.2f,%.2f,%.2f\r\n",
        g_water_quality.tds,
        g_water_quality.conductivity,
        (float)g_water_quality.salinity_x100 / 100.0f,
        (float)g_water_quality.sg_x10000 / 10000.0f,
        (float)g_water_quality.temp_x10 / 10.0f,
        g_water_quality.hardness,
        flow_rate,
        total_flow,
        WQ_GetMonthlyCost()
    );
    uart_send_esp_string(buf);
}

// 发送漏水警报状态到ESP32 (1=漏水, 0=正常)
void send_leak_to_esp32(uint8_t alarm)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "@LEAK:%d\r\n", alarm);
    uart_send_esp_string(buf);
}


void esp_received(void)
{
    // ֡�����Ŵ���������ֱ�ӷ���
    if (!uart_rx.frameComplete) return;

    uart_process_frame((char*)uart_rx.buffer);

    uart_rx.index = 0;
    uart_rx.frameComplete = 0;
}


void uart_process_frame(char* frame)
{
    char buf1[24];
    int len = strlen(frame);
    if (len < 3) return;

    // 阀门控制: @VALVE:0=断水, @VALVE:1=供水
    if (strncmp(frame, "@VALVE:", 7) == 0)
    {
        if (frame[7] == '0') GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
        else GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
        return;
    }

    // WiFi状态
    if (strncmp(frame, "@1\r\n", 4) == 0)
    {
        //wifi����ʧ��
        printf("wifi����ʧ��");
        sprintf(buf1, "va0.val=2");
        tjc_send_string(buf1);
        return;
    }

    if (strncmp(frame, "@2\r\n", 4) == 0)
    {
        //wifi���ӳɹ�
        sprintf(buf1, "va0.val=1");
        tjc_send_string(buf1);
        printf("wifi���ӳɹ�");
        return;
    }
    char time_str[MAX_RX_BUFFER_SIZE];
    strncpy(time_str, frame + 1, len - 2);  // ȥ�� '@' �� '\n'
    time_str[len - 2] = '\0';

    //printf("���յ�ʱ���ַ���: %s\n", time_str);

    int year, month, day, hour, min, sec;

    // �� sscanf ����ʱ���ַ�����ע��&���ָ���
    int ret = sscanf(time_str, "%d-%d-%d&%d:%d:%d",
                     &year, &month, &day, &hour, &min, &sec);

    if (ret == 6) {

        // ���µ�ǰʱ��
        currentTime.year = year;
        currentTime.month = month;
        currentTime.day = day;
        currentTime.hour = hour;
        currentTime.min = min;
        currentTime.sec = sec;
        //char str11[64];
        // ��ʽ���������ַ�������
        // sprintf(str11, "t9.txt=\"%04d-%02d-%02d  %02d:%02d:%02d\"", year, month, day,hour, min, sec);
        // tjc_send_string(str11);  // ��ȷ�����Ѿ�ʵ������������������ַ���
        // ��ʽ��ʱ�����ַ�������
        // sprintf(str11, "t10.txt=\"%02d:%02d:%02d\"", hour, min, sec);
        // tjc_send_string(str11);
    } else {
        printf("ʱ���ַ�����ʽ����ʧ��\n");
    }
}

//�����жϷ������
__attribute__((interrupt("WCH-Interrupt-fast")))
void USART3_IRQHandler(void)
{
   flag = 0;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART3) & 0xFF;
        
        if (uart_rx.index == 0)
        {
            // �ȴ�֡ͷ '@'
            if (data == '@')
            {
                uart_rx.buffer[0] = data;
                uart_rx.index = 1;
                uart_rx.frameComplete = 0;
            }
        }
        else
        {
            if (uart_rx.index < MAX_RX_BUFFER_SIZE - 1)
            {
                uart_rx.buffer[uart_rx.index++] = data;

                if (data == '\n')
                {
                    uart_rx.buffer[uart_rx.index] = '\0';
                    uart_rx.frameComplete = 1;  // ֪ͨ��ѭ������
                }
            }
            else
            {
                // ���������������
                uart_rx.index = 0;
                uart_rx.frameComplete = 0;
            }
        }

        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}



// �ж����꺯��
bool IsLeapYear(int year)
{
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

// ��ȡĳ������
int GetMonthDays(int year, int month)
{
    static const int days_in_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && IsLeapYear(year)) return 29;
    return days_in_month[month - 1];
}

// 1��ʱ���������
void TimeTick_1s(void)
{
    currentTime.sec++;
    if (currentTime.sec >= 60) {
        currentTime.sec = 0;
        currentTime.min++;
        if (currentTime.min >= 60) {
            currentTime.min = 0;
            currentTime.hour++;
            if(currentTime.hour >= 24){
                currentTime.hour = 0;
                currentTime.day++;
                int mdays = GetMonthDays(currentTime.year, currentTime.month);
                if(currentTime.day > mdays){
                    currentTime.day = 1;
                    currentTime.month++;
                    if(currentTime.month > 12){
                        currentTime.month = 1;
                        currentTime.year++;
                    }
                }
            }
        }
    }
}

//��ʾʱ��
void lcd_time(void)
{
    if(time_flag)
    {
    currentTime.sec++;
    if (currentTime.sec >= 60) {
        currentTime.sec = 0;
        currentTime.min++;
        if (currentTime.min >= 60) {
            currentTime.min = 0;
            currentTime.hour++;
            if(currentTime.hour >= 24){
                currentTime.hour = 0;
                currentTime.day++;
                int mdays = GetMonthDays(currentTime.year, currentTime.month);
                if(currentTime.day > mdays){
                    currentTime.day = 1;
                    currentTime.month++;
                    if(currentTime.month > 12){
                        currentTime.month = 1;
                        currentTime.year++;
                    }
                }
            }
        }
      }
    // ˢ����ʾ
    char buf[64];
    sprintf(buf, "t9.txt=\"%04d-%02d-%02d  %02d:%02d:%02d\"", currentTime.year, currentTime.month, currentTime.day, currentTime.hour, currentTime.min, currentTime.sec);
    tjc_send_string(buf);
      time_flag =0;
    }


}






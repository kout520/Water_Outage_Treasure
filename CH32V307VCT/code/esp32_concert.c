<<<<<<< HEAD
#include "headfile.h"
#include <stdio.h>
#include <stdbool.h>

// ============================================================
// ESP32ÈÄö‰ø° (USART3: PB10=TX, PB11=RX, 115200bps)
// ÂçèËÆÆ: @ÂëΩ‰ª§ Ê†ºÂºè
// ============================================================

// ÂÖ®Â±ÄÊó∂Èó¥Êï∞ÊçÆ
=======
#include"headfile.h"
#include <stdio.h>
#include <stdbool.h>

// ∂®“Â»´æ÷ ±º‰±‰¡ø
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
} TimeType;

<<<<<<< HEAD
TimeType currentTime = {2026, 4, 11, 16, 45, 20};

// UARTÊé•Êî∂ÁºìÂÜ≤Âå∫
=======
TimeType currentTime = {2026, 4, 11, 16, 45, 20};  // ¿˝£∫≥ı ºªØŒ™Ω” ’µΩµƒ ±º‰

// ∂®“ÂΩ” ’ª∫≥Â«¯
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
#define MAX_RX_BUFFER_SIZE 64

typedef struct {
    uint8_t buffer[MAX_RX_BUFFER_SIZE];
    uint16_t index;
    uint8_t frameComplete;
} UART_RxBuffer_t;

UART_RxBuffer_t uart_rx = {0};
<<<<<<< HEAD
uint16_t receivedData1 = 0;
uint8_t adc = 0;
uint8_t time_flag;

// ============================================================
// USART3ÂàùÂßãÂåñ
// ============================================================
void UART_ESP32_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /* USART3 TX-->PB.10  RX-->PB.11 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitTypeDef  USART_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
=======
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //…Ë÷√PB10Œ™∏¥”√Õ∆ÕÏ ‰≥ˆ
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //…Ë÷√PB11Œ™∏°ø’ ‰»Î
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    USART_InitTypeDef  USART_InitStructure={0};
  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART3, &USART_InitStructure);

<<<<<<< HEAD
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);

    // NVICÈÖçÁΩÆ
=======
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART3,ENABLE);

        // ≤π…œ’‚∂Œ NVIC ≈‰÷√
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

<<<<<<< HEAD
// ============================================================
// USART3ÂèëÈÄÅ
// ============================================================
void uart_send_esp_char(char che)
{
    uint8_t che2 = (uint8_t)che;
    USART_SendData(USART3, che2);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
=======

void uart_send_esp_char(char che)
{
    uint8_t che2 = (uint8_t)che;
    
    // ∑¢ÀÕµ•∏ˆ◊÷∑˚
    USART_SendData(USART3, che2);
    
    // µ»¥˝∑¢ÀÕÕÍ≥…
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    
    return;
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
}

void uart_send_esp_string(char* stre)
{
<<<<<<< HEAD
    while (*stre != 0 && stre != 0)
    {
        uart_send_esp_char(*stre++);
    }
}

// ============================================================
// ÂèëÈÄÅÈÖçÁΩÆÂà∞ESP32
// ============================================================
=======
    //µ±«∞◊÷∑˚¥Æµÿ÷∑≤ª‘⁄Ω·Œ≤ ≤¢«“ ◊÷∑˚¥Æ ◊µÿ÷∑≤ªŒ™ø’
    while(*stre!=0&&stre!=0)
    {
        //∑¢ÀÕ◊÷∑˚¥Æ ◊µÿ÷∑÷–µƒ◊÷∑˚£¨≤¢«“‘⁄∑¢ÀÕÕÍ≥…÷Æ∫Û ◊µÿ÷∑◊‘‘ˆ
        uart_send_esp_char(*stre++);
    }
	return;
}

>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
void send_wifi_to_esp32(const char* ssid, const char* pwd)
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "@WIFI:%s,%s\r\n", ssid, pwd);
    uart_send_esp_string(buffer);
<<<<<<< HEAD
}

=======
    printf("∑¢ÀÕµΩESP32: %s", buffer);
}
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
void send_serve_to_esp32(const char* serve)
{
    char buffer1[128];
    snprintf(buffer1, sizeof(buffer1), "@SERVE:%s\r\n", serve);
    uart_send_esp_string(buffer1);
<<<<<<< HEAD
}

// ============================================================
// ÂèëÈÄÅÊ∞¥Ë¥®+ÊµÅÈáèÊï∞ÊçÆÂà∞ESP32
// Ê†ºÂºè: @WQ:tds,cond,sal,sg,temp,hard,flow_rate,total_flow,cost\r\n
// ============================================================
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

// ============================================================
// ÂèëÈÄÅÊºèÊ∞¥Ë≠¶Êä•Áä∂ÊÄÅÂà∞ESP32 (1=ÊºèÊ∞¥, 0=Ê≠£Â∏∏)
// ============================================================
void send_leak_to_esp32(uint8_t alarm)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "@LEAK:%d\r\n", alarm);
    uart_send_esp_string(buf);
}

// ============================================================
// ESP32Êï∞ÊçÆÊé•Êî∂Â§ÑÁêÜ
// ============================================================
void esp_received(void)
{
=======
    printf("∑¢ÀÕµΩESP32: %s", buffer1);
}


void esp_received(void)
{
    // ÷°ÕÍ’˚≤≈¥¶¿Ì£¨∑Ò‘Ú÷±Ω”∑µªÿ
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    if (!uart_rx.frameComplete) return;

    uart_process_frame((char*)uart_rx.buffer);

    uart_rx.index = 0;
    uart_rx.frameComplete = 0;
}

<<<<<<< HEAD
=======

>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
void uart_process_frame(char* frame)
{
    char buf1[24];
    int len = strlen(frame);
<<<<<<< HEAD
    if (len < 3) return;

    // ÈòÄÈó®ÊéßÂà∂: @VALVE:0=Êñ≠Ê∞¥(PE9È´ò), @VALVE:1=‰æõÊ∞¥(PE9‰Ωé)
    if (strncmp(frame, "@VALVE:", 7) == 0)
    {
        if (frame[7] == '0') {
            //GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_SET);    // PE9È´òÁîµÂπ≥, Êñ≠Èó∏
            GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);    // PA4È´òÁîµÂπ≥, Êñ≠Èó∏
        } else if (frame[7] == '1') {
            //GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_RESET);  // PE9‰ΩéÁîµÂπ≥, ‰æõÊ∞¥
            GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);  // PA4‰ΩéÁîµÂπ≥, ‰æõÊ∞¥
        }
        return;
    }

    // WiFiËøûÊé•Â§±Ë¥•
    if (strncmp(frame, "@1\r\n", 4) == 0)
    {
=======
    if (len < 3 ) {

        return;
    }
    // ≈–∂œÃÿ ‚÷∏¡Ó÷°
    if (strncmp(frame, "@1\r\n", 4) == 0)
    {
        //wifi¡¨Ω” ß∞‹
        printf("wifi¡¨Ω” ß∞‹");
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
        sprintf(buf1, "va0.val=2");
        tjc_send_string(buf1);
        return;
    }

<<<<<<< HEAD
    // WiFiËøûÊé•ÊàêÂäü
    if (strncmp(frame, "@2\r\n", 4) == 0)
    {
        sprintf(buf1, "va0.val=1");
        tjc_send_string(buf1);
        return;
    }

    // Êó∂Èó¥ÂêåÊ≠•Â∏ß: @2026-04-11&16:45:20
    char time_str[MAX_RX_BUFFER_SIZE];
    strncpy(time_str, frame + 1, len - 2);
    time_str[len - 2] = '\0';

    int year, month, day, hour, min, sec;
=======
    if (strncmp(frame, "@2\r\n", 4) == 0)
    {
        //wifi¡¨Ω”≥…π¶
        sprintf(buf1, "va0.val=1");
        tjc_send_string(buf1);
        printf("wifi¡¨Ω”≥…π¶");
        return;
    }
    char time_str[MAX_RX_BUFFER_SIZE];
    strncpy(time_str, frame + 1, len - 2);  // »•µÙ '@' ∫Õ '\n'
    time_str[len - 2] = '\0';

    //printf("Ω” ’µΩ ±º‰◊÷∑˚¥Æ: %s\n", time_str);

    int year, month, day, hour, min, sec;

    // ”√ sscanf Ω‚Œˆ ±º‰◊÷∑˚¥Æ£¨◊¢“‚&◊ˆ∑÷∏Ù∑˚
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    int ret = sscanf(time_str, "%d-%d-%d&%d:%d:%d",
                     &year, &month, &day, &hour, &min, &sec);

    if (ret == 6) {
<<<<<<< HEAD
        currentTime.year  = year;
        currentTime.month = month;
        currentTime.day   = day;
        currentTime.hour  = hour;
        currentTime.min   = min;
        currentTime.sec   = sec;
    }
}

// ============================================================
// USART3Êé•Êî∂‰∏≠Êñ≠
// ============================================================
void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART3) & 0xFF;

        if (uart_rx.index == 0)
        {
            // Á≠âÂæÖÂ∏ßÂ§¥ '@'
=======

        // ∏¸–¬µ±«∞ ±º‰
        currentTime.year = year;
        currentTime.month = month;
        currentTime.day = day;
        currentTime.hour = hour;
        currentTime.min = min;
        currentTime.sec = sec;
        //char str11[64];
        // ∏Ò ΩªØƒÍ‘¬»’◊÷∑˚¥Æ∑¢ÀÕ
        // sprintf(str11, "t9.txt=\"%04d-%02d-%02d  %02d:%02d:%02d\"", year, month, day,hour, min, sec);
        // tjc_send_string(str11);  // «Î»∑»œƒ„“—æ≠ µœ÷’‚∏ˆ∫Ø ˝£¨∏∫‘∑¢ÀÕ◊÷∑˚¥Æ
        // ∏Ò ΩªØ ±∑÷√Î◊÷∑˚¥Æ∑¢ÀÕ
        // sprintf(str11, "t10.txt=\"%02d:%02d:%02d\"", hour, min, sec);
        // tjc_send_string(str11);
    } else {
        printf(" ±º‰◊÷∑˚¥Æ∏Ò ΩΩ‚Œˆ ß∞‹\n");
    }
}

//¥Æø⁄÷–∂œ∑˛ŒÒ≥Ã–Ú
__attribute__((interrupt("WCH-Interrupt-fast")))
void USART3_IRQHandler(void)
{
   flag = 0;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART3) & 0xFF;
        
        if (uart_rx.index == 0)
        {
            // µ»¥˝÷°Õ∑ '@'
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
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
<<<<<<< HEAD
                    uart_rx.frameComplete = 1;
=======
                    uart_rx.frameComplete = 1;  // Õ®÷™÷˜—≠ª∑¥¶¿Ì
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
                }
            }
            else
            {
<<<<<<< HEAD
                // ÁºìÂÜ≤Âå∫Ê∫¢Âá∫, ÈáçÁΩÆ
=======
                // ª∫≥Â«¯“Á≥ˆ£¨÷ÿ÷√
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
                uart_rx.index = 0;
                uart_rx.frameComplete = 0;
            }
        }

        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}

<<<<<<< HEAD
// ============================================================
// Êó∂Èó¥Â§ÑÁêÜ
// ============================================================
=======


// ≈–∂œ»ÚƒÍ∫Ø ˝
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
bool IsLeapYear(int year)
{
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

<<<<<<< HEAD
=======
// ªÒ»°ƒ≥‘¬ÃÏ ˝
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
int GetMonthDays(int year, int month)
{
    static const int days_in_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && IsLeapYear(year)) return 29;
    return days_in_month[month - 1];
}

<<<<<<< HEAD
=======
// 1√Î ±º‰µ›‘ˆ∫Ø ˝
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
void TimeTick_1s(void)
{
    currentTime.sec++;
    if (currentTime.sec >= 60) {
        currentTime.sec = 0;
        currentTime.min++;
        if (currentTime.min >= 60) {
            currentTime.min = 0;
            currentTime.hour++;
<<<<<<< HEAD
            if (currentTime.hour >= 24) {
                currentTime.hour = 0;
                currentTime.day++;
                int mdays = GetMonthDays(currentTime.year, currentTime.month);
                if (currentTime.day > mdays) {
                    currentTime.day = 1;
                    currentTime.month++;
                    if (currentTime.month > 12) {
=======
            if(currentTime.hour >= 24){
                currentTime.hour = 0;
                currentTime.day++;
                int mdays = GetMonthDays(currentTime.year, currentTime.month);
                if(currentTime.day > mdays){
                    currentTime.day = 1;
                    currentTime.month++;
                    if(currentTime.month > 12){
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
                        currentTime.month = 1;
                        currentTime.year++;
                    }
                }
            }
        }
    }
}

<<<<<<< HEAD
void lcd_time(void)
{
    if (time_flag)
    {
        currentTime.sec++;
        if (currentTime.sec >= 60) {
            currentTime.sec = 0;
            currentTime.min++;
            if (currentTime.min >= 60) {
                currentTime.min = 0;
                currentTime.hour++;
                if (currentTime.hour >= 24) {
                    currentTime.hour = 0;
                    currentTime.day++;
                    int mdays = GetMonthDays(currentTime.year, currentTime.month);
                    if (currentTime.day > mdays) {
                        currentTime.day = 1;
                        currentTime.month++;
                        if (currentTime.month > 12) {
                            currentTime.month = 1;
                            currentTime.year++;
                        }
=======
//œ‘ æ ±º‰
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
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
                    }
                }
            }
        }
<<<<<<< HEAD
        // Âà∑Êñ∞HMIÊó∂Èó¥ÊòæÁ§∫
        char buf[64];
        sprintf(buf, "t9.txt=\"%04d-%02d-%02d  %02d:%02d:%02d\"",
                currentTime.year, currentTime.month, currentTime.day,
                currentTime.hour, currentTime.min, currentTime.sec);
        tjc_send_string(buf);
        time_flag = 0;
    }
}
=======
      }
    // À¢–¬œ‘ æ
    char buf[64];
    sprintf(buf, "t9.txt=\"%04d-%02d-%02d  %02d:%02d:%02d\"", currentTime.year, currentTime.month, currentTime.day, currentTime.hour, currentTime.min, currentTime.sec);
    tjc_send_string(buf);
      time_flag =0;
    }


}





>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb

#ifndef _esp32_concert_
#define _esp32_concert_

<<<<<<< HEAD
// ESP32通信 (USART3: PB10=TX, PB11=RX, 115200bps)

=======
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
void UART_ESP32_Init(void);
void uart_send_esp_char(char ch);
void uart_send_esp_string(char* str);

void esp_received(void);
void uart_process_frame(char* frame);
void TimeTick_1s(void);
void lcd_time(void);
void send_wifi_to_esp32(const char* ssid, const char* pwd);
void send_serve_to_esp32(const char* serve);
<<<<<<< HEAD
void send_water_quality_to_esp32(void);
void send_leak_to_esp32(uint8_t alarm);

extern uint8_t time_flag;

#endif
=======
extern uint8_t time_flag;

#endif

>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb

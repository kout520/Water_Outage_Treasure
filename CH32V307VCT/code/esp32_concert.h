#ifndef _esp32_concert_
#define _esp32_concert_

void UART_ESP32_Init(void);
void uart_send_esp_char(char ch);
void uart_send_esp_string(char* str);

void esp_received(void);
void uart_process_frame(char* frame);
void TimeTick_1s(void);
void lcd_time(void);
void send_wifi_to_esp32(const char* ssid, const char* pwd);
void send_serve_to_esp32(const char* serve);
extern uint8_t time_flag;

#endif


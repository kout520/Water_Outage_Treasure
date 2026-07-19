
#ifndef _zigbee_
#define _zigbee_

// ============================================================
// Zigbee模块 (UART4: PC10=TX, PC11=RX)
// ============================================================

void Zigbee_Init(uint32_t baud_rate);
void Zigbee_SendString(char* str);
void Zigbee_SendSensorData(void);
void Zigbee_ParseCommand(char* cmd);

extern char g_zigbee_rx_buf[128];
extern uint8_t g_zigbee_rx_ready;

#endif

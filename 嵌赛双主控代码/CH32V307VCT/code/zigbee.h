#ifndef _zigbee_
#define _zigbee_

// ============================================================
// Zigbee模块 (UART4: PC10=TX, PC11=RX)
// 与塔楼CH32通信
// ============================================================

void Zigbee_Init(uint32_t baud_rate);
void Zigbee_SendString(char* str);
void Zigbee_Task(void);                // 主循环调用
void Zigbee_ParseCommand(char* cmd);   // 解析塔楼发来的数据

// 发送指令到塔楼
void Zigbee_SendPumpSpeed(uint8_t speed);
void Zigbee_SendValve(uint8_t open);

// 塔楼数据
extern float g_tower_water_level;      // 水位(m)
extern float g_tower_humidity;         // 湿度(%)
extern float g_tower_temperature;      // 温度(℃)
extern int   g_tower_pump_speed;       // 水泵转速
extern uint8_t g_tower_data_updated;   // 数据更新标志

#endif



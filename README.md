# Water_Outage_Treasure · 水宝
> 基于**双 CH32V307 RISC-V 主控**的智能水监控系统 —— 集水质监测、用水计量、AI 漏水防护、塔楼水箱联动与远程告警于一体，面向嵌入式设计大赛打造的全栈智能用水安全装置！
[![MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)
[![CH32V307](https://img.shields.io/badge/MCU-CH32V307%20RISC--V-blue.svg)](http://www.wch.cn/products/CH32V307.html)
[![MounRiver](https://img.shields.io/badge/IDE-MounRiver%20Studio-orange.svg)](http://www.mounriver.com/)
[![Embedded Competition](https://img.shields.io/badge/Embedded%20Competition-Ready-red.svg)]()

---
## 📖 项目简介
本项目（**水宝**）是面向嵌入式设计大赛沁恒组打造的**多传感智能用水监控装置**，采用**双 CH32V307 RISC-V 主控**架构，构建了集**水质监测、流量计量、AI 漏水防护、塔楼水箱控制与远程告警**于一体的闭环防护系统。

系统实时采集**水温、pH、TDS、浊度、盐浓度、流量**等多路水质数据，通过 **4.3 寸串口触控屏**本地可视化展示；同时经 **ESP32 网络模块**上传云端，支持 Web 端远程查看、历史用水记录与水质曲线分析。内置**轻量化 AI 异常识别算法**，自动区分正常用水、微量渗漏、爆管大流量泄漏等场景，异常时**自动切断电磁阀**并同步向手机**微信推送告警**。终端主控与塔楼主控通过 **ZigBee 无线通信**联动，实现分布式水箱水位、温湿度监测与水泵功率控制。整机采用**全密封防水结构**，适用于家庭、出租屋、浴池、小型楼宇等潮湿用水环境。✨

---
## ✨ 核心特性
- 🛰️ **双 CH32V307 分布式主控**：终端主控（水质/流量/漏水/人机交互）+ 塔楼主控（水位/温湿度/水泵），ZigBee 无线组网联动
- 🧪 **多传感一体化采集**：水温、pH、TDS、浊度、盐浓度、流量六路水质数据，片内 ADC 高精度采样 + RC 硬件滤波
- 🤖 **轻量化 AI 异常识别**：阈值判断 + 流量趋势分析，区分正常用水 / 微量渗水 / 爆管大流量，分级执行告警与断水
- 🚰 **主动断水防护**：检测异常 ≤3s 内自动驱动电磁阀切断水路，杜绝漏水损失
- 📱 **微信远程告警**：ESP32 + Server酱 向手机微信推送漏水、系统状态等图文告警
- ☁️ **云端 Web 平台**：MQTT 上云，Web 端实时查看、历史用水查询、水质曲线分析、用水统计报表
- 🏢 **塔楼水箱联动**：HC-SR04 超声波水位 + AHT20 温湿度监测，编码器联动水泵自动调速
- 🖥️ **本地离线可用**：4.3 寸串口触控屏独立运行，断网仍具备完整监测与本地断水防护

---
## 🧩 已集成模块清单
### ⚙️ 终端主控（CH32V307VCT）—— 水质 / 流量 / 漏水 / 人机交互
| 模块名称 | 功能说明 | 接口 |
| :--- | :--- | :--- |
| water_quality | 水质探头协议解析（TDS/盐度/电导率/比重/温度/硬度） | UART |
| water_speed | 流量计量（瞬时/累计流量、输入捕获）与漏水检测 AI 判别 | TIM/捕获 |
| tjc_usart_hmi | 4.3 寸 TJC 串口触控屏驱动（数据刷新/参数设置） | UART1 |
| esp32_concert | 与 ESP32 网络模块协同（数据上传/微信告警下发） | UART3 |
| zigbee | ZigBee 无线组网，与塔楼主控数据互传 | UART4 |
| adc_going / pwm / key / lcd | ADC 采样、PWM、按键、指示 | ADC/PWM/GPIO |

### 🏢 塔楼主控（CH32V307VCT_1）—— 水位 / 温湿度 / 水泵
| 模块名称 | 功能说明 | 接口 |
| :--- | :--- | :--- |
| hc_sr04 | 超声波测距，塔楼水箱水位检测（`水位 = 水深 - 距离`） | GPIO |
| aht20 / humidity / temperature | AHT20 温湿度采集 | I2C |
| encoder | 编码器测速，联动水泵调速 | 编码器 |
| pump_ctrl | TB6612 水泵驱动（PWM 调速 0~100%、方向、待机控制） | PWM/GPIO |
| JLNU_OLED / MYI2C | OLED 本地显示（温度/湿度/水位/水泵状态） | I2C |
| zigbee | ZigBee 无线组网，上报传感器数据 / 接收指令 | UART4 |

### 🌐 网络与云端（ESP32 网络部分 + 网站）
| 模块名称 | 功能说明 |
| :--- | :--- |
| ESP32 网络模块 | MicroPython 实现，WiFi 联网、MQTT 上云、NTP 时间同步 |
| 微信告警 | Server酱（sctapi）向手机微信推送漏水/系统状态告警 |
| 水宝 Web 平台 | `网站/水宝.html` 水质实时监测前端页面 |
| lwIP 参考工程 | `CH32V307_lwIP-1.41_FreeRTOS-main` lwIP + FreeRTOS 协议栈参考 |

---
## 🚀 快速上手
### 1. 环境要求
- **主控芯片**：沁恒 CH32V307（RISC-V 内核）
- **开发工具**：MounRiver Studio（MRS，沁恒官方 IDE），工程位于 `CH32V307VCT/`、`CH32V307VCT_1/`
- **网络模块**：ESP32（MicroPython 固件，代码见 `ESP32网络部分/`）
- **联调工具**：4.3 寸 TJC 串口触控屏、ZigBee 模块、CMSIS-DAP/WCH-Link 下载器

### 2. 使用步骤（详细版）

#### 第一步：获取代码
```bash
git clone https://github.com/kout520/Water_Outage_Treasure.git
cd Water_Outage_Treasure
```

#### 第二步：烧录双 CH32V307 主控
**环境**：MounRiver Studio（MRS）+ WCH-Link / CMSIS-DAP 下载器，两块芯片型号均为 **CH32V307VCT6**。

1. 用 MRS 打开终端主控工程 `CH32V307VCT/`，点击 **Build / F7** 编译
2. 连接下载器，确认目标芯片为 CH32V307VCT6，点击 **Flash Download** 烧录
3. 用同样方式打开、编译并烧录塔楼主控工程 `CH32V307VCT_1/`
4. 若下载报错，检查 SWD 接线、供电电压与芯片型号选择

#### 第三步：配置 ESP32 网络模块（MicroPython）
1. 用 esptool 给 ESP32 刷入 MicroPython 固件：
   ```bash
   esptool.py --port COMx erase_flash
   esptool.py --port COMx --baud 460800 write_flash -z 0x1000 micropython.bin
   ```
2. 用 Thonny / ampy 将 `ESP32网络部分/` 下的 `main.py`、`boot.py`、`wifi.py`、`umqttsimple.py`、`urequests.py` 全部上传到 ESP32 根目录
3. 修改 `wifi_config.json`，填入你自己的 WiFi 热点：
   ```json
   {"ssid": "你的WiFi名", "password": "你的WiFi密码"}
   ```
4. 微信告警配置（可选）：将 Server酱 的 SendKey 填入 `sendkey.txt`（或使用 `main.py` 中 `SENDKEY` 默认值）；如需更换云端服务器，修改 `main.py` 中的 MQTT 服务器 IP（默认 `159.75.161.237`，数据上报主题 `aa`，订阅主题 `ledctl`）
5. 将 ESP32 的 UART2（RX=16 / TX=17，115200bps）与终端主控连接

#### 第四步：硬件接线（可参考 `嵌赛系统.dwg` 与设计报告）
**终端主控 CH32V307VCT：**
| 外设 | 连接接口 |
| :--- | :--- |
| 4.3 寸 TJC 串口触控屏 | USART1 |
| ESP32 网络模块 | USART3（PB10=TX，PB11=RX，115200） |
| ZigBee 模块 | UART4（PC10=TX，PC11=RX，115200） |
| 水质传感器 | UART（`WQ_Init(9600)`） |
| 流量传感器 | 输入捕获引脚（`Input_Capture_Init(0xFFFF, 47999)`） |
| 电磁阀（断水） | 继电器输出 |

**塔楼主控 CH32V307VCT_1：**
| 外设 | 连接接口 |
| :--- | :--- |
| ZigBee 模块 | UART4（PC10=TX，PC11=RX，115200） |
| HC-SR04 超声波（水位） | GPIO |
| AHT20 温湿度 | I2C |
| TB6612 水泵驱动 | PB0（PWMA/TIM3_CH3）、PC1（AIN1）、PE9（AIN2）、PE2（STBY） |
| 编码器 | 测速引脚 |
| OLED 显示屏 | I2C |

#### 第五步：上电联调
1. 先给**塔楼主控**上电 → OLED 显示 `Tower Monitor`，HC-SR04 每 2s 测一次水位，ZigBee 每 2s 上报数据
2. 再给**终端主控**上电 → 4.3 寸串口屏点亮，实时显示水质 / 流量 / 水位等数据
3. 等待双主控 **ZigBee 组网**（两端 UART4 收发交叉对连、波特率 115200），塔楼的水位 / 温湿度 / 水泵状态应同步显示在终端串口屏上
4. 转动编码器 → 塔楼水泵按编码器速度自动调速（`Encoder_GetSpeed → Pump_SetSpeed`）
5. 人为制造漏水 / 流量突变 → 终端主控 **≤3s 识别异常并驱动电磁阀断水**，ESP32 同步向手机 **微信推送告警**
6. 用浏览器打开 `网站/水宝.html`，验证 Web 端实时数据展示

#### 第六步：参数与阈值调整
| 调整项 | 修改位置 |
| :--- | :--- |
| 漏水判定阈值、流量 / 水质参数 | `CH32V307VCT/code/water_speed.c`、`water_quality.c` |
| 塔楼水箱水深基准 | `CH32V307VCT_1/User/main.c` 中 `TANK_DEPTH`（默认 27.0cm） |
| 水泵调速上限 | `CH32V307VCT_1/code/pump_ctrl.c` |

#### 常见问题
- **串口屏无显示**：检查 USART1 接线与波特率是否匹配
- **微信收不到告警**：确认 Server酱 SendKey 有效、ESP32 已联网成功（观察串口打印 `✅ WiFi重连成功`）
- **双主控数据不通**：核对两端 ZigBee 波特率（115200）与 TX/RX 收发交叉接线

### 3. 双主控初始化流程（main.c 示例）
```c
/* ===== 终端主控 CH32V307VCT ===== */
int main(void)
{
    UART_HIM_Init();        /* 4.3寸串口屏 */
    UART_ESP32_Init();      /* ESP32 联网 */
    Zigbee_Init(115200);    /* ZigBee 组网 */
    WQ_Init(9600);          /* 水质传感器 */
    Input_Capture_Init(0xFFFF, 48000 - 1); /* 流量输入捕获 */
    Leak_Reset();           /* 漏水检测 */
    while (1) {
        HMI_task();  esp_received();
        Leak_Task(); WQ_Task();   /* AI 漏水判别 + 水质采集 */
        Delay_Ms(1);
    }
}

/* ===== 塔楼主控 CH32V307VCT_1 ===== */
int main(void)
{
    HC_SR04_Init();         /* 超声波测水位 */
    AHT20_Init();           /* 温湿度 */
    Pump_Init();            /* 水泵驱动 */
    Encoder_Init();         /* 编码器测速 */
    Zigbee_Init(115200);
    while (1) {
        HC_SR04_GetDistance_cm();   /* 水位 = 水深 - 距离 */
        AHT20_Task();
        Pump_SetSpeed(Encoder_GetSpeed()); /* 编码器联动水泵 */
        Zigbee_SendSensorData();    /* 上报塔楼数据 */
        Delay_Ms(10);
    }
}
```

---
## 📂 项目结构
```
Water_Outage_Treasure/
├── CH32V307VCT/                    # 终端主控工程（水质/流量/漏水/串口屏/联网）
│   ├── code/                       # 业务代码（water_quality / water_speed / tjc_usart_hmi / esp32_concert / zigbee 等）
│   ├── User/main.c                 # 终端主程序
│   ├── Core/  Peripheral/  Startup/
├── CH32V307VCT_1/                  # 塔楼主控工程（水位/温湿度/水泵/OLED）
│   └── code/                       # hc_sr04 / aht20 / encoder / pump_ctrl / JLNU_OLED 等
├── CH32V307_lwIP-1.41_FreeRTOS-main/  # lwIP + FreeRTOS 网络协议栈参考工程
├── ESP32网络部分/                   # ESP32 网络模块（MicroPython）
│   ├── main.py                     # WiFi/MQTT/微信告警/NTP
│   ├── wifi.py / wifi_config.json  # WiFi 连接与配置
│   └── umqttsimple.py / urequests.py  # MQTT / HTTP 客户端
├── 网站/                           # Web 端展示平台（水宝.html）
├── 嵌赛双主控代码/                   # 双主控代码（旧版备份）
├── 嵌赛系统.dwg                     # 系统结构设计图
├── 嵌赛设计报告.docx                # 设计报告
├── .gitignore                       # Git 忽略文件
├── LICENSE                          # MIT 开源协议
└── README.md                        # 项目说明文档
```

---
## 🎯 主要性能指标
| 检测项目 | 参数指标 |
| :--- | :--- |
| 流量检测范围 | 0.01 – 10 L/min，精度 ±2% |
| 水温检测 | 0 – 60℃，精度 ±0.5℃ |
| TDS 测量范围 | 0 – 1000 mg/L |
| 浊度检测精度 | 0 – 100 NTU |
| 本地显示 | 4.3 寸串口触控屏，1 秒刷新 |
| 云端上传周期 | 10 – 60s 可调 |
| 漏水响应速度 | 异常识别 ≤3s，断水动作 ≤3s |

---
## 📄 开源协议
本项目采用 **MIT License** 开源协议，详细协议请查看 [LICENSE](./LICENSE) 文件。

---
## 🎉 致谢
感谢沁恒（WCH）提供的优秀国产 RISC-V 芯片与开发环境，祝各位在嵌入式设计大赛中取得优异成绩！
---

/**
使用注意事项:
    1.将tjc_usart_hmi.c和tjc_usart_hmi.h 分别导入工程
    2.在需要使用的函数所在的头文件中添加 #include "tjc_usart_hmi.h"


*/
#include "tjc_usart_hmi.h"
#include "headfile.h"


#define FRAME_LENGTH 7

typedef struct
{
    uint16_t Head;
    uint16_t Tail;
    uint16_t Length;
    uint8_t  Ring_data[RINGBUFFER_LEN];
}RingBuffer_t;

RingBuffer_t ringBuffer;	//创建一个ringBuffer的缓冲区
uint8_t RxBuffer[1];


void UART_HIM_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    /* USART2 TX-->A.2   RX-->A.3 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //设置PA2为复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //设置PA3为浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_InitTypeDef  USART_InitStructure={0};
  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &USART_InitStructure);
        NVIC_InitTypeDef   NVIC_InitStructure={0};

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;      //抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;           //子优先级为1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                              //中断优先级初始化
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2,ENABLE);
}

/********************************************************
函数名：  		intToStr
日期：    	2024.09.18
功能：    	将整形转换为字符串
输入参数：		要转换的整形数据,输出的字符串数组
返回值： 		无
修改记录：
**********************************************************/
void intToStr(int num, char* str) {
    int i = 0;
    int isNegative = 0;

    // 处理负数
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // 提取每一位数字
    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num);

    // 如果是负数，添加负号
    if (isNegative) {
        str[i++] = '-';
    }

    // 添加字符串终止符
    str[i] = '\0';

    // 反转字符串
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
    return ;
}


/********************************************************
函数名：  		uart_send_char
日期：    	2024.09.18
功能：    	串口发送单个字符
输入参数：		要发送的单个字符
返回值： 		无
修改记录：
**********************************************************/
void uart_send_char(char ch)
{
    uint8_t ch2 = (uint8_t)ch;
    
    // 发送单个字符
    USART_SendData(USART2, ch2);
    
    // 等待发送完成
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    
    return;
}

// /********************************************************
// 函数名：  		uart_send_char
// 日期：    	2024.09.18
// 功能：    	串口发送单个字符
// 输入参数：		要发送的单个字符
// 返回值： 		无
// 修改记录：
// **********************************************************/
// void uart_send_char(char ch)
// {
// 	uint8_t ch2 = (uint8_t)ch;
//     //当串口0忙的时候等待，不忙的时候再发送传进来的字符
// 	//while(__HAL_UART_GET_FLAG(&TJC_UART, UART_FLAG_TXE) == RESET);	//等待发送完毕
// 	while(__HAL_UART_GET_FLAG(&TJC_UART, UART_FLAG_TC) == RESET);
//     //发送单个字符
// 	HAL_UART_Transmit_IT(&TJC_UART, &ch2, 1);



    
//     // USART_SendData(pUSARTx, data);
    
//     // while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);



// 	return;
// }



void uart_send_string(char* str)
{
    //当前字符串地址不在结尾 并且 字符串首地址不为空
    while(*str!=0&&str!=0)
    {
        //发送字符串首地址中的字符，并且在发送完成之后首地址自增
        uart_send_char(*str++);
    }
	return;
}

/********************************************************
函数名：  		tjc_send_string
日期：    	2024.09.18
功能：    	串口发送字符串和结束符
输入参数：		要发送的字符串
返回值： 		无
示例:			tjc_send_val("n0", "val", 100); 发出的数据就是 n0.val=100
修改记录：
**********************************************************/
void tjc_send_string(char* str)
{
    //当前字符串地址不在结尾 并且 字符串首地址不为空
    while(*str!=0&&str!=0)
    {
        //发送字符串首地址中的字符，并且在发送完成之后首地址自增
        uart_send_char(*str++);
    }
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

/********************************************************
函数名：  		tjc_send_txt
日期：    	2024.09.18
功能：    	串口发送字符串和结束符
输入参数：		要发送的字符串
返回值： 		无
示例:			tjc_send_txt("t0", "txt", "ABC"); 发出的数据就是t0.txt="ABC"
修改记录：
**********************************************************/
void tjc_send_txt(char* objname, char* attribute, char* txt)
{

    uart_send_string(objname);
    uart_send_char('.');
    uart_send_string(attribute);
    uart_send_string("=\"");
    uart_send_string(txt);
    uart_send_char('\"');
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}


/********************************************************
函数名：  		tjc_send_val
日期：    	2024.09.18
功能：    	串口发送字符串和结束符
输入参数：		要发送的字符串
返回值： 		无
修改记录：
**********************************************************/
void tjc_send_val(char* objname, char* attribute, int val)
{
	//拼接字符串,比如n0.val=123
    uart_send_string(objname);
    uart_send_char('.');
    uart_send_string(attribute);
    uart_send_char('=');
    //C语言中整形的取值范围是：“-2147483648 ~ 2147483647”, 最长为-2147483648,加上结束符\0一共12个字符
    char txt[12]="";
    intToStr(val, txt);
    uart_send_string(txt);
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

/********************************************************
函数名：  		tjc_send_nstring
日期：    	2024.09.18
功能：    	串口发送字符串和结束符
输入参数：		要发送的字符串,字符串长度
返回值： 		无
修改记录：
**********************************************************/
void tjc_send_nstring(char* str, unsigned char str_length)
{
    //当前字符串地址不在结尾 并且 字符串首地址不为空
    for (int var = 0; var < str_length; ++var)
    {
        //发送字符串首地址中的字符，并且在发送完成之后首地址自增
        uart_send_char(*str++);
    }
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

u8 USART_Rbuffer_Num = 0;
u8 USART_Tbuffer_Num = 0;
// u8 USART_Rbuffer[buffer_len];//接收缓冲区数组



/********************************************************
函数名：  		initRingBuffer
日期：    	2022.10.08
功能：    	初始化环形缓冲区
输入参数：
返回值： 		void
修改记录：
**********************************************************/
void initRingBuffer(void)
{
	//初始化相关信息
	ringBuffer.Head = 0;
	ringBuffer.Tail = 0;
	ringBuffer.Length = 0;
	return;
}

//接收中断
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void)
{
    
    // 正确读取接收到的 HEX 数据
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        
        uint8_t receivedData = (uint8_t)USART_ReceiveData(USART2);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        
        write1ByteToRingBuffer(receivedData);
        USART_Rbuffer_Num++;
        
    }


}


/********************************************************
函数名：  		write1ByteToRingBuffer
日期：    	2022.10.08
功能：    	往环形缓冲区写入数据
输入参数：		要写入的1字节数据
返回值： 		void
修改记录：
**********************************************************/
void write1ByteToRingBuffer(uint8_t data)
{
	if(ringBuffer.Length >= RINGBUFFER_LEN) //判断缓冲区是否已满
	{
	return ;
	}
	ringBuffer.Ring_data[ringBuffer.Tail]=data;
	ringBuffer.Tail = (ringBuffer.Tail+1)%RINGBUFFER_LEN;//防止越界非法访问
	ringBuffer.Length++;
	return ;
}
/********************************************************
函数名：  		read1ByteFromRingBuffer
作者：
日期：    	2022.10.08
功能：    	从串口缓冲区读取1字节数据
输入参数：		position:读取的位置
返回值： 		所在位置的数据(1字节)
修改记录：
**********************************************************/
uint8_t read1ByteFromRingBuffer(uint16_t position)
{
	uint16_t realPosition = (ringBuffer.Head + position) % RINGBUFFER_LEN;

	return ringBuffer.Ring_data[realPosition];
}





/********************************************************
函数名：  		deleteRingBuffer
作者：
日期：    	2022.10.08
功能：    	删除串口缓冲区中相应长度的数据
输入参数：		要删除的长度
返回值： 		void
修改记录：
**********************************************************/
void deleteRingBuffer(uint16_t size)
{
	if(size >= ringBuffer.Length)
	{
	    initRingBuffer();
	    return;
	}
	for(int i = 0; i < size; i++)
	{
		ringBuffer.Head = (ringBuffer.Head+1)%RINGBUFFER_LEN;//防止越界非法访问
		ringBuffer.Length--;
		return;
	}

}






/********************************************************
函数名：  		getRingBufferLength
作者：
日期：    	2022.10.08
功能：    	获取串口缓冲区的数据数量
输入参数：
返回值： 		串口缓冲区的数据数量
修改记录：
**********************************************************/
uint16_t getRingBufferLength()
{
	return ringBuffer.Length;
}


/********************************************************
函数名：  		isRingBufferOverflow
作者：
日期：    	2022.10.08
功能：    	判断环形缓冲区是否已满
输入参数：
返回值： 		0:环形缓冲区已满 , 1:环形缓冲区未满
修改记录：
**********************************************************/
uint8_t isRingBufferOverflow()
{
	return ringBuffer.Length < RINGBUFFER_LEN;
}


// 定义WiFi信息缓冲区
#define WIFI_SSID_MAX_LEN 32
#define WIFI_PWD_MAX_LEN 64
#define SERVE_MAX_LEN 64
static char wifi_ssid[WIFI_SSID_MAX_LEN] = {0};
static char wifi_pwd[WIFI_PWD_MAX_LEN] = {0};
static char serve_num[SERVE_MAX_LEN] = {0};
static uint8_t wifi_ssid_len = 0;
static uint8_t wifi_pwd_len = 0;
static uint8_t serve_len = 0;
static uint8_t wifi_data_type = 0; // 0:空闲 1:等待SSID 2:等待密码
static uint8_t wifi_frame_received = 0; // 标记是否已接收数据帧

/********************************************************
函数名：  		void HMI_task()
作者：
功能：    	    实现串口屏数据传输
输入参数：
修改记录：
**********************************************************/
void HMI_task()
{
    esp_received();
    while (usize >= FRAME_LENGTH)
    {
    // 校验帧头帧尾是否匹配
    if (usize >= FRAME_LENGTH && u(0) == 0x55 && u(4) == 0xff && u(5) == 0xff && u(6) == 0xff)
    {
        // 匹配，进行解析
        if (u(1) == 0x01)//water_speed置零
        {
            // total_pulses = 0;
            // total_flow = 0;
            // a = 0;
            // sprintf(str, "x1.val=%d\xff\xff\xff", (uint16_t)(total_flow * 100));
            // tjc_send_string(str);
        } else if (u(1) == 0x02)
        {
            wifi_data_type = 1;
            wifi_frame_received = 0; // 重置接收标记
            wifi_ssid_len = 0;
            memset(wifi_ssid, 0, WIFI_SSID_MAX_LEN);
            printf("开始接收SSID\r\n");
        } else if (u(1) == 0x03)
        {
            printf("SSID接收完成: %s\r\n", wifi_ssid);
            wifi_data_type = 2;
            wifi_frame_received = 0; // 重置接收标记
            wifi_pwd_len = 0;
            memset(wifi_pwd, 0, WIFI_PWD_MAX_LEN);
            printf("开始接收密码\r\n");
        }else if (u(1) == 0x04)
        {
            // 立即停止接收
            wifi_data_type = 0;
            wifi_frame_received = 0;
            
            // 调试输出
            printf("WiFi SSID: %s\r\n", wifi_ssid);
            printf("WiFi PWD: %s\r\n", wifi_pwd);
            
            // 一次性发送完整的WiFi信息给ESP32
            send_wifi_to_esp32(wifi_ssid, wifi_pwd);
            
            // 清空缓冲区
            memset(wifi_ssid, 0, WIFI_SSID_MAX_LEN);
            memset(wifi_pwd, 0, WIFI_PWD_MAX_LEN);
            wifi_ssid_len = 0;
            wifi_pwd_len = 0;
        }else if (u(1) == 0x05)
        {
            wifi_data_type = 3;
            wifi_frame_received = 0; // 重置接收标记
            serve_len = 0;
            memset(serve_num, 0, SERVE_MAX_LEN);
            printf("开始接收服务号\r\n");
        }else if (u(1) == 0x06)
        {
            // 立即停止接收
            wifi_data_type = 0;
            wifi_frame_received = 0;
            // 调试输出
            printf("serve: %s\r\n", serve_num);
            send_serve_to_esp32(serve_num);
            // 清空缓冲区
            memset(serve_num, 0, SERVE_MAX_LEN);
            serve_len = 0;
        }


        udelete(7); // 删除解析过的数据
    } 
        // 检查数据帧：70 XX XX ... FF FF FF
    else if (u(0) == 0x70 && wifi_data_type != 0 && wifi_frame_received == 0) 
    {
        // 只在接收状态且未接收过数据帧时才处理
        
        // 查找结尾的 FF FF FF
        uint8_t frame_end = 0;
        for (uint8_t i = 1; i < usize - 2; i++)
        {
            if (u(i) == 0xff && u(i+1) == 0xff && u(i+2) == 0xff)
            {
                frame_end = i;
                break;
            }
        }
        
        if (frame_end > 0) // 找到完整帧
        {
            uint8_t data_len = frame_end - 1; // 数据长度（去掉0x70帧头）
            
            if (wifi_data_type == 1) // 接收SSID
            {
                if (data_len < WIFI_SSID_MAX_LEN)
                {
                    for (uint8_t i = 0; i < data_len; i++)
                    {
                        wifi_ssid[wifi_ssid_len++] = u(i + 1);
                    }
                    wifi_ssid[wifi_ssid_len] = '\0';
                    wifi_frame_received = 1; // 标记已接收
                    printf("接收SSID数据: %s (长度:%d)\r\n", wifi_ssid, wifi_ssid_len);
                }
            }
            else if (wifi_data_type == 2) // 接收密码
            {
                if (data_len < WIFI_PWD_MAX_LEN)
                {
                    for (uint8_t i = 0; i < data_len; i++)
                    {
                        wifi_pwd[wifi_pwd_len++] = u(i + 1);
                    }
                    wifi_pwd[wifi_pwd_len] = '\0';
                    wifi_frame_received = 1; // 标记已接收
                    printf("接收密码数据: %s (长度:%d)\r\n", wifi_pwd, wifi_pwd_len);
                }
            }
            else if (wifi_data_type == 3) // 接收服务号
            {
                if (data_len < WIFI_PWD_MAX_LEN)
                {
                    for (uint8_t i = 0; i < data_len; i++)
                    {
                        serve_num[serve_len++] = u(i + 1);
                    }
                    serve_num[serve_len] = '\0';
                    wifi_frame_received = 1; // 标记已接收
                    printf("接收服务号数据: %s (长度:%d)\r\n", serve_num, serve_len);
                }
            }
            
            udelete(frame_end + 3); // 删除整个数据帧（包括3个0xFF）
        }
        else
        {
            break; // 数据不完整，等待更多数据
        }
    }
    else
    {
            // 不匹配删除1字节
        udelete(1);
        break;
    }
    }
}














/**
ʹ��ע������:
    1.��tjc_usart_hmi.c��tjc_usart_hmi.h �ֱ��빤��
    2.����Ҫʹ�õĺ������ڵ�ͷ�ļ������� #include "tjc_usart_hmi.h"


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

RingBuffer_t ringBuffer;	//����һ��ringBuffer�Ļ�����
uint8_t RxBuffer[1];


void UART_HIM_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    /* USART2 TX-->A.2   RX-->A.3 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //����PA2Ϊ�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //����PA3Ϊ��������
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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;      //��ռ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;           //�����ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                              //�ж����ȼ���ʼ��
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2,ENABLE);
}

/********************************************************
��������  		intToStr
���ڣ�    	2024.09.18
���ܣ�    	������ת��Ϊ�ַ���
���������		Ҫת������������,������ַ�������
����ֵ�� 		��
�޸ļ�¼��
**********************************************************/
void intToStr(int num, char* str) {
    int i = 0;
    int isNegative = 0;

    // ��������
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // ��ȡÿһλ����
    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num);

    // ����Ǹ��������Ӹ���
    if (isNegative) {
        str[i++] = '-';
    }

    // �����ַ�����ֹ��
    str[i] = '\0';

    // ��ת�ַ���
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
��������  		uart_send_char
���ڣ�    	2024.09.18
���ܣ�    	���ڷ��͵����ַ�
���������		Ҫ���͵ĵ����ַ�
����ֵ�� 		��
�޸ļ�¼��
**********************************************************/
void uart_send_char(char ch)
{
    uint8_t ch2 = (uint8_t)ch;
    
    // ���͵����ַ�
    USART_SendData(USART2, ch2);
    
    // �ȴ��������
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    
    return;
}

// /********************************************************
// ��������  		uart_send_char
// ���ڣ�    	2024.09.18
// ���ܣ�    	���ڷ��͵����ַ�
// ���������		Ҫ���͵ĵ����ַ�
// ����ֵ�� 		��
// �޸ļ�¼��
// **********************************************************/
// void uart_send_char(char ch)
// {
// 	uint8_t ch2 = (uint8_t)ch;
//     //������0æ��ʱ��ȴ�����æ��ʱ���ٷ��ʹ��������ַ�
// 	//while(__HAL_UART_GET_FLAG(&TJC_UART, UART_FLAG_TXE) == RESET);	//�ȴ��������
// 	while(__HAL_UART_GET_FLAG(&TJC_UART, UART_FLAG_TC) == RESET);
//     //���͵����ַ�
// 	HAL_UART_Transmit_IT(&TJC_UART, &ch2, 1);



    
//     // USART_SendData(pUSARTx, data);
    
//     // while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);



// 	return;
// }



void uart_send_string(char* str)
{
    //��ǰ�ַ�����ַ���ڽ�β ���� �ַ����׵�ַ��Ϊ��
    while(*str!=0&&str!=0)
    {
        //�����ַ����׵�ַ�е��ַ��������ڷ������֮���׵�ַ����
        uart_send_char(*str++);
    }
	return;
}

/********************************************************
��������  		tjc_send_string
���ڣ�    	2024.09.18
���ܣ�    	���ڷ����ַ����ͽ�����
���������		Ҫ���͵��ַ���
����ֵ�� 		��
ʾ��:			tjc_send_val("n0", "val", 100); ���������ݾ��� n0.val=100
�޸ļ�¼��
**********************************************************/
void tjc_send_string(char* str)
{
    //��ǰ�ַ�����ַ���ڽ�β ���� �ַ����׵�ַ��Ϊ��
    while(*str!=0&&str!=0)
    {
        //�����ַ����׵�ַ�е��ַ��������ڷ������֮���׵�ַ����
        uart_send_char(*str++);
    }
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

/********************************************************
��������  		tjc_send_txt
���ڣ�    	2024.09.18
���ܣ�    	���ڷ����ַ����ͽ�����
���������		Ҫ���͵��ַ���
����ֵ�� 		��
ʾ��:			tjc_send_txt("t0", "txt", "ABC"); ���������ݾ���t0.txt="ABC"
�޸ļ�¼��
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
��������  		tjc_send_val
���ڣ�    	2024.09.18
���ܣ�    	���ڷ����ַ����ͽ�����
���������		Ҫ���͵��ַ���
����ֵ�� 		��
�޸ļ�¼��
**********************************************************/
void tjc_send_val(char* objname, char* attribute, int val)
{
	//ƴ���ַ���,����n0.val=123
    uart_send_string(objname);
    uart_send_char('.');
    uart_send_string(attribute);
    uart_send_char('=');
    //C���������ε�ȡֵ��Χ�ǣ���-2147483648 ~ 2147483647��, �Ϊ-2147483648,���Ͻ�����\0һ��12���ַ�
    char txt[12]="";
    intToStr(val, txt);
    uart_send_string(txt);
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

/********************************************************
��������  		tjc_send_nstring
���ڣ�    	2024.09.18
���ܣ�    	���ڷ����ַ����ͽ�����
���������		Ҫ���͵��ַ���,�ַ�������
����ֵ�� 		��
�޸ļ�¼��
**********************************************************/
void tjc_send_nstring(char* str, unsigned char str_length)
{
    //��ǰ�ַ�����ַ���ڽ�β ���� �ַ����׵�ַ��Ϊ��
    for (int var = 0; var < str_length; ++var)
    {
        //�����ַ����׵�ַ�е��ַ��������ڷ������֮���׵�ַ����
        uart_send_char(*str++);
    }
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

u8 USART_Rbuffer_Num = 0;
u8 USART_Tbuffer_Num = 0;
// u8 USART_Rbuffer[buffer_len];//���ջ���������



/********************************************************
��������  		initRingBuffer
���ڣ�    	2022.10.08
���ܣ�    	��ʼ�����λ�����
���������
����ֵ�� 		void
�޸ļ�¼��
**********************************************************/
void initRingBuffer(void)
{
	//��ʼ�������Ϣ
	ringBuffer.Head = 0;
	ringBuffer.Tail = 0;
	ringBuffer.Length = 0;
	return;
}

//�����ж�
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void)
{
    
    // ��ȷ��ȡ���յ��� HEX ����
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        
        uint8_t receivedData = (uint8_t)USART_ReceiveData(USART2);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        
        write1ByteToRingBuffer(receivedData);
        USART_Rbuffer_Num++;
        
    }


}


/********************************************************
��������  		write1ByteToRingBuffer
���ڣ�    	2022.10.08
���ܣ�    	�����λ�����д������
���������		Ҫд���1�ֽ�����
����ֵ�� 		void
�޸ļ�¼��
**********************************************************/
void write1ByteToRingBuffer(uint8_t data)
{
	if(ringBuffer.Length >= RINGBUFFER_LEN) //�жϻ������Ƿ�����
	{
	return ;
	}
	ringBuffer.Ring_data[ringBuffer.Tail]=data;
	ringBuffer.Tail = (ringBuffer.Tail+1)%RINGBUFFER_LEN;//��ֹԽ��Ƿ�����
	ringBuffer.Length++;
	return ;
}
/********************************************************
��������  		read1ByteFromRingBuffer
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	�Ӵ��ڻ�������ȡ1�ֽ�����
���������		position:��ȡ��λ��
����ֵ�� 		����λ�õ�����(1�ֽ�)
�޸ļ�¼��
**********************************************************/
uint8_t read1ByteFromRingBuffer(uint16_t position)
{
	uint16_t realPosition = (ringBuffer.Head + position) % RINGBUFFER_LEN;

	return ringBuffer.Ring_data[realPosition];
}





/********************************************************
��������  		deleteRingBuffer
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	ɾ�����ڻ���������Ӧ���ȵ�����
���������		Ҫɾ���ĳ���
����ֵ�� 		void
�޸ļ�¼��
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
		ringBuffer.Head = (ringBuffer.Head+1)%RINGBUFFER_LEN;//��ֹԽ��Ƿ�����
		ringBuffer.Length--;
		return;
	}

}






/********************************************************
��������  		getRingBufferLength
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	��ȡ���ڻ���������������
���������
����ֵ�� 		���ڻ���������������
�޸ļ�¼��
**********************************************************/
uint16_t getRingBufferLength()
{
	return ringBuffer.Length;
}


/********************************************************
��������  		isRingBufferOverflow
���ߣ�
���ڣ�    	2022.10.08
���ܣ�    	�жϻ��λ������Ƿ�����
���������
����ֵ�� 		0:���λ��������� , 1:���λ�����δ��
�޸ļ�¼��
**********************************************************/
uint8_t isRingBufferOverflow()
{
	return ringBuffer.Length < RINGBUFFER_LEN;
}


// ����WiFi��Ϣ������
#define WIFI_SSID_MAX_LEN 32
#define WIFI_PWD_MAX_LEN 64
#define SERVE_MAX_LEN 64
static char wifi_ssid[WIFI_SSID_MAX_LEN] = {0};
static char wifi_pwd[WIFI_PWD_MAX_LEN] = {0};
static char serve_num[SERVE_MAX_LEN] = {0};
static uint8_t wifi_ssid_len = 0;
static uint8_t wifi_pwd_len = 0;
static uint8_t serve_len = 0;
static uint8_t wifi_data_type = 0; // 0:���� 1:�ȴ�SSID 2:�ȴ�����
static uint8_t wifi_frame_received = 0;
static int g_time_left = 0;
static int g_time_right = 24;

/********************************************************
��������  		void HMI_task()
���ߣ�
���ܣ�    	    ʵ�ִ��������ݴ���
���������
�޸ļ�¼��
**********************************************************/
void HMI_task()
{
    esp_received();
    while (usize >= FRAME_LENGTH)
    {
    // У��֡ͷ֡β�Ƿ�ƥ��
    if (usize >= FRAME_LENGTH && u(0) == 0x55 && u(4) == 0xff && u(5) == 0xff && u(6) == 0xff)
    {
        // ƥ�䣬���н���
        if (u(1) == 0x01)//water_speed����
        {
            // total_pulses = 0;
            // total_flow = 0;
            // a = 0;
            // sprintf(str, "x1.val=%d\xff\xff\xff", (uint16_t)(total_flow * 100));
            // tjc_send_string(str);
        } else if (u(1) == 0x02)
        {
            wifi_data_type = 1;
            wifi_frame_received = 0; // ���ý��ձ��
            wifi_ssid_len = 0;
            memset(wifi_ssid, 0, WIFI_SSID_MAX_LEN);
            printf("��ʼ����SSID\r\n");
        } else if (u(1) == 0x03)
        {
            printf("SSID�������: %s\r\n", wifi_ssid);
            wifi_data_type = 2;
            wifi_frame_received = 0; // ���ý��ձ��
            wifi_pwd_len = 0;
            memset(wifi_pwd, 0, WIFI_PWD_MAX_LEN);
            printf("��ʼ��������\r\n");
        }else if (u(1) == 0x04)
        {
            // ����ֹͣ����
            wifi_data_type = 0;
            wifi_frame_received = 0;
            
            // �������
            printf("WiFi SSID: %s\r\n", wifi_ssid);
            printf("WiFi PWD: %s\r\n", wifi_pwd);
            
            // һ���Է���������WiFi��Ϣ��ESP32
            send_wifi_to_esp32(wifi_ssid, wifi_pwd);
            
            // ��ջ�����
            memset(wifi_ssid, 0, WIFI_SSID_MAX_LEN);
            memset(wifi_pwd, 0, WIFI_PWD_MAX_LEN);
            wifi_ssid_len = 0;
            wifi_pwd_len = 0;
        }else if (u(1) == 0x05)
        {
            wifi_data_type = 3;
            wifi_frame_received = 0; // ���ý��ձ��
            serve_len = 0;
            memset(serve_num, 0, SERVE_MAX_LEN);
            printf("��ʼ���շ����\r\n");
        }else if (u(1) == 0x06)
        {
            wifi_data_type = 0;
            wifi_frame_received = 0;
            printf("serve: %s\r\n", serve_num);
            send_serve_to_esp32(serve_num);
            memset(serve_num, 0, SERVE_MAX_LEN);
            serve_len = 0;
        }else if (u(1) == 0x07)
        {
            WQ_SetRate(WQ_GetRate() + 0.01f);
            char rbuf[32];
            sprintf(rbuf, "t6.txt=\"%.2f\"", WQ_GetRate());
            tjc_send_string(rbuf);
        }else if (u(1) == 0x08)
        {
            float r = WQ_GetRate() - 0.01f;
            if (r < 0) r = 0;
            WQ_SetRate(r);
            char rbuf[32];
            sprintf(rbuf, "t6.txt=\"%.2f\"", WQ_GetRate());
            tjc_send_string(rbuf);
        }else if (u(1) == 0x09)
        {
            if (g_time_left < g_time_right) g_time_left++;
            char tbuf[16];
            sprintf(tbuf, "t7.txt=\"%d-%d\"", g_time_left, g_time_right);
            tjc_send_string(tbuf);
        }else if (u(1) == 0x10)
        {
            if (g_time_right > g_time_left) g_time_right--;
            char tbuf[16];
            sprintf(tbuf, "t7.txt=\"%d-%d\"", g_time_left, g_time_right);
            tjc_send_string(tbuf);
        }else if (u(1) == 0x11)
        {
            g_time_left = 0; g_time_right = 24;
            tjc_send_string("t7.txt=\"0-24\"");
        }else if (u(1) == 0x12)
        {
            Leak_SetNormalFlow(Leak_GetNormalFlow() + 0.05f);
            char fbuf[24];
            sprintf(fbuf, "t8.txt=\"%.2fL/min\"", Leak_GetNormalFlow());
            tjc_send_string(fbuf);
        }else if (u(1) == 0x13)
        {
            float f = Leak_GetNormalFlow() - 0.05f;
            if (f < 0.05f) f = 0.05f;
            Leak_SetNormalFlow(f);
            char fbuf[24];
            sprintf(fbuf, "t8.txt=\"%.2fL/min\"", Leak_GetNormalFlow());
            tjc_send_string(fbuf);
        }else if (u(1) == 0x20)
        {
            static uint8_t leak_test = 0;
            leak_test = !leak_test;
            if (leak_test) {
                tjc_send_string("t4.txt=\"测试漏水!\"");
                send_leak_to_esp32(1);
                GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
            } else {
                tjc_send_string("t4.txt=\"安全\"");
                send_leak_to_esp32(0);
                GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
                Leak_Reset();
            }
        }else if (u(1) == 0x21)
        {
            char dbuf[32];
            sprintf(dbuf, "t6.txt=\"%.2f\"", WQ_GetRate());
            tjc_send_string(dbuf);
            sprintf(dbuf, "t7.txt=\"%d-%d\"", g_time_left, g_time_right);
            tjc_send_string(dbuf);
            sprintf(dbuf, "t8.txt=\"%.2fL/min\"", Leak_GetNormalFlow());
            tjc_send_string(dbuf);
        }


        udelete(7);
    } 
        // �������֡��70 XX XX ... FF FF FF
    else if (u(0) == 0x70 && wifi_data_type != 0 && wifi_frame_received == 0) 
    {
        // ֻ�ڽ���״̬��δ���չ�����֡ʱ�Ŵ���
        
        // ���ҽ�β�� FF FF FF
        uint8_t frame_end = 0;
        for (uint8_t i = 1; i < usize - 2; i++)
        {
            if (u(i) == 0xff && u(i+1) == 0xff && u(i+2) == 0xff)
            {
                frame_end = i;
                break;
            }
        }
        
        if (frame_end > 0) // �ҵ�����֡
        {
            uint8_t data_len = frame_end - 1; // ���ݳ��ȣ�ȥ��0x70֡ͷ��
            
            if (wifi_data_type == 1) // ����SSID
            {
                if (data_len < WIFI_SSID_MAX_LEN)
                {
                    for (uint8_t i = 0; i < data_len; i++)
                    {
                        wifi_ssid[wifi_ssid_len++] = u(i + 1);
                    }
                    wifi_ssid[wifi_ssid_len] = '\0';
                    wifi_frame_received = 1; // ����ѽ���
                    printf("����SSID����: %s (����:%d)\r\n", wifi_ssid, wifi_ssid_len);
                }
            }
            else if (wifi_data_type == 2) // ��������
            {
                if (data_len < WIFI_PWD_MAX_LEN)
                {
                    for (uint8_t i = 0; i < data_len; i++)
                    {
                        wifi_pwd[wifi_pwd_len++] = u(i + 1);
                    }
                    wifi_pwd[wifi_pwd_len] = '\0';
                    wifi_frame_received = 1; // ����ѽ���
                    printf("������������: %s (����:%d)\r\n", wifi_pwd, wifi_pwd_len);
                }
            }
            else if (wifi_data_type == 3) // ���շ����
            {
                if (data_len < WIFI_PWD_MAX_LEN)
                {
                    for (uint8_t i = 0; i < data_len; i++)
                    {
                        serve_num[serve_len++] = u(i + 1);
                    }
                    serve_num[serve_len] = '\0';
                    wifi_frame_received = 1; // ����ѽ���
                    printf("���շ��������: %s (����:%d)\r\n", serve_num, serve_len);
                }
            }
            
            udelete(frame_end + 3); // ɾ����������֡������3��0xFF��
        }
        else
        {
            break; // ���ݲ��������ȴ���������
        }
    }
    else
    {
            // ��ƥ��ɾ��1�ֽ�
        udelete(1);
        break;
    }
    }
}














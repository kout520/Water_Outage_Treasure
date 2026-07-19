
#include "JLNU_OLED.h"
#include "JLNU_Font.h"

// ============================================================
// 软件I2C (PB6=SCL, PB7=SDA) — 替代STM32 HAL
// ============================================================
#define OLED_SCL_PIN  GPIO_Pin_6
#define OLED_SDA_PIN  GPIO_Pin_7
#define OLED_PORT     GPIOB

static void OLED_I2C_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // SCL和SDA均设为开漏输出
    GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN | OLED_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_PORT, &GPIO_InitStructure);
    GPIO_SetBits(OLED_PORT, OLED_SCL_PIN | OLED_SDA_PIN);
}

static void I2C_Start(void)
{
    GPIO_SetBits(OLED_PORT, OLED_SDA_PIN);
    GPIO_SetBits(OLED_PORT, OLED_SCL_PIN);
    Delay_Us(4);
    GPIO_ResetBits(OLED_PORT, OLED_SDA_PIN);
    Delay_Us(4);
    GPIO_ResetBits(OLED_PORT, OLED_SCL_PIN);
}

static void I2C_Stop(void)
{
    GPIO_ResetBits(OLED_PORT, OLED_SDA_PIN);
    GPIO_SetBits(OLED_PORT, OLED_SCL_PIN);
    Delay_Us(4);
    GPIO_SetBits(OLED_PORT, OLED_SDA_PIN);
    Delay_Us(4);
}

static uint8_t I2C_WaitAck(void)
{
    uint8_t ack;
    GPIO_SetBits(OLED_PORT, OLED_SDA_PIN);
    Delay_Us(1);
    GPIO_SetBits(OLED_PORT, OLED_SCL_PIN);
    Delay_Us(1);
    ack = GPIO_ReadInputDataBit(OLED_PORT, OLED_SDA_PIN);
    GPIO_ResetBits(OLED_PORT, OLED_SCL_PIN);
    return ack;
}

static void I2C_SendByte(uint8_t data)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (data & 0x80)
            GPIO_SetBits(OLED_PORT, OLED_SDA_PIN);
        else
            GPIO_ResetBits(OLED_PORT, OLED_SDA_PIN);
        Delay_Us(2);
        GPIO_SetBits(OLED_PORT, OLED_SCL_PIN);
        Delay_Us(2);
        GPIO_ResetBits(OLED_PORT, OLED_SCL_PIN);
        data <<= 1;
    }
}

// ============================================================
// I2C写字节 (替代HAL_I2C_Mem_Write)
// OLED I2C地址=0x78, 控制字节+数据
// ============================================================
static void I2C_WriteByte(uint8_t reg, uint8_t data)
{
    I2C_Start();
    I2C_SendByte(OLED_ADDRESS);       // OLED_ADDRESS已是8位写地址(0x78)
    I2C_WaitAck();
    I2C_SendByte(reg);                // 控制字节(0x00=命令, 0x40=数据)
    I2C_WaitAck();
    I2C_SendByte(data);               // 数据
    I2C_WaitAck();
    I2C_Stop();
}

// 毫秒延时 (替代HAL_Delay)
#define delay_ms(ms)  Delay_Ms(ms)

// 公开的I2C初始化(主函数调用)
void OLED_I2C_Init(void)
{
    OLED_I2C_GPIO_Init();
}

void OLED_WriteCmd(uint8_t command)
{

	I2C_WriteByte(0x00,command);

}

void OLED_WriteData(uint8_t data)
{

	I2C_WriteByte(0x40,data);

}

void OLED_Init(void)
{
	delay_ms(100); //�ϵ������ʱ
	
	OLED_WriteCmd(0xAE); //�ر���ʾ��0xAE�ر�OLED��0xAF��OLED��
	OLED_WriteCmd(0x20);	//����GDDRAMѰַģʽ���������������	
	OLED_WriteCmd(0x10);	//00��ˮƽѰַģʽ; 01����ֱѰַģʽ; 10��ҳѰַģʽ;
	OLED_WriteCmd(0xb0);	//����ҳѰַģʽ��ҳ��ʼλ��
	OLED_WriteCmd(0xc8);	//����ɨ�跽��
	OLED_WriteCmd(0x02); //�����е�ַ��ʼ��ַ��λ
	OLED_WriteCmd(0x10); //�����е�ַ��ʼ��ַ��λ
	OLED_WriteCmd(0x40); //����GDDRAM��ʼ��
	OLED_WriteCmd(0x81); //���öԱȶ�
	OLED_WriteCmd(0xff); //���ȵ��� 0x00~0xff
	OLED_WriteCmd(0xa1); //���ҷ��ù�
	OLED_WriteCmd(0xa6); //������ʾģʽ
	OLED_WriteCmd(0xa8); //���ø�����
	OLED_WriteCmd(0x3F); 
	OLED_WriteCmd(0xa4); //���GDDRAM�е���ʾ����
	OLED_WriteCmd(0xd3); //������ʾƫ��
	OLED_WriteCmd(0x00); //��ƫ��
	OLED_WriteCmd(0xd5); //����ʱ��
	OLED_WriteCmd(0xf0); 
	OLED_WriteCmd(0xd9); //Ԥ�������
	OLED_WriteCmd(0x22); //
	OLED_WriteCmd(0xda); //����Ӳ������
	OLED_WriteCmd(0x12);
	OLED_WriteCmd(0xdb); //����Vcomh���
	OLED_WriteCmd(0x20); //0.77*Vcc
	OLED_WriteCmd(0x8d); //���õ�ɱ�
	OLED_WriteCmd(0x14); //���õ�ɱ�
	OLED_WriteCmd(0xaf); //��OLED��ʾ
}

void OLED_SetPos(uint8_t x, uint8_t y)
{
	OLED_WriteCmd(0xb0+y);
	OLED_WriteCmd(((x&0xf0)>>4)|0x10);
	OLED_WriteCmd((x&0x0f)|0x01);
}


void OLED_Clear(void)
{
	uint8_t m,n;
	for(m=0;m<8;m++)
	{
		OLED_WriteCmd(0xb0+m);		
		OLED_WriteCmd(0x00);		
		OLED_WriteCmd(0x10);		
		for(n=0;n<130;n++)
			{
				OLED_WriteData(0x00);
			}
	}
}

void OLED_ON(void)
{
	OLED_WriteCmd(0X8D);  //���õ�ɱ�
	OLED_WriteCmd(0X14);  //������ɱ�
	OLED_WriteCmd(0XAF);  //OLED����
}

void OLED_OFF(void)
{
	OLED_WriteCmd(0X8D);  //���õ�ɱ�
	OLED_WriteCmd(0X10);  //�رյ�ɱ�
	OLED_WriteCmd(0XAE);  //OLED����
}

//===========================================================
//		x,y:��ʼλ������
//		ch���ַ������确a�� 'b'
//	TextSize:		6*8�ַ���TextSize=1��  8*16�ַ���TextSize=2��
//===========================================================
void OLED_ShowStr(uint8_t x,uint8_t y, uint8_t ch[], uint8_t TextSize)
{
	uint8_t c = 0,i = 0,j=0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
					c = ch[j] - 32;
					if(x > 122)
					{
						x = 0;
						y++;
					}
					OLED_SetPos(x,y);
					for(i=0;i<6;i++)
					{	
					OLED_WriteData(Ch6x8[c][i]);
					}
					x+=6;
					j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y+=2;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
				{
					OLED_WriteData(Ch8x16[c*16+i]);
				}
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
				{
					OLED_WriteData(Ch8x16[c*16+i+8]);
				}
				x+=8;
				j++;
			}
		}break;
	}
}

//n�����ֿ�ĵڼ����֣�ÿ��ֻ����һ������

void OLED_ShowCN(uint8_t x,uint8_t y,uint8_t n)
{
        uint8_t i=0,y1;
        y1=y+1;

        OLED_SetPos(x,y);
        for(i=0;i<16;i++)
        {
            OLED_WriteData(Ch16x16[2*n][i]);
        }
        OLED_SetPos(x,y1);
        //j++;
        for(i=0;i<16;i++)
        {
            OLED_WriteData(Ch16x16[2*n+1][i]);
        }
        //j++;
        x+=16;

}

//������Χ���������ã�������ܻ��������
//(x0:0~126, y0:0~6);(x1:1~127,y1:1~7);
void OLED_DrawBMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1)
{
	uint16_t i;
	uint8_t  x,y;
	for(y=y0;y<=y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<=x1;x++)
		{
			OLED_WriteData(BMP[i++]);
		}
	}
}

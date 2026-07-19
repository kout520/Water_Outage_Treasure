#include "MYI2C.h"

MYI2C_Struct SENx;

// ============================================================
// 微秒延时 (软件循环)
// ============================================================
void MYI2C_Delay_us(unsigned long nTim)
{
    Delay_Us(nTim);  // 使用CH32库校准延时(原软件循环16次给低速MCU的,96MHz下严重偏短)
}

// ============================================================
// GPIO模式切换 (适配CH32V307)
// ============================================================
static void SDA_SetOutput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Pin = SDA_Pin;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IIC_SDA_PORT, &GPIO_InitStruct);
}

static void SDA_SetInput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = SDA_Pin;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IIC_SDA_PORT, &GPIO_InitStruct);
}

static void SCL_SetOutput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Pin = SCL_Pin;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IIC_SCL_PORT, &GPIO_InitStruct);
}

// ============================================================
// I2C基本操作
// ============================================================
static void MYI2C_IIC_Start(void)
{
    GPIO_SetBits(IIC_SCL_PORT, SCL_Pin); MYI2C_Delay_us(4);
    GPIO_SetBits(IIC_SDA_PORT, SDA_Pin); MYI2C_Delay_us(2);
    GPIO_ResetBits(IIC_SDA_PORT, SDA_Pin); MYI2C_Delay_us(2);
    GPIO_ResetBits(IIC_SCL_PORT, SCL_Pin); MYI2C_Delay_us(2);
}

static void MYI2C_IIC_Stop(void)
{
    GPIO_ResetBits(IIC_SCL_PORT, SCL_Pin);
    GPIO_ResetBits(IIC_SDA_PORT, SDA_Pin); MYI2C_Delay_us(2);
    GPIO_SetBits(IIC_SCL_PORT, SCL_Pin); MYI2C_Delay_us(4);
    GPIO_SetBits(IIC_SDA_PORT, SDA_Pin); MYI2C_Delay_us(2);
}

static void MYI2C_IIC_Ack(unsigned char ack)
{
    GPIO_ResetBits(IIC_SCL_PORT, SCL_Pin); MYI2C_Delay_us(2);
    if (ack)
        GPIO_ResetBits(IIC_SDA_PORT, SDA_Pin);
    else
        GPIO_SetBits(IIC_SDA_PORT, SDA_Pin);
    MYI2C_Delay_us(2);
    GPIO_SetBits(IIC_SCL_PORT, SCL_Pin); MYI2C_Delay_us(4);
}

static unsigned char MYI2C_IIC_Wait_Ack(unsigned int wait_time)
{
    GPIO_ResetBits(IIC_SCL_PORT, SCL_Pin); MYI2C_Delay_us(2);
    SDA_SetInput();
    GPIO_SetBits(IIC_SDA_PORT, SDA_Pin);
    MYI2C_Delay_us(2);
    GPIO_SetBits(IIC_SCL_PORT, SCL_Pin); MYI2C_Delay_us(2);

    while (wait_time)
    {
        if (GPIO_ReadInputDataBit(IIC_SDA_PORT, SDA_Pin) == 0) break;
        MYI2C_Delay_us(1);
        wait_time--;
    }
    SDA_SetOutput();
    return wait_time;
}

static void MYI2C_Write_IIC_Byte(unsigned char dat)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        GPIO_ResetBits(IIC_SCL_PORT, SCL_Pin); MYI2C_Delay_us(2);
        if (dat & 0x80)
            GPIO_SetBits(IIC_SDA_PORT, SDA_Pin);
        else
            GPIO_ResetBits(IIC_SDA_PORT, SDA_Pin);
        MYI2C_Delay_us(2);
        GPIO_SetBits(IIC_SCL_PORT, SCL_Pin); MYI2C_Delay_us(2);
        dat <<= 1;
    }
}

static unsigned char MYI2C_Read_IIC_Byte(void)
{
    unsigned char i, byt = 0;
    GPIO_ResetBits(IIC_SCL_PORT, SCL_Pin); MYI2C_Delay_us(2);
    SDA_SetInput();
    GPIO_SetBits(IIC_SDA_PORT, SDA_Pin);
    MYI2C_Delay_us(2);

    for (i = 0; i < 8; i++)
    {
        GPIO_ResetBits(IIC_SCL_PORT, SCL_Pin); MYI2C_Delay_us(2);
        GPIO_SetBits(IIC_SCL_PORT, SCL_Pin); MYI2C_Delay_us(2);
        if (GPIO_ReadInputDataBit(IIC_SDA_PORT, SDA_Pin)) byt++;
        if (i < 7) byt <<= 1;
    }
    SDA_SetOutput();
    return byt;
}

// ============================================================
// I2C读写函数
// ============================================================
static unsigned char MYI2C_READ_FUNC(MYI2C_Struct *pst, unsigned char device_addr,
                                      unsigned char register_addr, unsigned char *pDat, unsigned char len)
{
    unsigned char NoAck = 0;
    if (register_addr)
    {
        MYI2C_IIC_Start();
        MYI2C_Write_IIC_Byte((device_addr << 1) + 0);
        if (MYI2C_IIC_Wait_Ack(Wait_Ack_time) == 0) NoAck++;
        MYI2C_Write_IIC_Byte(register_addr);
        if (MYI2C_IIC_Wait_Ack(Wait_Ack_time) == 0) NoAck++;
        GPIO_ResetBits(IIC_SCL_PORT, SCL_Pin);
        GPIO_SetBits(IIC_SCL_PORT, SCL_Pin);
    }
    MYI2C_IIC_Start();
    MYI2C_Write_IIC_Byte((device_addr << 1) + 1);
    if (MYI2C_IIC_Wait_Ack(Wait_Ack_time) == 0) NoAck++;

    while (len && NoAck == 0 && len < MYI2C_Buffer_Size)
    {
        *pDat = MYI2C_Read_IIC_Byte();
        MYI2C_IIC_Ack(len - 1);
        pDat++;
        len--;
    }
    MYI2C_IIC_Stop();
    pst->ErrFlag = NoAck;
    return NoAck;
}

static unsigned char MYI2C_WRITE_FUNC(MYI2C_Struct *pst, unsigned char device_addr,
                                       unsigned char register_addr, unsigned char *pDat, unsigned char len)
{
    unsigned int NoAck = 0;
    MYI2C_IIC_Start();
    MYI2C_Write_IIC_Byte((device_addr << 1) + 0);
    if (MYI2C_IIC_Wait_Ack(Wait_Ack_time) == 0) NoAck++;
    MYI2C_Write_IIC_Byte(register_addr);
    if (MYI2C_IIC_Wait_Ack(Wait_Ack_time) == 0) NoAck++;

    while (NoAck == 0 && len && len < MYI2C_Buffer_Size)
    {
        MYI2C_Write_IIC_Byte(*pDat);
        MYI2C_IIC_Wait_Ack(Wait_Ack_time);
        pDat++;
        len--;
    }
    MYI2C_IIC_Stop();
    pst->ErrFlag = NoAck;
    return NoAck;
}

// ============================================================
// CRC8校验
// ============================================================
unsigned char CheckCrc8(unsigned char *pDat, unsigned char Lenth)
{
    unsigned char crc = 0xff, i, j;
    for (i = 0; i < Lenth; i++)
    {
        crc ^= *pDat;
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x80) crc = (crc << 1) ^ 0x31;
            else crc <<= 1;
        }
        pDat++;
    }
    return crc;
}

// ============================================================
// 初始化
// ============================================================
void MYI2C_Init(MYI2C_Struct *pst, unsigned int ReadTimMS, unsigned char xAddr)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    pst->Adrr = xAddr;
    pst->Step = SENSOR_IDLE;
    if (ReadTimMS > MinReadTim) pst->SetRTim = ReadTimMS;
    else pst->SetRTim = MinReadTim;

    SCL_SetOutput();
    SDA_SetOutput();
    GPIO_SetBits(IIC_SCL_PORT, SCL_Pin);
    GPIO_SetBits(IIC_SDA_PORT, SDA_Pin);
}

// ============================================================
// AHT20状态机(需每10ms调用一次)
// 步骤: IDLE → 发送触发指令 → MEASURE等待80ms → READ读取7字节 → COMPLETE
// ============================================================
void MYI2C_Handle(MYI2C_Struct *pst)
{
    unsigned long s32x;

    pst->timcnt += MYI2C_Tick;

    if (pst->timcnt > PowerOnTim && pst->Step == SENSOR_IDLE)
    {
        pst->Step = SENSOR_MEASURE;
        pst->SendByte[0] = 0x33;
        pst->SendByte[1] = 0x00;
        MYI2C_WRITE_FUNC(pst, pst->Adrr, 0xAC, &pst->SendByte[0], 2);
    }
    else if (pst->timcnt > MeasureTim && pst->Step == SENSOR_MEASURE)
    {
        pst->Step = SENSOR_COMPLETE;
        MYI2C_READ_FUNC(pst, pst->Adrr, 0, &pst->ReadByte[0], 7);
        if (pst->ErrFlag == 0)
        {
            if ((CheckCrc8(&pst->ReadByte[0], 6) == pst->ReadByte[6]) &&
                ((pst->ReadByte[0] & 0x98) == 0x18))
            {
                s32x  = pst->ReadByte[1]; s32x <<= 8;
                s32x += pst->ReadByte[2]; s32x <<= 8;
                s32x += pst->ReadByte[3]; s32x >>= 4;
                pst->RH = s32x;
                pst->RH = pst->RH * 100.0f / 1048576.0f;

                s32x  = pst->ReadByte[3] & 0x0F; s32x <<= 8;
                s32x += pst->ReadByte[4]; s32x <<= 8;
                s32x += pst->ReadByte[5];
                pst->T = s32x;
                pst->T = pst->T * 200.0f / 1048576.0f - 50.0f;
            }
        }
        else
        {
            pst->RH = 0;
            pst->T  = 0;
        }
    }
    else if (pst->timcnt > pst->SetRTim)
    {
        pst->Step = SENSOR_IDLE;
        pst->timcnt = 0;
    }
}

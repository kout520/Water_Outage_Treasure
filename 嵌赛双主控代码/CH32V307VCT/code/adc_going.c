#include"headfile.h"


u16 ADC_ConvertedValue[length];


//ADC对应GPIO初始化配置以及ADC初始化配置
void adc_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    ADC_InitTypeDef  ADC_InitStructure={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1 , ENABLE ); //使能GPIOA时钟和ADC
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置ADC时钟分频为6分频

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;   //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //GPIO模式为模拟输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //配置ADC为独立模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;        //多通道模式下开启扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //设置开启连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //设置转换不是由外部触发启动，软件触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //设置ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = length;                //规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure);                    //根据ADC_InitStructure中指定的参数初始化ADC1寄存器

    ADC_Cmd(ADC1, ENABLE);      //使能ADC1

    ADC_ResetCalibration(ADC1); //重置ADC1校准寄存器。

    while(ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准结束

    ADC_StartCalibration(ADC1); //开启AD校准

    while(ADC_GetCalibrationStatus(ADC1));      //等待校准结束
}
//ADC DMA模式配置
void DMA_Tx_Init( void )
{
    DMA_InitTypeDef DMA_InitStructure={0};

    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE ); //使能开启DMA时钟

    DMA_DeInit(DMA1_Channel1); //复位DMA控制器

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->RDATAR;  //配置外设地址为ADC数据寄存器地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue; //配置存储器地址为读取ADC值地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;              //配置数据源为外设，即DMA传输方式为外设到存储器
    DMA_InitStructure.DMA_BufferSize = length;                      //设置DMA数据缓冲区大小，此处设置为length
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设置DMA外设递增模式关闭
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         //设置DMA存储器递增模式开启
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //设置外设数据大小为半字，即两个字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //设置存储器数据大小为半字，即两个字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     //设置DMA模式为循环传输模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //设置DMA传输通道优先级为高，当使用一 DMA通道时，优先级设置不影响
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;        //因为此DMA传输方式为外设到存储器，因此禁用存储器到存储器传输方式
    DMA_Init( DMA1_Channel1, &DMA_InitStructure );      //初始化DMA

    DMA_Cmd(DMA1_Channel1 , ENABLE);  //使能DMA
}
void ADC_DMA_CONF(void)
{
    adc_Init();

    DMA_Tx_Init();

    // 配置 ADC 通道转换顺序为1，第一个转换，采样时间为55.5个时钟周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5);

    // 使能ADC DMA 请求
    ADC_DMACmd(ADC1, ENABLE);

    // 由于没有采用外部触发，所以使用软件触发ADC转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}





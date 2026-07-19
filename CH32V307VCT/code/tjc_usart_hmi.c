/**
<<<<<<< HEAD
 πÔøΩÔøΩ◊¢ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ:
    1.ÔøΩÔøΩtjc_usart_hmi.cÔøΩÔøΩtjc_usart_hmi.h ÔøΩ÷±ÔøΩÔøΩÎπ§ÔøΩÔøΩ
    2.ÔøΩÔøΩÔøΩÔøΩ“™ πÔøΩ√µƒ∫ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ⁄µÔøΩÕ∑ÔøΩƒºÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ #include "tjc_usart_hmi.h"
=======
 π”√◊¢“‚ ¬œÓ:
    1.Ω´tjc_usart_hmi.c∫Õtjc_usart_hmi.h ∑÷±µº»Îπ§≥Ã
    2.‘⁄–Ë“™ π”√µƒ∫Ø ˝À˘‘⁄µƒÕ∑Œƒº˛÷–ÃÌº” #include "tjc_usart_hmi.h"
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb


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

<<<<<<< HEAD
RingBuffer_t ringBuffer;	//ÔøΩÔøΩÔøΩÔøΩ“ªÔøΩÔøΩringBufferÔøΩƒªÔøΩÔøΩÔøΩÔøΩÔøΩ
uint8_t RxBuffer[1];
char str[36];
=======
RingBuffer_t ringBuffer;	//¥¥Ω®“ª∏ˆringBufferµƒª∫≥Â«¯
uint8_t RxBuffer[1];

>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb

void UART_HIM_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    /* USART2 TX-->A.2   RX-->A.3 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
<<<<<<< HEAD
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //ÔøΩÔøΩÔøΩÔøΩPA2Œ™ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //ÔøΩÔøΩÔøΩÔøΩPA3Œ™ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
=======
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //…Ë÷√PA2Œ™∏¥”√Õ∆ÕÏ ‰≥ˆ
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;        //…Ë÷√PA3Œ™∏°ø’ ‰»Î
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
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
<<<<<<< HEAD
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;      //ÔøΩÔøΩ’ºÔøΩÔøΩÔøΩ»ºÔøΩŒ™1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;           //ÔøΩÔøΩÔøΩÔøΩÔøΩ»ºÔøΩŒ™1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQÕ®ÔøΩÔøΩ πÔøΩÔøΩ
    NVIC_Init(&NVIC_InitStructure);                              //ÔøΩ–∂ÔøΩÔøΩÔøΩÔøΩ»ºÔøΩÔøΩÔøΩ ºÔøΩÔøΩ
=======
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;      //«¿’º”≈œ»º∂Œ™1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;           //◊””≈œ»º∂Œ™1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQÕ®µ¿ πƒ‹
    NVIC_Init(&NVIC_InitStructure);                              //÷–∂œ”≈œ»º∂≥ı ºªØ
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2,ENABLE);
}

/********************************************************
<<<<<<< HEAD
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		intToStr
ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2024.09.18
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ◊™ÔøΩÔøΩŒ™ÔøΩ÷∑ÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?		“™◊™ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ,ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?
ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		ÔøΩÔøΩ
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
=======
∫Ø ˝√˚£∫  		intToStr
»’∆⁄£∫    	2024.09.18
π¶ƒ‹£∫    	Ω´’˚–Œ◊™ªªŒ™◊÷∑˚¥Æ
 ‰»Î≤Œ ˝£∫		“™◊™ªªµƒ’˚–Œ ˝æ›, ‰≥ˆµƒ◊÷∑˚¥Æ ˝◊È
∑µªÿ÷µ£∫ 		Œﬁ
–ﬁ∏ƒº«¬º£∫
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
**********************************************************/
void intToStr(int num, char* str) {
    int i = 0;
    int isNegative = 0;

<<<<<<< HEAD
    // ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
=======
    // ¥¶¿Ì∏∫ ˝
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

<<<<<<< HEAD
    // ÔøΩÔøΩ»°√ø“ªŒªÔøΩÔøΩÔøΩÔøΩ
=======
    // Ã·»°√ø“ªŒª ˝◊÷
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num);

<<<<<<< HEAD
    // ÔøΩÔøΩÔøΩÔøΩ«∏ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ”∏ÔøΩÔøΩÔøΩ?
=======
    // »Áπ˚ «∏∫ ˝£¨ÃÌº”∏∫∫≈
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    if (isNegative) {
        str[i++] = '-';
    }

<<<<<<< HEAD
    // ÔøΩÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÔøΩ÷πÔøΩÔøΩ
    str[i] = '\0';

    // ÔøΩÔøΩ◊™ÔøΩ÷∑ÔøΩÔøΩÔøΩ
=======
    // ÃÌº”◊÷∑˚¥Æ÷’÷π∑˚
    str[i] = '\0';

    // ∑¥◊™◊÷∑˚¥Æ
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
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
<<<<<<< HEAD
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		uart_send_char
ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2024.09.18
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	ÔøΩÔøΩÔøΩ⁄∑ÔøΩÔøΩÕµÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?		“™ÔøΩÔøΩÔøΩÕµƒµÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩ
ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		ÔøΩÔøΩ
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
=======
∫Ø ˝√˚£∫  		uart_send_char
»’∆⁄£∫    	2024.09.18
π¶ƒ‹£∫    	¥Æø⁄∑¢ÀÕµ•∏ˆ◊÷∑˚
 ‰»Î≤Œ ˝£∫		“™∑¢ÀÕµƒµ•∏ˆ◊÷∑˚
∑µªÿ÷µ£∫ 		Œﬁ
–ﬁ∏ƒº«¬º£∫
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
**********************************************************/
void uart_send_char(char ch)
{
    uint8_t ch2 = (uint8_t)ch;
    
<<<<<<< HEAD
    // ÔøΩÔøΩÔøΩÕµÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩ
    USART_SendData(USART2, ch2);
    
    // ÔøΩ»¥ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?
=======
    // ∑¢ÀÕµ•∏ˆ◊÷∑˚
    USART_SendData(USART2, ch2);
    
    // µ»¥˝∑¢ÀÕÕÍ≥…
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    
    return;
}

// /********************************************************
<<<<<<< HEAD
// ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		uart_send_char
// ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2024.09.18
// ÔøΩÔøΩÔøΩ‹£ÔøΩ    	ÔøΩÔøΩÔøΩ⁄∑ÔøΩÔøΩÕµÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩ
// ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?		“™ÔøΩÔøΩÔøΩÕµƒµÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩ
// ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		ÔøΩÔøΩ
// ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
=======
// ∫Ø ˝√˚£∫  		uart_send_char
// »’∆⁄£∫    	2024.09.18
// π¶ƒ‹£∫    	¥Æø⁄∑¢ÀÕµ•∏ˆ◊÷∑˚
//  ‰»Î≤Œ ˝£∫		“™∑¢ÀÕµƒµ•∏ˆ◊÷∑˚
// ∑µªÿ÷µ£∫ 		Œﬁ
// –ﬁ∏ƒº«¬º£∫
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
// **********************************************************/
// void uart_send_char(char ch)
// {
// 	uint8_t ch2 = (uint8_t)ch;
<<<<<<< HEAD
//     //ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ0√¶ÔøΩÔøΩ ±ÔøΩÔøΩ»¥ÔøΩÔøΩÔøΩÔøΩÔøΩ√¶ÔøΩÔøΩ ±ÔøΩÔøΩÔøΩŸ∑ÔøΩÔøΩÕ¥ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩ?
// 	//while(__HAL_UART_GET_FLAG(&TJC_UART, UART_FLAG_TXE) == RESET);	//ÔøΩ»¥ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?
// 	while(__HAL_UART_GET_FLAG(&TJC_UART, UART_FLAG_TC) == RESET);
//     //ÔøΩÔøΩÔøΩÕµÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩ
=======
//     //µ±¥Æø⁄0√¶µƒ ±∫Úµ»¥˝£¨≤ª√¶µƒ ±∫Ú‘Ÿ∑¢ÀÕ¥´Ω¯¿¥µƒ◊÷∑˚
// 	//while(__HAL_UART_GET_FLAG(&TJC_UART, UART_FLAG_TXE) == RESET);	//µ»¥˝∑¢ÀÕÕÍ±œ
// 	while(__HAL_UART_GET_FLAG(&TJC_UART, UART_FLAG_TC) == RESET);
//     //∑¢ÀÕµ•∏ˆ◊÷∑˚
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
// 	HAL_UART_Transmit_IT(&TJC_UART, &ch2, 1);



    
//     // USART_SendData(pUSARTx, data);
    
//     // while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);



// 	return;
// }



void uart_send_string(char* str)
{
<<<<<<< HEAD
    //ÔøΩÔøΩ«∞ÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩ⁄ΩÔøΩŒ≤ ÔøΩÔøΩÔøΩÔøΩ ÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩ◊µÔøΩ÷∑ÔøΩÔøΩŒ™ÔøΩÔøΩ
    while(*str!=0&&str!=0)
    {
        //ÔøΩÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩ◊µÔøΩ÷∑ÔøΩ–µÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ⁄∑ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ÷ÆÔøΩÔøΩÔøΩ◊µÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩ?
=======
    //µ±«∞◊÷∑˚¥Æµÿ÷∑≤ª‘⁄Ω·Œ≤ ≤¢«“ ◊÷∑˚¥Æ ◊µÿ÷∑≤ªŒ™ø’
    while(*str!=0&&str!=0)
    {
        //∑¢ÀÕ◊÷∑˚¥Æ ◊µÿ÷∑÷–µƒ◊÷∑˚£¨≤¢«“‘⁄∑¢ÀÕÕÍ≥…÷Æ∫Û ◊µÿ÷∑◊‘‘ˆ
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
        uart_send_char(*str++);
    }
	return;
}

/********************************************************
<<<<<<< HEAD
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		tjc_send_string
ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2024.09.18
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	ÔøΩÔøΩÔøΩ⁄∑ÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÕΩÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?		“™ÔøΩÔøΩÔøΩÕµÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		ÔøΩÔøΩ
 æÔøΩÔøΩ:			tjc_send_val("n0", "val", 100); ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ›æÔøΩÔøΩÔøΩ n0.val=100
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
**********************************************************/
void tjc_send_string(char* str)
{
    //ÔøΩÔøΩ«∞ÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩ⁄ΩÔøΩŒ≤ ÔøΩÔøΩÔøΩÔøΩ ÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩ◊µÔøΩ÷∑ÔøΩÔøΩŒ™ÔøΩÔøΩ
    while(*str!=0&&str!=0)
    {
        //ÔøΩÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩ◊µÔøΩ÷∑ÔøΩ–µÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ⁄∑ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ÷ÆÔøΩÔøΩÔøΩ◊µÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩ?
=======
∫Ø ˝√˚£∫  		tjc_send_string
»’∆⁄£∫    	2024.09.18
π¶ƒ‹£∫    	¥Æø⁄∑¢ÀÕ◊÷∑˚¥Æ∫ÕΩ· ¯∑˚
 ‰»Î≤Œ ˝£∫		“™∑¢ÀÕµƒ◊÷∑˚¥Æ
∑µªÿ÷µ£∫ 		Œﬁ
 æ¿˝:			tjc_send_val("n0", "val", 100); ∑¢≥ˆµƒ ˝æ›æÕ « n0.val=100
–ﬁ∏ƒº«¬º£∫
**********************************************************/
void tjc_send_string(char* str)
{
    //µ±«∞◊÷∑˚¥Æµÿ÷∑≤ª‘⁄Ω·Œ≤ ≤¢«“ ◊÷∑˚¥Æ ◊µÿ÷∑≤ªŒ™ø’
    while(*str!=0&&str!=0)
    {
        //∑¢ÀÕ◊÷∑˚¥Æ ◊µÿ÷∑÷–µƒ◊÷∑˚£¨≤¢«“‘⁄∑¢ÀÕÕÍ≥…÷Æ∫Û ◊µÿ÷∑◊‘‘ˆ
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
        uart_send_char(*str++);
    }
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

/********************************************************
<<<<<<< HEAD
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		tjc_send_txt
ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2024.09.18
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	ÔøΩÔøΩÔøΩ⁄∑ÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÕΩÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?		“™ÔøΩÔøΩÔøΩÕµÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		ÔøΩÔøΩ
 æÔøΩÔøΩ:			tjc_send_txt("t0", "txt", "ABC"); ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ›æÔøΩÔøΩÔøΩt0.txt="ABC"
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
=======
∫Ø ˝√˚£∫  		tjc_send_txt
»’∆⁄£∫    	2024.09.18
π¶ƒ‹£∫    	¥Æø⁄∑¢ÀÕ◊÷∑˚¥Æ∫ÕΩ· ¯∑˚
 ‰»Î≤Œ ˝£∫		“™∑¢ÀÕµƒ◊÷∑˚¥Æ
∑µªÿ÷µ£∫ 		Œﬁ
 æ¿˝:			tjc_send_txt("t0", "txt", "ABC"); ∑¢≥ˆµƒ ˝æ›æÕ «t0.txt="ABC"
–ﬁ∏ƒº«¬º£∫
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
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
<<<<<<< HEAD
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		tjc_send_val
ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2024.09.18
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	ÔøΩÔøΩÔøΩ⁄∑ÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÕΩÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?		“™ÔøΩÔøΩÔøΩÕµÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		ÔøΩÔøΩ
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
**********************************************************/
void tjc_send_val(char* objname, char* attribute, int val)
{
	//∆¥ÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩ,ÔøΩÔøΩÔøΩÔøΩn0.val=123
=======
∫Ø ˝√˚£∫  		tjc_send_val
»’∆⁄£∫    	2024.09.18
π¶ƒ‹£∫    	¥Æø⁄∑¢ÀÕ◊÷∑˚¥Æ∫ÕΩ· ¯∑˚
 ‰»Î≤Œ ˝£∫		“™∑¢ÀÕµƒ◊÷∑˚¥Æ
∑µªÿ÷µ£∫ 		Œﬁ
–ﬁ∏ƒº«¬º£∫
**********************************************************/
void tjc_send_val(char* objname, char* attribute, int val)
{
	//∆¥Ω”◊÷∑˚¥Æ,±»»Án0.val=123
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    uart_send_string(objname);
    uart_send_char('.');
    uart_send_string(attribute);
    uart_send_char('=');
<<<<<<< HEAD
    //CÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩŒµÔøΩ»°÷µÔøΩÔøΩŒßÔøΩ«£ÔøΩÔøΩÔøΩ-2147483648 ~ 2147483647ÔøΩÔøΩ, ÔøΩÓ≥§Œ™-2147483648,ÔøΩÔøΩÔøΩœΩÔøΩÔøΩÔøΩÔøΩÔøΩ\0“ªÔøΩÔøΩ12ÔøΩÔøΩÔøΩ÷∑ÔøΩ
=======
    //C”Ô—‘÷–’˚–Œµƒ»°÷µ∑∂Œß «£∫°∞-2147483648 ~ 2147483647°±, ◊Ó≥§Œ™-2147483648,º”…œΩ· ¯∑˚\0“ªπ≤12∏ˆ◊÷∑˚
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    char txt[12]="";
    intToStr(val, txt);
    uart_send_string(txt);
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

/********************************************************
<<<<<<< HEAD
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		tjc_send_nstring
ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2024.09.18
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	ÔøΩÔøΩÔøΩ⁄∑ÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÕΩÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?		“™ÔøΩÔøΩÔøΩÕµÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩ,ÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		ÔøΩÔøΩ
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
**********************************************************/
void tjc_send_nstring(char* str, unsigned char str_length)
{
    //ÔøΩÔøΩ«∞ÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩ⁄ΩÔøΩŒ≤ ÔøΩÔøΩÔøΩÔøΩ ÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩ◊µÔøΩ÷∑ÔøΩÔøΩŒ™ÔøΩÔøΩ
    for (int var = 0; var < str_length; ++var)
    {
        //ÔøΩÔøΩÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩ◊µÔøΩ÷∑ÔøΩ–µÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ⁄∑ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ÷ÆÔøΩÔøΩÔøΩ◊µÔøΩ÷∑ÔøΩÔøΩÔøΩÔøΩ?
=======
∫Ø ˝√˚£∫  		tjc_send_nstring
»’∆⁄£∫    	2024.09.18
π¶ƒ‹£∫    	¥Æø⁄∑¢ÀÕ◊÷∑˚¥Æ∫ÕΩ· ¯∑˚
 ‰»Î≤Œ ˝£∫		“™∑¢ÀÕµƒ◊÷∑˚¥Æ,◊÷∑˚¥Æ≥§∂»
∑µªÿ÷µ£∫ 		Œﬁ
–ﬁ∏ƒº«¬º£∫
**********************************************************/
void tjc_send_nstring(char* str, unsigned char str_length)
{
    //µ±«∞◊÷∑˚¥Æµÿ÷∑≤ª‘⁄Ω·Œ≤ ≤¢«“ ◊÷∑˚¥Æ ◊µÿ÷∑≤ªŒ™ø’
    for (int var = 0; var < str_length; ++var)
    {
        //∑¢ÀÕ◊÷∑˚¥Æ ◊µÿ÷∑÷–µƒ◊÷∑˚£¨≤¢«“‘⁄∑¢ÀÕÕÍ≥…÷Æ∫Û ◊µÿ÷∑◊‘‘ˆ
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
        uart_send_char(*str++);
    }
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

u8 USART_Rbuffer_Num = 0;
u8 USART_Tbuffer_Num = 0;
<<<<<<< HEAD
// u8 USART_Rbuffer[buffer_len];//ÔøΩÔøΩÔøΩ’ªÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
=======
// u8 USART_Rbuffer[buffer_len];//Ω” ’ª∫≥Â«¯ ˝◊È
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb



/********************************************************
<<<<<<< HEAD
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		initRingBuffer
ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2022.10.08
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	ÔøΩÔøΩ ºÔøΩÔøΩÔøΩÔøΩÔøΩŒªÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?
ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		void
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
**********************************************************/
void initRingBuffer(void)
{
	//ÔøΩÔøΩ ºÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?
=======
∫Ø ˝√˚£∫  		initRingBuffer
»’∆⁄£∫    	2022.10.08
π¶ƒ‹£∫    	≥ı ºªØª∑–Œª∫≥Â«¯
 ‰»Î≤Œ ˝£∫
∑µªÿ÷µ£∫ 		void
–ﬁ∏ƒº«¬º£∫
**********************************************************/
void initRingBuffer(void)
{
	//≥ı ºªØœ‡πÿ–≈œ¢
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
	ringBuffer.Head = 0;
	ringBuffer.Tail = 0;
	ringBuffer.Length = 0;
	return;
}

<<<<<<< HEAD
//ÔøΩÔøΩÔøΩÔøΩÔøΩ–∂ÔøΩ
=======
//Ω” ’÷–∂œ
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void)
{
    
<<<<<<< HEAD
    // ÔøΩÔøΩ»∑ÔøΩÔøΩ»°ÔøΩÔøΩÔøΩ’µÔøΩÔøΩÔøΩ HEX ÔøΩÔøΩÔøΩÔøΩ
=======
    // ’˝»∑∂¡»°Ω” ’µΩµƒ HEX  ˝æ›
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        
        uint8_t receivedData = (uint8_t)USART_ReceiveData(USART2);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        
        write1ByteToRingBuffer(receivedData);
        USART_Rbuffer_Num++;
        
    }


}


/********************************************************
<<<<<<< HEAD
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		write1ByteToRingBuffer
ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2022.10.08
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	ÔøΩÔøΩÔøΩÔøΩÔøΩŒªÔøΩÔøΩÔøΩÔøΩÔøΩ–¥ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?		“™–¥ÔøΩÔøΩÔøΩ?1ÔøΩ÷ΩÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		void
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
**********************************************************/
void write1ByteToRingBuffer(uint8_t data)
{
	if(ringBuffer.Length >= RINGBUFFER_LEN) //ÔøΩ–∂œªÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ«∑ÔøΩÔøΩÔøΩÔøΩÔøΩ
=======
∫Ø ˝√˚£∫  		write1ByteToRingBuffer
»’∆⁄£∫    	2022.10.08
π¶ƒ‹£∫    	Õ˘ª∑–Œª∫≥Â«¯–¥»Î ˝æ›
 ‰»Î≤Œ ˝£∫		“™–¥»Îµƒ1◊÷Ω⁄ ˝æ›
∑µªÿ÷µ£∫ 		void
–ﬁ∏ƒº«¬º£∫
**********************************************************/
void write1ByteToRingBuffer(uint8_t data)
{
	if(ringBuffer.Length >= RINGBUFFER_LEN) //≈–∂œª∫≥Â«¯ «∑Ò“—¬˙
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
	{
	return ;
	}
	ringBuffer.Ring_data[ringBuffer.Tail]=data;
<<<<<<< HEAD
	ringBuffer.Tail = (ringBuffer.Tail+1)%RINGBUFFER_LEN;//ÔøΩÔøΩ÷π‘ΩÔøΩÔøΩ«∑ÔøΩÔøΩÔøΩÔøΩÔøΩ?
=======
	ringBuffer.Tail = (ringBuffer.Tail+1)%RINGBUFFER_LEN;//∑¿÷π‘ΩΩÁ∑«∑®∑√Œ 
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
	ringBuffer.Length++;
	return ;
}
/********************************************************
<<<<<<< HEAD
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		read1ByteFromRingBuffer
ÔøΩÔøΩÔøΩﬂ£ÔøΩ
ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2022.10.08
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	ÔøΩ”¥ÔøΩÔøΩ⁄ªÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ»°1ÔøΩ÷ΩÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?		position:ÔøΩÔøΩ»°ÔøΩÔøΩŒªÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		ÔøΩÔøΩÔøΩÔøΩŒªÔøΩ√µÔøΩÔøΩÔøΩÔøΩÔøΩ(1ÔøΩ÷ΩÔøΩ)
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
=======
∫Ø ˝√˚£∫  		read1ByteFromRingBuffer
◊˜’ﬂ£∫
»’∆⁄£∫    	2022.10.08
π¶ƒ‹£∫    	¥”¥Æø⁄ª∫≥Â«¯∂¡»°1◊÷Ω⁄ ˝æ›
 ‰»Î≤Œ ˝£∫		position:∂¡»°µƒŒª÷√
∑µªÿ÷µ£∫ 		À˘‘⁄Œª÷√µƒ ˝æ›(1◊÷Ω⁄)
–ﬁ∏ƒº«¬º£∫
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
**********************************************************/
uint8_t read1ByteFromRingBuffer(uint16_t position)
{
	uint16_t realPosition = (ringBuffer.Head + position) % RINGBUFFER_LEN;

	return ringBuffer.Ring_data[realPosition];
}





/********************************************************
<<<<<<< HEAD
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		deleteRingBuffer
ÔøΩÔøΩÔøΩﬂ£ÔøΩ
ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2022.10.08
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	…æÔøΩÔøΩÔøΩÔøΩÔøΩ⁄ªÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ”¶ÔøΩÔøΩÔøΩ»µÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?		“™…æÔøΩÔøΩÔøΩƒ≥ÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		void
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
=======
∫Ø ˝√˚£∫  		deleteRingBuffer
◊˜’ﬂ£∫
»’∆⁄£∫    	2022.10.08
π¶ƒ‹£∫    	…æ≥˝¥Æø⁄ª∫≥Â«¯÷–œ‡”¶≥§∂»µƒ ˝æ›
 ‰»Î≤Œ ˝£∫		“™…æ≥˝µƒ≥§∂»
∑µªÿ÷µ£∫ 		void
–ﬁ∏ƒº«¬º£∫
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
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
<<<<<<< HEAD
		ringBuffer.Head = (ringBuffer.Head+1)%RINGBUFFER_LEN;//ÔøΩÔøΩ÷π‘ΩÔøΩÔøΩ«∑ÔøΩÔøΩÔøΩÔøΩÔøΩ?
=======
		ringBuffer.Head = (ringBuffer.Head+1)%RINGBUFFER_LEN;//∑¿÷π‘ΩΩÁ∑«∑®∑√Œ 
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
		ringBuffer.Length--;
		return;
	}

}






/********************************************************
<<<<<<< HEAD
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		getRingBufferLength
ÔøΩÔøΩÔøΩﬂ£ÔøΩ
ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2022.10.08
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	ÔøΩÔøΩ»°ÔøΩÔøΩÔøΩ⁄ªÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?
ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		ÔøΩÔøΩÔøΩ⁄ªÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
=======
∫Ø ˝√˚£∫  		getRingBufferLength
◊˜’ﬂ£∫
»’∆⁄£∫    	2022.10.08
π¶ƒ‹£∫    	ªÒ»°¥Æø⁄ª∫≥Â«¯µƒ ˝æ› ˝¡ø
 ‰»Î≤Œ ˝£∫
∑µªÿ÷µ£∫ 		¥Æø⁄ª∫≥Â«¯µƒ ˝æ› ˝¡ø
–ﬁ∏ƒº«¬º£∫
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
**********************************************************/
uint16_t getRingBufferLength()
{
	return ringBuffer.Length;
}


/********************************************************
<<<<<<< HEAD
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		isRingBufferOverflow
ÔøΩÔøΩÔøΩﬂ£ÔøΩ
ÔøΩÔøΩÔøΩ⁄£ÔøΩ    	2022.10.08
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	ÔøΩ–∂œªÔøΩÔøΩŒªÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ«∑ÔøΩÔøΩÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?
ÔøΩÔøΩÔøΩÔøΩ÷µÔøΩÔøΩ 		0:ÔøΩÔøΩÔøΩŒªÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ , 1:ÔøΩÔøΩÔøΩŒªÔøΩÔøΩÔøΩÔøΩÔøΩŒ¥ÔøΩÔøΩ
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
=======
∫Ø ˝√˚£∫  		isRingBufferOverflow
◊˜’ﬂ£∫
»’∆⁄£∫    	2022.10.08
π¶ƒ‹£∫    	≈–∂œª∑–Œª∫≥Â«¯ «∑Ò“—¬˙
 ‰»Î≤Œ ˝£∫
∑µªÿ÷µ£∫ 		0:ª∑–Œª∫≥Â«¯“—¬˙ , 1:ª∑–Œª∫≥Â«¯Œ¥¬˙
–ﬁ∏ƒº«¬º£∫
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
**********************************************************/
uint8_t isRingBufferOverflow()
{
	return ringBuffer.Length < RINGBUFFER_LEN;
}


<<<<<<< HEAD

static char wifi_ssid[WIFI_SSID_MAX_LEN] = {0};
static char wifi_pwd[WIFI_PWD_MAX_LEN] = {0};
char serve_num[SERVE_MAX_LEN] = {0};
static uint8_t wifi_ssid_len = 0;
static uint8_t wifi_pwd_len = 0;
static uint8_t serve_len = 0;
static uint8_t wifi_data_type = 0; // 0:ÔøΩÔøΩÔøΩÔøΩ 1:ÔøΩ»¥ÔøΩSSID 2:ÔøΩ»¥ÔøΩÔøΩÔøΩÔøΩÔøΩ
static uint8_t wifi_frame_received = 0;

static int g_time_left = 0;     // Êó∂Èó¥ÊÆµÂ∑¶ÔøΩ?
static int g_time_right = 24;   // Êó∂Èó¥ÊÆµÂè≥ÔøΩ?

/********************************************************
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ  		void HMI_task()
ÔøΩÔøΩÔøΩﬂ£ÔøΩ
ÔøΩÔøΩÔøΩ‹£ÔøΩ    	     µÔøΩ÷¥ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ›¥ÔøΩÔøΩÔøΩ
ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?
ÔøΩﬁ∏ƒºÔøΩ¬ºÔøΩÔøΩ
=======
// ∂®“ÂWiFi–≈œ¢ª∫≥Â«¯
#define WIFI_SSID_MAX_LEN 32
#define WIFI_PWD_MAX_LEN 64
#define SERVE_MAX_LEN 64
static char wifi_ssid[WIFI_SSID_MAX_LEN] = {0};
static char wifi_pwd[WIFI_PWD_MAX_LEN] = {0};
static char serve_num[SERVE_MAX_LEN] = {0};
static uint8_t wifi_ssid_len = 0;
static uint8_t wifi_pwd_len = 0;
static uint8_t serve_len = 0;
static uint8_t wifi_data_type = 0; // 0:ø’œ– 1:µ»¥˝SSID 2:µ»¥˝√‹¬Î
static uint8_t wifi_frame_received = 0; // ±Íº« «∑Ò“—Ω” ’ ˝æ›÷°

/********************************************************
∫Ø ˝√˚£∫  		void HMI_task()
◊˜’ﬂ£∫
π¶ƒ‹£∫    	     µœ÷¥Æø⁄∆¡ ˝æ›¥´ ‰
 ‰»Î≤Œ ˝£∫
–ﬁ∏ƒº«¬º£∫
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
**********************************************************/
void HMI_task()
{
    esp_received();
    while (usize >= FRAME_LENGTH)
    {
<<<<<<< HEAD
    // –£ÔøΩÔøΩ÷°Õ∑÷°Œ≤ÔøΩ«∑ÔøΩ∆•ÔøΩÔøΩ
    if (usize >= FRAME_LENGTH && u(0) == 0x55 && u(4) == 0xff && u(5) == 0xff && u(6) == 0xff)
    {
        // ∆•ÔøΩ‰£¨ÔøΩÔøΩÔøΩ–ΩÔøΩÔøΩÔøΩ
        if (u(1) == 0x01)//water_speedÔøΩÔøΩÔøΩÔøΩ
        {
            total_pulses = 10;
            total_flow = 0;
            sprintf(str,  "t13.txt=\"%d\"",total_pulses );
            tjc_send_string(str);


        } else if (u(1) == 0x02)
        {
            
            wifi_data_type = 1;
            wifi_frame_received = 0; // ÔøΩÔøΩÔøΩ√ΩÔøΩÔøΩ’±ÔøΩÔøΩ?
            wifi_ssid_len = 0;
            memset(wifi_ssid, 0, WIFI_SSID_MAX_LEN);
        } else if (u(1) == 0x03)
        {
            wifi_data_type = 2;
            wifi_frame_received = 0; // ÔøΩÔøΩÔøΩ√ΩÔøΩÔøΩ’±ÔøΩÔøΩ?
            wifi_pwd_len = 0;
            memset(wifi_pwd, 0, WIFI_PWD_MAX_LEN);
        }else if (u(1) == 0x04)
        {
            // ÔøΩÔøΩÔøΩÔøΩÕ£÷πÔøΩÔøΩÔøΩÔøΩ
            wifi_data_type = 0;
            wifi_frame_received = 0;
            
            // printf("WiFi SSID: %s\r\n", wifi_ssid);
            // printf("WiFi PWD: %s\r\n", wifi_pwd);
            
            // “ªÔøΩÔøΩÔøΩ‘∑ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩWiFiÔøΩÔøΩœ¢ÔøΩÔøΩESP32
            send_wifi_to_esp32(wifi_ssid, wifi_pwd);
            
            // ÔøΩÔøΩ’ªÔøΩÔøΩÔøΩÔøΩÔøΩ?
=======
    // –£—È÷°Õ∑÷°Œ≤ «∑Ò∆•≈‰
    if (usize >= FRAME_LENGTH && u(0) == 0x55 && u(4) == 0xff && u(5) == 0xff && u(6) == 0xff)
    {
        // ∆•≈‰£¨Ω¯––Ω‚Œˆ
        if (u(1) == 0x01)//water_speed÷√¡„
        {
            // total_pulses = 0;
            // total_flow = 0;
            // a = 0;
            // sprintf(str, "x1.val=%d\xff\xff\xff", (uint16_t)(total_flow * 100));
            // tjc_send_string(str);
        } else if (u(1) == 0x02)
        {
            wifi_data_type = 1;
            wifi_frame_received = 0; // ÷ÿ÷√Ω” ’±Íº«
            wifi_ssid_len = 0;
            memset(wifi_ssid, 0, WIFI_SSID_MAX_LEN);
            printf("ø™ ºΩ” ’SSID\r\n");
        } else if (u(1) == 0x03)
        {
            printf("SSIDΩ” ’ÕÍ≥…: %s\r\n", wifi_ssid);
            wifi_data_type = 2;
            wifi_frame_received = 0; // ÷ÿ÷√Ω” ’±Íº«
            wifi_pwd_len = 0;
            memset(wifi_pwd, 0, WIFI_PWD_MAX_LEN);
            printf("ø™ ºΩ” ’√‹¬Î\r\n");
        }else if (u(1) == 0x04)
        {
            // ¡¢º¥Õ£÷πΩ” ’
            wifi_data_type = 0;
            wifi_frame_received = 0;
            
            // µ˜ ‘ ‰≥ˆ
            printf("WiFi SSID: %s\r\n", wifi_ssid);
            printf("WiFi PWD: %s\r\n", wifi_pwd);
            
            // “ª¥Œ–‘∑¢ÀÕÕÍ’˚µƒWiFi–≈œ¢∏¯ESP32
            send_wifi_to_esp32(wifi_ssid, wifi_pwd);
            
            // «Âø’ª∫≥Â«¯
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
            memset(wifi_ssid, 0, WIFI_SSID_MAX_LEN);
            memset(wifi_pwd, 0, WIFI_PWD_MAX_LEN);
            wifi_ssid_len = 0;
            wifi_pwd_len = 0;
        }else if (u(1) == 0x05)
        {
            wifi_data_type = 3;
<<<<<<< HEAD
            wifi_frame_received = 0; // ÔøΩÔøΩÔøΩ√ΩÔøΩÔøΩ’±ÔøΩÔøΩ?
            serve_len = 0;
            memset(serve_num, 0, SERVE_MAX_LEN);

        }else if (u(1) == 0x06)
        {
            // ÔøΩÔøΩÔøΩÔøΩÕ£÷πÔøΩÔøΩÔøΩÔøΩ
            wifi_data_type = 0;
            wifi_frame_received = 0;

            // printf("serve: %s\r\n", serve_num);
            send_serve_to_esp32(serve_num);

            // ÔøΩÔøΩ»°√∞ÔøΩ≈∫ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ?
            char *serve_num1 = strchr(serve_num, ':');
            if (serve_num1 != NULL)
            {
                serve_num1++;  // ÔøΩÔøΩÔøΩÔøΩ√∞ÔøΩ≈±ÔøΩÔøΩÔøΩÔøΩÔøΩ÷∏ÔøΩÔøΩ√∞ÔøΩ≈∫ÔøΩÔøΩ“ªÔøΩÔøΩÔøΩ÷∑ÔøΩ?
                sprintf(str, "t14.txt=\"%s\"", serve_num1);  // ? ÔøΩÔøΩ %s
            }
            else
            {
                // √ªÔøΩ“µÔøΩ√∞ÔøΩ≈£ÔøΩÔøΩÔøΩÔøΩÔøΩ‘≠ ºÔøΩÔøΩÔøΩ›ªÔøΩÔøΩÔøΩ÷∑ÔøΩÔøΩÔøΩ?
                sprintf(str, "t14.txt=\"%s\"", serve_num);
            }
            tjc_send_string(str);

            // ÔøΩÔøΩ’ªÔøΩÔøΩÔøΩÔøΩÔøΩ?
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
            // ÈáçÁΩÆÊó∂Èó¥ÔøΩ?
            g_time_left = 0; g_time_right = 24;
            tjc_send_string("t7.txt=\"0-24\"");
        }else if (u(1) == 0x12)
        {
            // Ê≠£Â∏∏ÊµÅÈÄüÈòàÔøΩ? +0.05
            Leak_SetNormalFlow(Leak_GetNormalFlow() + 0.05f);
            char fbuf[24];
            sprintf(fbuf, "t8.txt=\"%.2fL/min\"", Leak_GetNormalFlow());
            tjc_send_string(fbuf);
        }else if (u(1) == 0x13)
        {
            // Ê≠£Â∏∏ÊµÅÈÄüÈòàÔøΩ? -0.05, ÊúÄÔøΩ?0.05
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
                tjc_send_string("va0.val=12");//va0.val=12
                send_leak_to_esp32(1);
                GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
            } else {
                tjc_send_string("va0.val=11");//va0.val=11
                send_leak_to_esp32(0);
                GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
                Leak_Reset();
            }
        }else if (u(1) == 0x21)
        {
            char dbuf[32];
            // t6: Ê∞¥Ë¥πÁéá
            sprintf(dbuf, "t6.txt=\"%.2f\"", WQ_GetRate());
            tjc_send_string(dbuf);
            // t7: Êó∂Èó¥ÊÆµ
            sprintf(dbuf, "t7.txt=\"%d-%d\"", g_time_left, g_time_right);
            tjc_send_string(dbuf);
            // t8: Ê≠£Â∏∏ÊµÅÈÄüÈòàÂÄº
            sprintf(dbuf, "t8.txt=\"%.2fL/min\"", Leak_GetNormalFlow());
            tjc_send_string(dbuf);
        }


        udelete(7); // …æÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
    } 
        // ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ÷°ÔøΩÔøΩ?70 XX XX ... FF FF FF
    else if (u(0) == 0x70 && wifi_data_type != 0 && wifi_frame_received == 0) 
    {
        // ÷ªÔøΩ⁄ΩÔøΩÔøΩÔøΩ◊¥Ã¨ÔøΩÔøΩŒ¥ÔøΩÔøΩÔøΩ’πÔøΩÔøΩÔøΩÔøΩÔøΩ÷° ±ÔøΩ≈¥ÔøΩÔøΩÔøΩ
        
        // ÔøΩÔøΩÔøΩ“ΩÔøΩŒ≤ÔøΩÔøΩ FF FF FF
=======
            wifi_frame_received = 0; // ÷ÿ÷√Ω” ’±Íº«
            serve_len = 0;
            memset(serve_num, 0, SERVE_MAX_LEN);
            printf("ø™ ºΩ” ’∑˛ŒÒ∫≈\r\n");
        }else if (u(1) == 0x06)
        {
            // ¡¢º¥Õ£÷πΩ” ’
            wifi_data_type = 0;
            wifi_frame_received = 0;
            // µ˜ ‘ ‰≥ˆ
            printf("serve: %s\r\n", serve_num);
            send_serve_to_esp32(serve_num);
            // «Âø’ª∫≥Â«¯
            memset(serve_num, 0, SERVE_MAX_LEN);
            serve_len = 0;
        }


        udelete(7); // …æ≥˝Ω‚Œˆπ˝µƒ ˝æ›
    } 
        // ºÏ≤È ˝æ›÷°£∫70 XX XX ... FF FF FF
    else if (u(0) == 0x70 && wifi_data_type != 0 && wifi_frame_received == 0) 
    {
        // ÷ª‘⁄Ω” ’◊¥Ã¨«“Œ¥Ω” ’π˝ ˝æ›÷° ±≤≈¥¶¿Ì
        
        // ≤È’“Ω·Œ≤µƒ FF FF FF
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
        uint8_t frame_end = 0;
        for (uint8_t i = 1; i < usize - 2; i++)
        {
            if (u(i) == 0xff && u(i+1) == 0xff && u(i+2) == 0xff)
            {
                frame_end = i;
                break;
            }
        }
        
<<<<<<< HEAD
        if (frame_end > 0) // ÔøΩ“µÔøΩÔøΩÔøΩÔøΩÔøΩ÷°
        {
            uint8_t data_len = frame_end - 1; // ÔøΩÔøΩÔøΩ›≥ÔøΩÔøΩ»£ÔøΩ»•ÔøΩÔøΩ0x70÷°Õ∑ÔøΩÔøΩ
            
            if (wifi_data_type == 1) // ÔøΩÔøΩÔøΩÔøΩSSID
=======
        if (frame_end > 0) // ’“µΩÕÍ’˚÷°
        {
            uint8_t data_len = frame_end - 1; //  ˝æ›≥§∂»£®»•µÙ0x70÷°Õ∑£©
            
            if (wifi_data_type == 1) // Ω” ’SSID
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
            {
                if (data_len < WIFI_SSID_MAX_LEN)
                {
                    for (uint8_t i = 0; i < data_len; i++)
                    {
                        wifi_ssid[wifi_ssid_len++] = u(i + 1);
                    }
                    wifi_ssid[wifi_ssid_len] = '\0';
<<<<<<< HEAD
                    wifi_frame_received = 1; // ÔøΩÔøΩÔøΩÔøΩ—ΩÔøΩÔøΩÔøΩ?
                    // printf("SSID: %s (%d)\r\n", wifi_ssid, wifi_ssid_len);
                }
            }
            else if (wifi_data_type == 2) // ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
=======
                    wifi_frame_received = 1; // ±Íº«“—Ω” ’
                    printf("Ω” ’SSID ˝æ›: %s (≥§∂»:%d)\r\n", wifi_ssid, wifi_ssid_len);
                }
            }
            else if (wifi_data_type == 2) // Ω” ’√‹¬Î
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
            {
                if (data_len < WIFI_PWD_MAX_LEN)
                {
                    for (uint8_t i = 0; i < data_len; i++)
                    {
                        wifi_pwd[wifi_pwd_len++] = u(i + 1);
                    }
                    wifi_pwd[wifi_pwd_len] = '\0';
<<<<<<< HEAD
                    wifi_frame_received = 1; // ÔøΩÔøΩÔøΩÔøΩ—ΩÔøΩÔøΩÔøΩ?
                    // printf("PWD: %s (%d)\r\n", wifi_pwd, wifi_pwd_len);
                }
            }
            else if (wifi_data_type == 3) // ÔøΩÔøΩÔøΩ’∑ÔøΩÔøΩÔøΩÔøΩ?
=======
                    wifi_frame_received = 1; // ±Íº«“—Ω” ’
                    printf("Ω” ’√‹¬Î ˝æ›: %s (≥§∂»:%d)\r\n", wifi_pwd, wifi_pwd_len);
                }
            }
            else if (wifi_data_type == 3) // Ω” ’∑˛ŒÒ∫≈
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
            {
                if (data_len < WIFI_PWD_MAX_LEN)
                {
                    for (uint8_t i = 0; i < data_len; i++)
                    {
                        serve_num[serve_len++] = u(i + 1);
                    }
                    serve_num[serve_len] = '\0';
<<<<<<< HEAD
                    wifi_frame_received = 1; // ÔøΩÔøΩÔøΩÔøΩ—ΩÔøΩÔøΩÔøΩ?
                    // printf("SERVE: %s (%d)\r\n", serve_num, serve_len);
                }
            }
            
            udelete(frame_end + 3); // …æÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ÷°ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ3ÔøΩÔøΩ0xFFÔøΩÔøΩ
        }
        else
        {
            break; // ÔøΩÔøΩÔøΩ›≤ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ»¥ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
=======
                    wifi_frame_received = 1; // ±Íº«“—Ω” ’
                    printf("Ω” ’∑˛ŒÒ∫≈ ˝æ›: %s (≥§∂»:%d)\r\n", serve_num, serve_len);
                }
            }
            
            udelete(frame_end + 3); // …æ≥˝’˚∏ˆ ˝æ›÷°£®∞¸¿®3∏ˆ0xFF£©
        }
        else
        {
            break; //  ˝æ›≤ªÕÍ’˚£¨µ»¥˝∏¸∂‡ ˝æ›
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
        }
    }
    else
    {
<<<<<<< HEAD
            // ÔøΩÔøΩ∆•ÔøΩÔøΩ…æÔøΩÔøΩ1ÔøΩ÷ΩÔøΩ
=======
            // ≤ª∆•≈‰…æ≥˝1◊÷Ω⁄
>>>>>>> c63018c7a2f4c111caab0dc82632fa181bd609cb
        udelete(1);
        break;
    }
    }
}














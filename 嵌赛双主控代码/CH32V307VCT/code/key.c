#include "headfile.h"

uint16_t B1_state;
uint16_t B1_state_last;
uint16_t B2_state;
uint16_t B2_state_last;

uint8_t j;
uint8_t i;

void key_scan(void)
{
    B1_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    B2_state = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
    
    if(B1_state == 0 && B1_state_last == 1 )
    {
    }
    else if(B2_state == 0 && B2_state_last == 1 )
    {
        GPIO_WriteBit(GPIOE, GPIO_Pin_1, (j==0) ? (j=Bit_SET):(j=Bit_RESET));
        
    }

    B1_state_last = B1_state;
    B2_state_last = B2_state;


        
}

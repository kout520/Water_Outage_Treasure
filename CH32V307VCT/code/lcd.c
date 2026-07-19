#include "headfile.h"


char str1[100];
uint8_t flag=1;





//显示流量/速
void lcd_speed(void)
{



}


//显示水质
void lcd_water(void)
{

    sprintf(str1, "t7.txt=\"%d\"", ADC_ConvertedValue[0]);
    tjc_send_string(str1);
    sprintf(str1, "t13.txt=\"%d\"", ADC_ConvertedValue[1]);
    tjc_send_string(str1);

}


//显示月费
void lcd_take(void)
{

}

//置零
void lcd_clear(void)
{



}















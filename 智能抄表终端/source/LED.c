#include "LED.h"

/****************************************************************************
函数名称:   LED_Init
功能描述:   LED初始化
****************************************************************************/
void LED_Init(void)
{
  GPIO_Config(GPIO_LED_UP,LED_UP,GPIO_OUT_PP);	
  GPIO_Config(GPIO_LED_DOWN,LED_DOWN,GPIO_OUT_PP);	
  GPIO_Config(GPIO_LED_WARNING,	LED_WARNNING,GPIO_OUT_PP);	

  GPIO_WordWrite(GPIO_LED_UP, GPIO_WordRead(GPIO_LED_UP) |LED_UP);  //灭掉上行灯
  GPIO_WordWrite(GPIO_LED_DOWN, GPIO_WordRead(GPIO_LED_DOWN) |(LED_DOWN));  //灭掉下行灯
  GPIO_WordWrite(GPIO_LED_WARNING,GPIO_WordRead(GPIO_LED_WARNING) |(LED_WARNNING));//灭掉报报警灯
}

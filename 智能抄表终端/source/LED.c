#include "LED.h"

/****************************************************************************
��������:   LED_Init
��������:   LED��ʼ��
****************************************************************************/
void LED_Init(void)
{
  GPIO_Config(GPIO_LED_UP,LED_UP,GPIO_OUT_PP);	
  GPIO_Config(GPIO_LED_DOWN,LED_DOWN,GPIO_OUT_PP);	
  GPIO_Config(GPIO_LED_WARNING,	LED_WARNNING,GPIO_OUT_PP);	

  GPIO_WordWrite(GPIO_LED_UP, GPIO_WordRead(GPIO_LED_UP) |LED_UP);  //������е�
  GPIO_WordWrite(GPIO_LED_DOWN, GPIO_WordRead(GPIO_LED_DOWN) |(LED_DOWN));  //������е�
  GPIO_WordWrite(GPIO_LED_WARNING,GPIO_WordRead(GPIO_LED_WARNING) |(LED_WARNNING));//�����������
}

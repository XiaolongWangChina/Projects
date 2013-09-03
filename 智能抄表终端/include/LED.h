#ifndef __LED_H__
#define __LED_H__
#include "71x_lib.h"
#include "gpio.h"

//����LED�ļ����궨��
#define	LED_WARNNING		(1UL<<3)	//P1.3
#define	LED_DOWN			(1UL<<4)	//P1.4
#define	LED_UP				(1UL<<5)	//P1.5

/****************************************************************************
��������:   LED_Init
��������:   LED��ʼ��
****************************************************************************/
void LED_Init(void);

#endif

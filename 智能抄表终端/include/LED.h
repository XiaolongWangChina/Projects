#ifndef __LED_H__
#define __LED_H__
#include "71x_lib.h"
#include "gpio.h"

//关于LED的几个宏定义
#define	LED_WARNNING		(1UL<<3)	//P1.3
#define	LED_DOWN			(1UL<<4)	//P1.4
#define	LED_UP				(1UL<<5)	//P1.5

/****************************************************************************
函数名称:   LED_Init
功能描述:   LED初始化
****************************************************************************/
void LED_Init(void);

#endif

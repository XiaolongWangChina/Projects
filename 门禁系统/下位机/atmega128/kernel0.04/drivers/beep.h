#ifndef _BEEP_H_
#define _BEEP_H_

#include "../include.h"

// Beep初始化
#define BeepInit()			\
	do {				\
		DDRF |= BM(DOORBEEP);	\
		PORTF &= ~BM(DOORBEEP);	\
	} while (0)

// 设置蜂鸣位
#define SET_BEEP()	(PORTF |= BM(DOORBEEP))

// 清除蜂鸣位
#define CLR_BEEP()	(PORTF &= ~BM(DOORBEEP))

// 蜂鸣
#define BEEP(n, v)				\
	do {					\
		unsigned char i;		\
		for (i = n; i > 0; i--) {	\
			SET_BEEP();		\
			halWait(v);		\
			CLR_BEEP();		\
			halWait(v);		\
		}				\
	} while (0)

#endif

#ifndef _DOOR_H_
#define _DOOR_H_

#include "../include.h"

//门禁初始化
#define DoorInit()			\
	do {				\
		DDRE |= BM(DOOR_OUT);	\
		DDRE &= ~BM(DOOR_IN);	\
		PORTE |= BM(DOOR_OUT);	\
	} while (0)

//判断门是否打开
#define IsDoorOpen()	!(PINE & (BM(DOOR_IN)))

//打开门
#define DoorOpen()	PORTE |= BM(DOOR_OUT)

//关闭门
#define DoorClose()	PORTE &= ~ BM(DOOR_OUT)

#endif

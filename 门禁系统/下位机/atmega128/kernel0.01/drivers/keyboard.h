#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "../include.h"

//键盘初始化
void keyboard_init(void);

//获取一个键值
unsigned char GetAKey(void);

// 复位键盘状态
void Keyboard_reset(void);

#endif

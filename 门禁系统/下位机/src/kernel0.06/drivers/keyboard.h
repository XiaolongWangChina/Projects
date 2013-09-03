#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "../include.h"

//键盘初始化
void keyboard_init(void);

//获取一个键值
unsigned char GetAKey(void);

// 复位键盘状态
void Keyboard_reset(void);

//获取数字按键对应的值
unsigned char GetKeyValue(unsigned char key);

//允许键盘中断
#define Enable_KEYBOARD()  EIMSK |= BM(INT4);  
//禁止键盘中断        
#define Disable_KEYBOARD() EIMSK &= ~(BM(INT4));

#endif

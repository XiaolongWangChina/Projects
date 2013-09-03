#include "keyboard.h"
#include "led.h"
#include "beep.h"
#include "../tasks.h"

//键盘初始化
void keyboard_init(void) {
	//外部中断初始化
	EICRB |= BM(ISC41);
	EICRB &= ~BM(ISC40);//INT4为下降沿中断
	EIMSK |= BM(INT4);
	//设置端口状态
	//b4 b5 b6 b7
	DDRB |= 0xf0;//低四位输出
	PORTB &= 0x0F;//输出低电平
	//f4 f5 f6 f7
	DDRF &= 0x0f;//高四位输入
}

/*********************************************
 * 键盘----扫描码
 * {
 * {'1','2','3','F1',},
 * {'4','5','6','F2',},
 * {'7','8','9','F3',},
 * {'*','0','#','F4',},
 * };
 * {
 * {0x18,0x28,0x48,0x88,},
 * {0x14,0x24,0x44,0x84,},
 * {0x12,0x22,0x42,0x82,},
 * {0X11,0x21,0x41,0x81,},
 * };
 * 各个功能键的作用
 * F1 密码验证 F2 指纹验证
 * F3 清除输入数字 F4 确认
 * '*' 修改密码 '#'修改指纹
 **********************************************/
unsigned char GetAKey(void) {
	unsigned char key; //按键
	unsigned char temp;

	
	temp = PINF&0xF0;//与掉低四位
	if(temp==0xF0) {
		return 0; // 无按键返回
	} else {
		halWait(2000);
		temp = PINF&0xF0; //延时去抖后再检测
		if(temp==0xF0)
			return 0;
		else
			key= ((~temp)&0xF0) >> 4;
	}
	
	//翻转
	DDRB &= 0x0f;//低四位输入
	DDRF |= 0xf0;//高四位输出
	PORTF |= 0xf0;//输出低电平

	halWait(2000);   //延时等待稳定
	temp = PINB & 0xF0;//与掉高四位
	if(temp == 0xF0)
		return 0; // 无按键返回
	else   //这里不再延时再扫描，因为已经确定了不是抖动才会进入本步操作。
	{
		//对调数据
		//0001 1000
		//0010 0100
		//0100 0010
		//1000 0001
		//对调数据并移位
		switch(temp){
			case 0x10: temp = 0x80; break;
			case 0x20: temp = 0x40; break;
			case 0x40: temp = 0x20; break;
			case 0x80: temp = 0x10; break;
			default:  return;
		}

		key |= temp; //高低位的键值进入KEY
  
		BEEP(1,15000);
	}

	return key;
}


// 复位键盘状态
void Keyboard_reset(void) {
	//设置端口状态
	DDRB |= 0xf0; //低4位输出
	PORTB &= 0x0f; //输出低电平
	DDRF &= 0x0f; //高4位输入
	EIFR |= BM(INTF4);
}


//键盘中断处理程序
SIGNAL(SIG_INTERRUPT4) {
	
	DISABLE_GLOBAL_INT();
	
	unsigned char key = GetAKey();
	
	switch (key) {
		//F1 密码验证
		case 0x88:
			SET_DLED1();
			BEEP(1, 2500);
			passwdauth = 1; 
			break;
			//F2 指纹验证
		case 0x84:
			SET_DLED2();
			BEEP(2, 2500);
			fingermapauth = 1;
			break; 
			// * 修改密码
		case 0x11:
			SET_DLED3();
			BEEP(3, 2500);
			passwdmodify = 1;
			break;
			// # 修改指纹
		case 0x41:
			SET_DLED4();
			BEEP(4, 2500);
			fingermapmodify = 1;
			break;
			// F4确认,则依次调用各个处理程序
		case 0x81:
			SET_DLED5();
			BEEP(5, 2500);
			// 依次检查各个任务
			confirm = 1;
			
			break;
		default:
			SET_DLED4();
			if (passwdauth) {
				get_key_count++;
				if (get_key_count <= 4) {
					name[get_key_count - 1] = key;
				} else if (get_key_count <= 10) {
					passwd[get_key_count - 5] = key;
				} else { //多余键舍弃
				}
			}else if(passwdmodify){
				get_key_count++;
				if(get_key_count <= 6){
					passwd[get_key_count-1] = key;
				}else if(get_key_count <= 12){
					passwd_confirm[get_key_count-7] = key;
				}else{ //多余键舍弃
				}

			}
			BEEP(6, 2500);
			CLR_DLED4();
			break;
	}
	
	//复位键盘状态
	Keyboard_reset();
	ENABLE_GLOBAL_INT();
}



//获取数字按键对应的值
unsigned char GetKeyValue(unsigned char key) {
	unsigned char result;
	switch(key) {
		case 0x18:
			result = 1;
			break;
		case 0x28:
			result = 2;
			break;
		case 0x48:
			result = 3;
			break;
		case 0x14:
			result = 4;
			break;
		case 0x24:
			result = 5;
			break;
		case 0x44:
			result = 6;
			break;
		case 0x12:
			result = 7;
			break;
		case 0x22:
			result = 8;
			break;
		case 0x42:
			result = 9;
			break;
		case 0x21:
			result = 0;
			break;
		default:
			result = -1;
			break;
	}
	return result;
}
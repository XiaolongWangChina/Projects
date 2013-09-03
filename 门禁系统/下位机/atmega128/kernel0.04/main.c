#include "include.h"
#include "drivers/door.h"
#include "drivers/beep.h"
#include "drivers/led.h"
#include "drivers/uart0.h"
#include "drivers/uart1.h"
#include "drivers/keyboard.h"
#include "tasks.h"
#include "drivers/fingermap.h"

//主方法
int main(void) {
	////设备初始化阶段////
	//蜂鸣器
	BeepInit();
	//门禁
	DoorInit();
	//LED初始化
	LED_init();

	//UART0
	UART0_InitUART(230400);
	UART1_InitUART(230400);
	
	//键盘初始化
	keyboard_init();
	
	DoorClose();

	// 全局中断使能
	ENABLE_GLOBAL_INT();

	while (1) {
	
		ENABLE_GLOBAL_INT();
		
		//首先关门。
		if (IsDoorOpen()) {
			DoorClose();
		}

		//检查有无收到一帧，有则处理。
		if (if_receive_a_frame()) {
			BEEP(5,100);
			SET_DLED3();
			Reset_receive_frame();
			process_commands();
		}
		
		//密码验证，输入确认后
		//用户按 F1 -> 输入用户名 密码 -> F4 ，confirm=1
		if(confirm){
			confirm = 0;
			
			//密码传输有点问题。。。。
			password_authentication();
			halWait(250000);
		}
		
		//指纹验证 用户按F2
		if (fingermapauth) {
			fingermap_authentication();
		}
		
		
		//指纹修改。用户按#
		if (fingermapmodify) {
			fingermap_modify();
		}
		
		//密码修改。用户按下 * 
		if(passwdmodify){
			password_modify();
		}
		
		
		
		
		
		DISABLE_GLOBAL_INT();
		
	}

	return 0;
}

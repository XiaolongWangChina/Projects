#include "include.h"
#include "drivers/door.h"
#include "drivers/beep.h"
#include "drivers/led.h"
#include "drivers/uart0.h"
#include "drivers/keyboard.h"

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
	
	//键盘初始化
	keyboard_init();
	
	DoorClose();

	// 全局中断使能
	ENABLE_GLOBAL_INT();

	while (1) {
		if (IsDoorOpen()) {
			DoorClose();
		}

		if (if_receive_a_frame()) {
			Reset_receive_frame();
			//process_commands();
		}
	}

	return 0;
}

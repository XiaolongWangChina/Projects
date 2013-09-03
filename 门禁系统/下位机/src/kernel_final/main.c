#include "include.h"
#include "drivers/door.h"
#include "drivers/beep.h"
#include "drivers/led.h"
#include "drivers/uart0.h"
#include "drivers/uart1.h"
#include "drivers/keyboard.h"
#include "drivers/fingermap.h"
#include "kernel/timer.h"
#include "kernel/scheduler.h"
#include "kernel/tasks.h"

unsigned char stack[600];

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
	UART1_InitUART(115200);
	
	//键盘初始化
	keyboard_init();
	
	//初始化定时器
	TickInit();
	Tick0Init();
	
	DoorClose();

	// 全局中断使能
	ENABLE_GLOBAL_INT();

	ready_table = 0;
	current = 0;

	task_create(password_authentication, &stack[99], 0);
	task_create(password_modify, &stack[199], 1);
	task_create(fingermap_authentication, &stack[299], 2);
	task_create(fingermap_modify, &stack[399], 3);
	task_create(process_commands, &stack[499], 4);
	task_create(scheduler, &stack[599], 5);

	start_schedule();

	return 0;
}

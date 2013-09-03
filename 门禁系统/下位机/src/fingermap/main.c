#include "include.h"
#include "drivers/beep.h"
#include "drivers/led.h"
#include "drivers/fingermap.h"
#include "drivers/uart1.h"

//主方法
int main(void) {
	////设备初始化阶段////
	//蜂鸣器
	BeepInit();
	//LED初始化
	
	LED_init();

	UART1_InitUART(115200);
	
	// 全局中断使能
	ENABLE_GLOBAL_INT();

	//指纹录入例程
	fingermap_enroll_apl();
	
	SET_DLED2();
	
	while(fingermap_auth_apl())
	{
		BEEP(9,18000);//OK
	}
	SET_DLED1();
	
	halWait(500000);
	CLR_DLED1();
	CLR_DLED2();
	CLR_DLED3();
	CLR_DLED4();
	CLR_DLED5();

	//删除指纹
	if(!fingermap_erase_apl(fingermap_id))
	{
		SET_DLED5();
		BEEP(5, 10000);
	}
	
	while(fingermap_auth_apl())
	{
		BEEP(9,18000);//OK
	}
	SET_DLED1();

	while (1) {
		SET_DLED5();
		halWait(50000);
		CLR_DLED5();
		halWait(50000);
	}

	return 0;
}

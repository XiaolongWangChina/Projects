#include "fingermap.h"
#include "uart1.h"
#include "led.h"
#include "beep.h"
#include "../tasks.h"

/*****************************************************************
 User_enroll()注册用户
 Command ID:0x21
协议头	协议长度	固定字段	命令码	指纹序号	协议尾
02	04	        01 02	        21	MM	        02 01 03

******************************************************************/
unsigned char User_enroll(unsigned char ucFPIdx)
{
	unsigned char txbuff[9];
	unsigned char rxbuff[11];

	txbuff[0]=0x02;
	txbuff[1]=0x04;
	txbuff[2]=0x01;
	txbuff[3]=0x02;
	txbuff[4]=0x21;
	txbuff[5]=ucFPIdx;  //未指定指纹存储序号		
	txbuff[6]=0x02;
	txbuff[7]=0x01;
	txbuff[8]=0x03;
	
	SET_DLED5();
	
	UART1_write(txbuff,9);
	
	UART1_read(rxbuff,11);
	
	if(   (rxbuff[0]!= 0x02)||(rxbuff[1]!= 0x06)||(rxbuff[2]!= 0x04)||(rxbuff[3]!= 0x04)
		||(rxbuff[4]!= 0x21)||(rxbuff[5]!= 0x01)||(rxbuff[6]!= 0x02)||(rxbuff[7]!= 0xFF)
		||(rxbuff[8]!= 0x02)||(rxbuff[9]!= 0x01)||(rxbuff[10]!= 0x06))
	{
		//验证错误
		BEEP(3,18000);
		
		CLR_DLED5();
		
		User_enroll(0xFF);
	} else {
		BEEP(1,18000);
		
		SET_DLED4();
		
		UART1_read(rxbuff,11);
		
		if(   (rxbuff[0]!= 0x02)||(rxbuff[1]!= 0x06)||(rxbuff[2]!= 0x04)||(rxbuff[3]!= 0x04)
			||(rxbuff[4]!= 0x21)||(rxbuff[5]!= 0x02)||(rxbuff[6]!= 0x02)||(rxbuff[7]!= 0xFF)
			||(rxbuff[8]!= 0x02)||(rxbuff[9]!= 0x01)||(rxbuff[10]!= 0x06))
		{
			//验证错误
			BEEP(3,18000);
			
			CLR_DLED4();
			CLR_DLED5();
			
			User_enroll(0xFF);
		}
		else
		{
			BEEP(2,18000);
			
			SET_DLED3();
			
			UART1_read(rxbuff,11);
			
			if(   (rxbuff[0]!= 0x02)||(rxbuff[1]!= 0x06)||(rxbuff[2]!= 0x04)||(rxbuff[3]!= 0x04)
				||(rxbuff[4]!= 0x21)||(rxbuff[5]!= 0x03)||(rxbuff[6]!= 0x01)
				||(rxbuff[8]!= 0x02)||(rxbuff[9]!= 0x01)||(rxbuff[10]!= 0x06))
			{
				//验证错误
				BEEP(3,18000);

				CLR_DLED3();
				CLR_DLED4();
				CLR_DLED5();
				
				User_enroll(0xFF);
			}
			else
			{
				SET_DLED2();
				
				//注册成功
				BEEP(1,60000);BEEP(2,25000);// by sprife
				halWait(50000);
				
				fingermapid = rxbuff[7];
				
				CLR_DLED2();
				CLR_DLED3();
				CLR_DLED4();
				CLR_DLED5();
				
				return 0;
			}
		}
	}	
}


//指纹注册的函数封装
unsigned char fingermap_enroll_apl(){
	unsigned char error = 1;
	while(1){ 
	
		//屏蔽键盘中断
		Disable_KEYBOARD();
		//刷新UART1输入缓冲区
		UART1_RX_flash();
		
		error = User_enroll(0xFF);
		
		//打开键盘中断
		Enable_KEYBOARD();
		
		if(!error){
			SET_DLED2();
			halWait(50000);
			CLR_DLED2();
			halWait(50000);
			
			return 0;
		}
	}
}

/*****************************************************************
 User_auth()指纹验证
 Command ID:0x24
协议头	协议长度	固定字段	命令码	指纹序号	协议尾
02		04			01 02			24		MM		02 01 03

******************************************************************/
unsigned char User_auth(unsigned char ucFPIdx)
{
	unsigned char txbuff[9];
	unsigned char rxbuff[9];
	
	txbuff[0]=0x02;
	txbuff[1]=0x04;
	txbuff[2]=0x01;
	txbuff[3]=0x02;
	
	txbuff[4]=0x24;
	txbuff[5]=0xFF;  //未指定指纹序号，为1:N验证
	txbuff[5]=ucFPIdx; //指定指纹号，为1:1验证 二选一
	
	txbuff[6]=0x02;
	txbuff[7]=0x01;
	txbuff[8]=0x03;

	//发送命令
	UART1_write(txbuff,9);

	//接收返回数据信息
	if(UART1_read(rxbuff,9) < 0){
		return 1;
	}

	if(rxbuff[0] == 0x02 && rxbuff[4] == 0x24){
			//验证成功
			if (rxbuff[5] == 0xFF) {
				//指纹验证错误
				BEEP(1,18000);
			} else if (rxbuff[5] == 0xFC) {
				//指纹Fail
				BEEP(3,18000);
			}else if (rxbuff[5] == 0xFE) {
				//指纹验证超时
				BEEP(2,18000);
			} else if (( rxbuff[5] >=0x01 ) && ( rxbuff[5] <= 0xF0 )){
				//指纹验证成功（1-200）
				SET_DLED5();
				SET_DLED4();
				SET_DLED3();
				
				fingermapid = rxbuff[5];
				
				return 0;
			} else {
				//其他
				BEEP(4,18000);
			}
	} else {
		//验证错误
		BEEP(5,18000);
	}
	
	return 1;
}

//指纹验证的函数封装
unsigned char fingermap_auth_apl(){
	unsigned char error = 0, i;
	
	for (i = 0; i < 3; i++){
		//屏蔽键盘中断
		Disable_KEYBOARD();
		//刷新输入缓冲区
		UART1_RX_flash();
		
		error = User_auth(0xff);
		
		//打开键盘中断
		Enable_KEYBOARD();
		
		if(!error){
			//LED 2	    
			SET_DLED2();
			halWait(50000);
			CLR_DLED2();
			halWait(50000);
			
			return 0;
		}
	}
	return 1;
}



/*****************************************************************
 User_erase()删除所指定指纹记录
 Command ID:0x22
协议头	协议长度	固定字段	命令码	指纹序号	协议尾
02	04		01 02		22	MM		02 01 03

******************************************************************/
unsigned char User_erase(unsigned char ucFPIdx){
	
	unsigned char txbuff[9];
	unsigned char rxbuff[9];
	txbuff[0]=0x02;
	txbuff[1]=0x04;
	txbuff[2]=0x01;
	txbuff[3]=0x02;
	
	txbuff[4]=0x22;
	txbuff[5]=ucFPIdx;  //所要删除的指纹序号
	
	txbuff[6]=0x02;
	txbuff[7]=0x01;
	txbuff[8]=0x03;
	
	//发送命令
	UART1_write(txbuff,9);
	//接收返回数据信息
	if(UART1_read(rxbuff,9) < 0){
		BEEP(3,18000);//add by lyj_uptech@126.com
		return 1;
	}
	
	//处理接收到的消息
	
	if(rxbuff[4] != 0x22)
	{
		return 1;
	}

	
	if( rxbuff[5] == 0xFE )
	{
		// 表示指纹不存在
		return 1;
	}
	else if( rxbuff[5] == 0xFF )
	{
		//删除失败
		return 1;
	}
	else if( rxbuff[5] == 0xFC )
	{
		return 1;
	}
	else if( rxbuff[5] >= 0x01 && rxbuff[5] <= 0xF0)
	{
		//成功删除 OK
		return 0;
	}
	else
	{
	}
	
}

//指纹删除的函数封装
unsigned char fingermap_erase_apl(unsigned char fgNum){
	
	unsigned char error_code = 1;
	//屏蔽键盘中断
	Disable_KEYBOARD();
	UART1_RX_flash();
	
	error_code = User_erase(fgNum);
	
	//打开键盘中断
	Enable_KEYBOARD();
	if(!error_code){
		SET_DLED2();
		halWait(50000);
		CLR_DLED2();
		halWait(50000);
		return 0;
	}
	return 1;

}


#include "tasks.h"
#include "../drivers/keyboard.h"
#include "../drivers/uart0.h"
#include "../drivers/fingermap.h"
#include "../drivers/door.h"
#include "../drivers/led.h"
#include "../drivers/beep.h"

//定义全局标志
unsigned char passwdauth = 0; //进行密码验证
unsigned char passwdmodify = 0; //进行密码修改
unsigned char fingermapauth = 0; //进行指纹验证
unsigned char fingermapmodify = 0; //进行指纹修改
unsigned char processcommand = 0; //处理上位机数据
unsigned char fingermapid = 0xff; //当前用户指纹号
unsigned char confirm = 0; //F4确认

unsigned char get_key_count = 0; //用户名和密码键数

//保存当前用户用户名和密码
unsigned char name[4];
unsigned char passwd[6];
unsigned char passwd_confirm[6];

//是否已经验证通过
unsigned char userauthed = 0;

//任务状态复位
void tasks_init(void) {
	passwdauth = 0;
	passwdmodify = 0;
	fingermapauth = 0;
	fingermapmodify = 0;
	processcommand = 0;
	fingermapid = 0xff;
	confirm = 0;
	get_key_count = 0;
	memset(name, 0, sizeof(name));
	memset(passwd, 0, sizeof(passwd));
	memset(passwd_confirm, 0, sizeof(passwd_confirm));
	userauthed = 0;
	DoorClose();
} 

//密码验证
// 密码验证请求：
// BB FF 07 00 CC 00 00 00 00 00 00 00 00 00 00 FF(5-8用户名，9-14密码)
//
static unsigned char UART0_TxBuf1[] = {0xBB,0xFF,0x07,0x00,0xCC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
void password_authentication(void) {

	while (1) {
		halWait(250000);
		halWait(250000);
		
		if (confirm && passwdauth) {

			confirm = 0;
			passwdauth = 0;
			userauthed = 0;
			fingermapid = 0xff;

			unsigned char i, j;
			for (i = 5, j = 1; i <= 14; i++, j++) {
				if (j <= 4) {
					UART0_TxBuf1[i] = GetKeyValue(name[j - 1]);
				} else if (j <= 10) {
					UART0_TxBuf1[i] = GetKeyValue(passwd[j - 5]);
				}
			}

			// 发送
			UART0_write(UART0_TxBuf1, 16);

			get_key_count = 0;
		}
		task_delay(10);
	}
}

//密码修改：
//BB FF 07 00 DB 00 00 00 00 00 00 00 00 00 00 FF（5-8：用户号 ，9-14：新密码）
//
static unsigned char UART0_TxBuf2[]={0xBB,0xFF,0x07,0x00,0xDB,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
void password_modify(void) {

	unsigned char i = 0, flag = 1;

	while (1) {
	
		halWait(250000);
		halWait(250000);
	
		if (confirm && passwdmodify) {
			confirm = 0;
			passwdmodify = 0;

			if(userauthed){
				SET_DLED2();
				//验证两次密码输入相同。
				for(i=0; i < 6; i++){
					if(GetKeyValue(passwd[i]) != GetKeyValue(passwd_confirm[i])){
						flag = 0;
						BEEP(5,20000);
						break;
					}
				}
				//flag=1说明两次密码相同。flag=0,说明两次密码不同。
				if(flag){

					SET_DLED1();
					BEEP(2,2000);

					unsigned char i, j;
					for (i = 5, j = 1; i <= 14; i++, j++) {
						if (j <= 4) {
							UART0_TxBuf2[i] = GetKeyValue(name[j - 1]);
						} else if (j <= 10) {
							UART0_TxBuf2[i] = GetKeyValue(passwd[j - 5]);
						}
					}

					UART0_write(UART0_TxBuf2, 16);

				}else{
					//两次密码不同，进行一些处理。
				}

			}

			get_key_count = 0;
		}
		task_delay(10);
	}

}


//指纹验证
//成功(5为指纹号)
//指纹验证通过，下位机告知上位机。（用于保存记录等）
//BB FF 07 00 DA 08 00 00 00 00 00 00 00 00 00 FF      ( 5表示指纹号)
static unsigned char UART0_TxBuf3[]={0xBB,0xFF,0x07,0x00,0xDA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
//失败
//指纹验证失败，下位机将此事件告知上位机，让上位机处理。
//BB FF 07 00 CF 00 00 00 00 00 00 00 00 00 00 FF
static unsigned char UART0_TxBuf4[]={0xBB,0xFF,0x07,0x00,0xCF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
void fingermap_authentication(void) {

	while (1) {
	
		halWait(250000);
		halWait(250000);
		
		if (confirm && fingermapauth) {
			fingermapauth = 0;
			userauthed = 0;
			fingermapid = 0xff;

			//进行指纹验证。
			if (!fingermap_auth_apl()) {
				//指纹验证OK
				DoorOpen();
				userauthed = 1;
				UART0_TxBuf3[5] = fingermapid;
				UART0_write(UART0_TxBuf3, 16);
			}else{
				//指纹验证Fail
				UART0_write(UART0_TxBuf4, 16);
			}
		}
		task_delay(10);
	}
}

// 指纹添加或修改
//指纹修改，传送指纹号：
//BB FF 07 00 CE 00 23 00 23 09 00 00 FF（5-8为用户名，9为指纹序号）
static unsigned char UART0_TxBuf5[]={0xBB,0xFF,0x07,0x00,0xCE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
void fingermap_modify(void) {

	while (1) {

		halWait(250000);
		halWait(250000);
	
		if (confirm && fingermapmodify) {
			confirm = 0;
			fingermapmodify = 0;
			if (userauthed) {
				if (fingermapid == 0xFF) {

					if (!fingermap_enroll_apl()) { //指纹录入成功
						//封装数据
						UART0_TxBuf5[5] = GetKeyValue(name[0]);
						UART0_TxBuf5[6] = GetKeyValue(name[1]);
						UART0_TxBuf5[7] = GetKeyValue(name[2]);
						UART0_TxBuf5[8] = GetKeyValue(name[3]);

						UART0_TxBuf5[9] = fingermapid;
						//发送
						UART0_write(UART0_TxBuf5, 16);
					} else { //指纹录入失败
					}	

				} else {

					if( !fingermap_erase_apl(fingermapid) )
					{//擦除OK
						if (!fingermap_enroll_apl()) { //指纹修改成功
							//封装数据
							UART0_TxBuf5[5] = GetKeyValue(name[0]);
							UART0_TxBuf5[6] = GetKeyValue(name[1]);
							UART0_TxBuf5[7] = GetKeyValue(name[2]);
							UART0_TxBuf5[8] = GetKeyValue(name[3]);

							UART0_TxBuf5[9] = fingermapid;

							//发送
							BEEP(5,10000);
							UART0_write(UART0_TxBuf5, 16);
						} else { //指纹修改失败
						}	
					}else{//擦除失败
					}

				}
			}else{
				//未通过验证，不能修改指纹
				BEEP(6,100000);
				SET_DLED1();
				SET_DLED2();
				SET_DLED3();
				SET_DLED4();
				SET_DLED5();
			}
		}
		task_delay(10);
	}

}


//处理上位机控制命令
static unsigned char UART0_TxBuf6[]={0xBB,0xFF,0x07,0x00,0xA7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
static unsigned char UART0_TxBuf7[]={0xBB,0xFF,0x07,0x00,0xB7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
void process_commands(void) {
	unsigned char temp_cmd[16];

	while (1) {
	
		halWait(250000);
		halWait(250000);
	
		if (if_receive_a_frame()) {
			Reset_receive_frame();
			// 从串口读取一条命令
			UART0_read(temp_cmd, 16);

			if (temp_cmd[0] != 0xAA || temp_cmd[1] != 0xFF || temp_cmd[2] != 0x07 || temp_cmd[3] != 0x03) {
				return;
			}
			//////////////////////////////////////////////////////////////////////
			//处理该命令
			switch (temp_cmd[4]) {
				// 若为查询门状态
				case 0xA7:
					if (IsDoorOpen()) {
						UART0_TxBuf6[5] = 0x00;
					} else {
						UART0_TxBuf6[5] = 0x01;
					}
					UART0_write(UART0_TxBuf6, 16);
					break;
					//开锁命令
				case 0xB7:
					DoorOpen();
					UART0_write(UART0_TxBuf7, 16);
					break;
					//密码验证结果
				case 0xCD:
					if (temp_cmd[5] == 0x00) {//成功

						userauthed = 1;
						//需要开门
						DoorOpen();
						//if (temp_cmd[6] == 0x00) { //有指纹号
						//	fingermapid = temp_cmd[7];
						//} else {
						//	fingermapid = 0xff;
						//}
						fingermapid = temp_cmd[6];//wlj

					} else { //失败
						SET_DLED5();
						halWait(30000);
						BEEP(3,50000);
						CLR_DLED5();
					}
					break;
					//指纹修改结果
				case 0xCE:
					if (temp_cmd[5] == 0x00) {//成功
					} else { //失败
					}
					break;
					//密码修改结果
				case 0xDB:
					if (temp_cmd[5] == 0x00) {//成功
					} else { //失败
					}
					break;
					//指纹验证通过后，上位机返回用户名
				case 0xDA:
					name[0] = temp_cmd[5];
					name[1] = temp_cmd[6];
					name[2] = temp_cmd[7];
					name[3] = temp_cmd[8];
				default:
					break;
			}
		}
		task_delay(10);
	}
}

#include "tasks.h"
//#include "scheduler.h"
#include "drivers/keyboard.h"
#include "drivers/uart0.h"
//#include "drivers/fingermap.h"
#include "drivers/door.h"
#include "drivers/led.h"
#include "drivers/beep.h"

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

//密码验证
static unsigned char UART0_TxBuf1[] = {0xBB,0xFF,0x07,0x00,0xCC,0x00,0x00,0x00,0x00,0x00,0xFF};
void password_authentication(void) {
	/*	while (1) {
		 */
	if (passwdauth) {
		BEEP(5,250000);
		
		passwdauth = 0;
		
		userauthed = 0;
		fingermapid = 0xff;

		///////////////////////////////////////////////////////////////
		
		short userIDinShort = (name[0]-'0')*1000 + (name[1]-'0')*100 +
								 (name[2]-'0')*10 + (name[3]-'0');
		unsigned char userIDinCmdL = (unsigned char)(userIDinShort & 0xFF);
		unsigned char userIDinCmdH = (unsigned char)(userIDinShort >> 8);
		
		int pswdInInt = (passwd[0]-'0')*100000 + (passwd[1]-'0')*10000 +
								 (passwd[2]-'0')*1000 + (passwd[3]-'0')*100+
									 (passwd[4]-'0')*10+ (passwd[5]-'0');
		unsigned char pH = (unsigned char)((pswdInInt & 0x00FF0000)>>16);
		unsigned char pM = (unsigned char)((pswdInInt & 0x0000FF00)>>8);
		unsigned char pL = (unsigned char)(pswdInInt & 0x000000FF);

		//////////////////////////////////////////////////////////////////
		
		
		// 修改协议
		UART0_TxBuf1[5] = userIDinCmdL;
		UART0_TxBuf1[6] = userIDinCmdH;

		UART0_TxBuf1[7] = pL;
		UART0_TxBuf1[8] = pM;
		UART0_TxBuf1[9] = pH;
		
		// 发送
		
		UART0_write(UART0_TxBuf1, 11);
		
		get_key_count = 0;
	}

	//延时
	//	OSTimeDly(200);
	//	}
}

//密码修改
static unsigned char UART0_TxBuf2[]={0xBB,0xFF,0x07,0x00,0xDB,0x00,0x00,0x00,0x00,0x00,0xFF};
void password_modify(void) {
	unsigned char tmp_passwd[3];
	unsigned char i, flag;
	/*
	   while (1) {
	*/
	if (passwdmodify && userauthed) {
		passwdmodify = 0;

		//GetPassword();
		for (i = 0; i < 3; i++) {
			tmp_passwd[i] = passwd[i];
		}
		//GetPassword();
		flag = 1;
		for (i = 0; i < 3; i++) {
			if (tmp_passwd[i] != passwd[i]) {
				flag = 0;
				break;
			}
		}

		if (flag) { //相等
			UART0_TxBuf2[5] = name[0];
			UART0_TxBuf2[6] = name[1];

			UART0_TxBuf2[7] = passwd[0];
			UART0_TxBuf2[8] = passwd[1];
			UART0_TxBuf2[9] = passwd[2];

			UART0_write(UART0_TxBuf2, 11);
		} else { //不相等
		}
	}

	//延时
	//OSTimeDly(200);
	//	}
}

#if 0
//指纹验证
//成功(5为指纹号)
static unsigned char UART0_TxBuf3[]={0xBB,0xFF,0x07,0x00,0xDA,0x00,0x00,0x00,0x00,0x00,0xFF};
//失败
static unsigned char UART0_TxBuf4[]={0xBB,0xFF,0x07,0x00,0xCF,0x00,0x00,0x00,0x00,0x00,0xFF};
void fingermap_authentication(void) {
	unsigned char i;
	unsigned char flag;
	/*
	   while (1) {
	    */
	if (fingermapauth) {
	   fingermapauth = 0;
	
		flag = 1;
		userauthed = 0;
		fingermapid = 0xff;

		for (i = 0; i < 3; i++) {
			if (!user_auth(0xff)) {
				flag = 0;
				break;
			}
		}

		if (flag) { //验证通过
			//开门
			DoorOpen();

			userauthed = 1;
			UART0_TxBuf3[5] = fingermapid;
	
			UART0_write(UART0_TxBuf3, 11);
		} else { //验证失败
			UART0_write(UART0_TxBuf4, 11);
		}

	}
	/*
	//延时
	OSTimeDly(200);
	}
	 */
}

// 指纹添加或修改
static unsigned char UART0_TxBuf5[]={0xBB,0xFF,0x07,0x00,0xCE,0x00,0x00,0x00,0x00,0x00,0xFF};
void fingermap_modify(void) {
	/*
	   while (1) {
		 */
	if (fingermapmodify && userauthed) {
	   fingermapmodify = 0;

		if (fingermapid != 0xff) {
			user_erase(fingermapid);
		}

		if (user_enroll(0xff)) { //指纹修改成功
			//封装数据
			UART0_TxBuf5[5] = name[0];
			UART0_TxBuf5[6] = name[1];
			UART0_TxBuf5[7] = fingermapid;

			//发送
			UART0_write(UART0_TxBuf5, 11);
		} else { //指纹修改失败
		}
	}
/*
	//延时
	OSTimeDly(200);
	}
	 */
}
#endif


//处理上位机控制命令
static unsigned char UART0_TxBuf6[]={0xBB,0xFF,0x07,0x00,0xA7,0x00,0x00,0x00,0x00,0x00,0xFF};
static unsigned char UART0_TxBuf7[]={0xBB,0xFF,0x07,0x00,0xB7,0x00,0x00,0x00,0x00,0x00,0xFF};
void process_commands(void) {
	unsigned char temp_cmd[11];
	/*
	   while (1) {
	   if (processcommand) {
	   processcommand = 0;
	 */

	// 从串口读取一条命令
	UART0_read(temp_cmd, 11);
	
	if (temp_cmd[0] != 0xAA || temp_cmd[1] != 0xFF || temp_cmd[2] != 0x07 || temp_cmd[3] != 0x03) {
		return;
	}
	
	//处理该命令
	switch (temp_cmd[4]) {
		// 若为查询门状态
		case 0xA7:
			if (IsDoorOpen()) {
				UART0_TxBuf6[5] = 0x00;
			} else {
				UART0_TxBuf6[5] = 0x01;
			}
			UART0_write(UART0_TxBuf6, 11);
			break;
			//开锁命令
		case 0xB7:
			DoorOpen();
			UART0_write(UART0_TxBuf7, 11);
			break;
			//密码验证结果
		case 0xCD:
			if (temp_cmd[5] == 0x00) {//成功
				userauthed = 1;
				//需要开门
				DoorOpen();
				if (temp_cmd[6] == 0x00) { //有指纹号
					fingermapid = temp_cmd[7];
				} else {
					fingermapid = 0xff;
				}
			} else { //失败
				SET_DLED4();
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
		default:
			break;
	}
	/*
	   }

	//延时
	OSTimeDly(200);
	}
	 */
}

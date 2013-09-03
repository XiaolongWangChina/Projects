
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//#include <sys/signal.h>
#include "tty.h"
#include"gprs.h"


void gprs_init()
{ 
	int i;
	for(i=0; i<5; i++) {
		tty_writecmd(cmd[i], strlen(cmd[i])); 
		
	}
	//下面选择通道0，使用普通电话话柄
	/*
	  gprs_cmd("AT+CHFA=0\r"); //设置通话通道为0
	  usleep(1000);
	  gprs_cmd("AT+CLVL=100\r");//设置受话器音量最大
	  usleep(1000);
	  gprs_cmd("AT+CMIC=0,10\r");//设置通道0的话筒增益
	  usleep(1000);
	*/
}

void gprs_hold()
{
	tty_writecmd("at", strlen("at"));
	tty_writecmd("ath", strlen("ath"));//发送挂机命令ATH
}

void gprs_ans()
{ 
	tty_writecmd("at", strlen("at"));
	tty_writecmd("ata", strlen("ata"));//发送接听命令ATA
}

//拨叫  
void gprs_call(char *number)			
{ 
    printf("number = %s\n", number);
	tty_write("atd", strlen("atd")); //发送拨打命令ATD	
	tty_write(number, strlen(number));
	tty_write(";\r", strlen(";\r"));
	usleep(200000);
}

//发送短信
void gprs_msg(char *number, char *text)		
{ 
    printf("number = %s\n", number);
    printf("text = %s\n", text);
	char ctl[]={26,0};
	//char text[]="Welcome to use up-tech embedded platform!";
	tty_writecmd("at", strlen("at"));
	usleep(5000);
	tty_writecmd("at", strlen("at"));
	tty_writecmd("at+cmgf=1", strlen("at+cmgf=1"));		//发送修改字符集命令
	tty_write("at+cmgs=", strlen("at+cmgs="));	//发送发短信命令，具体格式见手册
	tty_write("\"", strlen("\""));
	tty_write(number, strlen(number));
    tty_write("\"", strlen("\""));
    tty_write(";\r", strlen(";\r"));
	tty_write(text, strlen(text));
	tty_write(ctl, 1);      //“CTRL+Z"的ASCII码
	usleep(500000);
}

void gprs_baud(char *baud)
{
	tty_write("at+ipr=", strlen("at+ipr="));
	tty_writecmd(baud, strlen(baud) );
	usleep(200000);
}



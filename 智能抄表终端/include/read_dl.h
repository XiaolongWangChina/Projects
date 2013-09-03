#ifndef  _READ_DL_H_
#define _READ_DL_H_
#include "71x_lib.h"
#include "Global_define.h"
#define	THIS_POS_METER_NOT_EXIST			0XFF	//上位机在读多表电量时,给出来的位置,如果不存在如此多的有效表计,则报错
#define	CURRENT_DL_HAVE_NOT_COLLECTED	0XEE	//当前电量如果没有被采到,则在收到上位机读取当前电量的命令时,发送该值给上位机
#define	TYPE_SEND_NAK_UART_PC		0X1
#define	TYPE_SEND_NAK_UART_HW		!TYPE_SEND_NAK_UART_PC

/*******************************************************************************
* Function Name              : Read_Current_485_Dl
* Description                   : 读取当前电量
* inputpara                     :输入缓冲区
* outputpara                     :输出缓冲区
* Return                          : u8
* author                          :shizhide
* Date First Issued           :26/05/2007
*******************************************************************************/
u8 Read_Current_Dl(u8* Process_Buf_Ptr,u8* Send_Buf_Ptr);

#endif

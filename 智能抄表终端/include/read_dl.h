#ifndef  _READ_DL_H_
#define _READ_DL_H_
#include "71x_lib.h"
#include "Global_define.h"
#define	THIS_POS_METER_NOT_EXIST			0XFF	//��λ���ڶ�������ʱ,��������λ��,�����������˶����Ч���,�򱨴�
#define	CURRENT_DL_HAVE_NOT_COLLECTED	0XEE	//��ǰ�������û�б��ɵ�,�����յ���λ����ȡ��ǰ����������ʱ,���͸�ֵ����λ��
#define	TYPE_SEND_NAK_UART_PC		0X1
#define	TYPE_SEND_NAK_UART_HW		!TYPE_SEND_NAK_UART_PC

/*******************************************************************************
* Function Name              : Read_Current_485_Dl
* Description                   : ��ȡ��ǰ����
* inputpara                     :���뻺����
* outputpara                     :���������
* Return                          : u8
* author                          :shizhide
* Date First Issued           :26/05/2007
*******************************************************************************/
u8 Read_Current_Dl(u8* Process_Buf_Ptr,u8* Send_Buf_Ptr);

#endif

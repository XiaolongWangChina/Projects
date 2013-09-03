/***************************************************************************
* File Name          : uart.c
* Description        : This file provides all the UART software functions
*******************************************************************************/

#include "uart.h"
#include "Task_Flag.h"
#include "read_dl.h"
#include "manage_485_asset_addr.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


u8   	Set_Hour,Set_Minute,Set_Second;		//时分秒
u8   	Set_Day,Set_Month;					//月日
u16 	Set_Year;							//年

extern void	Send_Ack_Or_Data_To_Host_Uart_PC();


/***************************************************************************************************
* FunctionName : Parse_Com_Data
* Description  : 解析COM口是否有一个完整的数据帧收到                  
* Parameter(s) :                 
*                Com_Number : 是哪一个COM口 2为下行的485口有；3为上行的PC口
*
* Return       : void
***************************************************************************************************/
void	Parse_Com_Data(u8 Com_Number)
{
	u8* Com_Recv_Buf;			/*指向串口接收缓冲区的指针*/
	u8	Com_Data_Len;	        /*记录数据帧的数据域长度*/ 
	u8*	Com_Recv_Buf_Ptr_W;		/*串口接收缓冲区的写指针*/
	u8*	Com_Recv_Buf_Ptr_R;		/*串口接收缓冲区的读指针*/
	u16 COM_RECV_BUF_SIZE;      /*接收缓冲区的大小*/
	u32	TASK_FLAG_COM_RX_OK;    /*接收到一个完整的帧的标志位*/
	u8*	Com_Process_Buf;        /*如果接收的帧完整则将这一帧数据转存到这个处理缓冲区中为后面处理做准备*/
	u16 i=0;
    
	switch(Com_Number)
	{							
		case	2:
			Com_Recv_Buf 					  = Com_BJ_Recv_Buf;
			Com_Recv_Buf_Ptr_W 				  = &Com_BJ_Recv_Buf_Ptr_W;
			Com_Recv_Buf_Ptr_R 				  = &Com_BJ_Recv_Buf_Ptr_R;
			Com_Process_Buf   				  = Com_BJ_Process_Buf;
			COM_RECV_BUF_SIZE 				  = COM_RECV_BUF_SIZE_BJ;
			TASK_FLAG_COM_RX_OK               = TASK_FLAG_RX_BJ_BIT_OK;
			break;
		case	3:
			Com_Recv_Buf 					  = Com_PC_Recv_Buf;
			Com_Recv_Buf_Ptr_W 				  = &Com_PC_Recv_Buf_Ptr_W;
			Com_Recv_Buf_Ptr_R 				  = &Com_PC_Recv_Buf_Ptr_R;
			Com_Process_Buf   				  = Com_PC_Process_Buf;
			COM_RECV_BUF_SIZE 				  = COM_RECV_BUF_SIZE_HW_PC;
			TASK_FLAG_COM_RX_OK 			  = TASK_FLAG_RX_PC_BIT_OK;
			break;
		default:
			return;
	}
	

     /*去掉数据域缓冲区中的帧最少为12个字节，包括起始符1、地址符6、起始符1、控制码1、数据域长度1、校验码1、结束符1*/
	if( (*Com_Recv_Buf_Ptr_W - *Com_Recv_Buf_Ptr_R + COM_RECV_BUF_SIZE)%COM_RECV_BUF_SIZE < MIN_COM_LENGTH)
	{
		return ;    /*说明接收缓冲区中还没有形成一帧*/   
	}

	/*缓冲区中的数据有可能形成一个有效帧*/
	if(Com_Recv_Buf[*Com_Recv_Buf_Ptr_R] != START_CHAR || Com_Recv_Buf[(*Com_Recv_Buf_Ptr_R+7)%COM_RECV_BUF_SIZE] != START_CHAR)
	{
		//读指针的位置的字符不是起始字符，或者对应的第二个起始字符的位置上不是起始字符
		//则读指针后移，且退出
		*Com_Recv_Buf_Ptr_R = (*Com_Recv_Buf_Ptr_R + 1) % COM_RECV_BUF_SIZE;
		return ;
	}
	//在收到了两个起始符之后,可以先等上几十毫秒. 保证跟在后面的长度字节能够被收到
	Com_Data_Len  = Com_Recv_Buf[(*Com_Recv_Buf_Ptr_R + 9)%COM_RECV_BUF_SIZE] ;	
	if( ( (*Com_Recv_Buf_Ptr_W - *Com_Recv_Buf_Ptr_R + COM_RECV_BUF_SIZE)%COM_RECV_BUF_SIZE)<  (Com_Data_Len +12) )
	{
		return;//总的数据长度还不够，直接退出
	}

	//从长度上来判断，有可能存在一个有效的数据帧
	if(Com_Recv_Buf[((*Com_Recv_Buf_Ptr_R + Com_Data_Len +11) % COM_RECV_BUF_SIZE)] != STOP_CHAR) 
	{
		//终止字符的位置上，不是终止字符，则退出,读指针加1
		*Com_Recv_Buf_Ptr_R = (*Com_Recv_Buf_Ptr_R +1) % COM_RECV_BUF_SIZE;
		return;	
	}

	//前面的两个起始字符后面跟随的停止字符已经收到
	for(i=0;i<Com_Data_Len+12 + 2; i++)
	{
	    Com_Process_Buf[i] = Com_Recv_Buf[ (*Com_Recv_Buf_Ptr_R+i)% COM_RECV_BUF_SIZE];
	}
	//清除缓冲区中所有的数据. 
	memset(Com_Recv_Buf,0,COM_RECV_BUF_SIZE);
	//读写指针清零也可以. 暂时先不清零吧
	*Com_Recv_Buf_Ptr_R = 0;
	*Com_Recv_Buf_Ptr_W = 0;

	Global_Task_Flag |= TASK_FLAG_COM_RX_OK;	//设置收到串口1有效数据帧标志
	return;


}


/*******************************************************************************
* Function Name  : Process_ComX_Data
* Description       : 处理串口PC的数据
* Input                : None
* Return              : None
*******************************************************************************/
void	Process_ComX_Data()			
{
	u8   Ctrl_Code;
	u16 Id_Val;
	u8  temp_i=0,temp_j=0;
	u8  Len_Of_Data;
	u8* ComX_Process_Buf;	/*串口处理缓冲区*/
	u8* ComX_Send_Buf;	/*串口发送缓冲区*/
       
	ComX_Process_Buf 		= Com_PC_Process_Buf;
	ComX_Send_Buf           = Com_PC_Send_Buf;
			
        Len_Of_Data = ComX_Process_Buf[9];
	/*数据域减33h*/ 
	for(temp_i = 0 ;temp_i<Len_Of_Data;temp_i++)	/*数据域中的*/
	{
		ComX_Process_Buf[10+temp_i]  -= 0X33;
	}
	Ctrl_Code = ComX_Process_Buf[8];
	Id_Val = (ComX_Process_Buf[11]<<8) | ComX_Process_Buf[10] ;
	switch(Ctrl_Code)
	{
		case	CTRL_CODE_READ_PARA:		/*读参数*/
			memcpy(ComX_Send_Buf,ComX_Process_Buf,12);	/* 12包括了控制字、长度以及ID0 ID1 */
			switch(Id_Val)
			{
				case  DI_TERMINAL_ASSET_NUMBER:	//读取采集器资产编号命令
					ComX_Send_Buf[9] = 8;	        	// 2 + 6	返回帧的数据的长度
					memcpy(&ComX_Send_Buf[12],Base_ParaMeter.CJQ_485_Addr,6);
					break;
					
				case	DI_METER_485_ADDR_SET:	//读取32块表的485地址集合的命令
					      ComX_Send_Buf[9] = 194;  //  2B + 192B 	返回帧的数据的长度
				             memset(&Meter_485_Addr_Bak[0][0],0x00,192);
		                           memcpy((u8*)&Meter_485_Addr_Bak[0][0],(u8*)&Meter_485_Addr[0],6);
                                        memcpy(&ComX_Send_Buf[12],&Meter_485_Addr_Bak[0][0],192);
                                        break;
					
				case	DI_METER_ASSET_NUMBER_SET:	//读取32块表的资产号集合
					     ComX_Send_Buf[9] = 194;  //  2B + 192B 返回帧的数据的长度
					     memset(&Meter_Asset_Number_Bak[0][0],0x00,192);
		                          memcpy((u8*)&Meter_Asset_Number_Bak[0][0],(u8*)&Meter_Asset_Number[0],6);
                                        memcpy(&ComX_Send_Buf[12],&Meter_Asset_Number_Bak[0][0],192);
					break;
					
				case	DI_DATE:				//读取当前日期命令
					ComX_Send_Buf[9] = 6;		// 2 + 4	返回帧的数据的长度
					RTC_Read_Date_Time(&Time_Rtc); //从RTC 中读出当前日期，并送给上位机
					ComX_Send_Buf[12]  = 0x00;
					ComX_Send_Buf[13] = Time_Rtc.day;
					ComX_Send_Buf[14] = BCD_Inc(Time_Rtc.month);
					ComX_Send_Buf[15] = Time_Rtc.year;				
					break;
					
				case   DI_TIME:							//读取当前时间命令
					ComX_Send_Buf[9] = 5;		// 2 + 3	返回帧的数据的长度
					RTC_Read_Date_Time(&Time_Rtc);//从RTC 中的读出当前时间送给上位机
					ComX_Send_Buf[12] =Time_Rtc.second;
					ComX_Send_Buf[13] =Time_Rtc.minute;
					ComX_Send_Buf[14] =Time_Rtc.hour;
					break;
					
				case	DI_METER_TOTAL:	//抄表的总数
					ComX_Send_Buf[9]    = 3 ;		// 2 + 1 	返回帧的数据的长度				
					ComX_Send_Buf[12]  = Base_ParaMeter.Meter_Total_Number;	//在flash中，表的总数以十进制数的形式存储
					break;
					
				case	DI_CYCLE_SHOW_INTERVAL:	//读取循环显示时间
					ComX_Send_Buf[9]    = 3 ;		// 2 + 1 	返回帧的数据的长度	
					ComX_Send_Buf[12]  = Base_ParaMeter.InterVal_For_Cycle_Show;	//存储为BCD 码的格式
					break;
					
				case	DI_COLLECTION_INTERVAL:	//读取循环采集间隔
					ComX_Send_Buf[9]    = 4 ;		// 2 + 2 	返回帧的数据的长度
					ComX_Send_Buf[12]  = Base_ParaMeter.InterVal_For_Cycle_Collectin&0xff;//在flash中，循环采集间隔以十进制数的形式存储
					ComX_Send_Buf[13]  = (Base_ParaMeter.InterVal_For_Cycle_Collectin>>8)&0xff;	//在flash中，循环采集间隔以十进制数的形式存储
					break;
					
				case	DI_FREEZE_DAY:
					ComX_Send_Buf[9] = 3;
					ComX_Send_Buf[12]  = Base_ParaMeter.DL_Freeze_Day;	//在flash中，冻结日以十进制数的形式存储
					break;	

				default:	
					return;

			}
			ComX_Send_Buf[8] = CTRL_CODE_ACK_READ_PARA;
			Send_Ack_Or_Data_To_Host_Uart_PC();
			break;
		case	CTRL_CODE_READ_DL:			//读电量
			RTC_Read_Date_Time(&Time_Rtc);
			switch(Id_Val)
			{
				case  DI_RD_DLS://抄总电量集合
                                                
					Read_Current_Dl(ComX_Process_Buf,ComX_Send_Buf);
					break;
				default:	//异常情况，报错
				      return;
			}
			memcpy(ComX_Send_Buf,ComX_Process_Buf,9);
			ComX_Send_Buf[10] = ComX_Process_Buf[10];
			ComX_Send_Buf[11] = ComX_Process_Buf[11];
			ComX_Send_Buf[8] = CTRL_CODE_ACK_READ_DL;
			Send_Ack_Or_Data_To_Host_Uart_PC();
			break;

		case	CTRL_CODE_WRITE_PARA:		//写参数
			memcpy(&Base_ParaMeter_Bak,&Base_ParaMeter,sizeof(Base_ParaMeter));
			switch(Id_Val)
			{
				case	DI_TERMINAL_ASSET_NUMBER:
				              memcpy(Base_ParaMeter_Bak.CJQ_485_Addr,&ComX_Process_Buf[16],6);
                                          SPI_Write_161d(BASE_ADDR_BASE_PARA,(u8*)&Base_ParaMeter_Bak,sizeof(Base_ParaMeter_Bak));  //2007-11-30  added by chengzhi
			                     break; 
				case	DI_METER_485_ADDR_SET:
					       Set_Meter_485_Addr(ComX_Process_Buf);
					       break;
				case	DI_METER_ASSET_NUMBER_SET:
					       Set_Meter_Asset_Addr(ComX_Process_Buf);
					       break;
				case	DI_METER_ADD:
					      break;
				case	DI_METER_DELETE:
					       Delete_Meter_485_Asset_Addr(ComX_Process_Buf);
					       break;
				case	DI_METER_REPLACE:
					       break;
				case	DI_DATE:
					TIME=RTC_CounterValue();
					ptim = *(struct tm*)localtime(&TIME);
					//向RTC 的寄存器中写入时间和日期
					ptim.tm_mday  		=   ((ComX_Process_Buf[17]>>4)&0x0F)*10 + (ComX_Process_Buf[17]&0x0F);  //BCD 码转换成10进制数;
					ptim.tm_mon    		=  ((ComX_Process_Buf[18]>>4)&0x0F)*10 + (ComX_Process_Buf[18]&0x0F)-1;
					ptim.tm_year  		=   CURRENT_CENTURY_YEAR + ((ComX_Process_Buf[19]>>4)&0x0F)*10 + (ComX_Process_Buf[19]&0x0F);		
					ptim.tm_year		-= DATE_START_YEAR ;
					TIME = mktime(&ptim);
					
					RTC_WaitForLastTask();
					RTC_EnterCfgMode(); // Enter In Configuration Mode
					RTC_WaitForLastTask(); // Wait For Last Task Completion
					RTC->CNTL = TIME & 0x0000FFFF;
					RTC_WaitForLastTask(); // Wait For Last Task Completion
					RTC->CNTH = ( TIME & 0xFFFF0000 ) >> 16;
					RTC_WaitForLastTask(); // Wait For Last Task Completion
					RTC_ExitCfgMode (); // Exit From Configuration Mode		
					break;
				case	DI_TIME:
				       //向RTC 的寄存器中写入时间和日期
					TIME=RTC_CounterValue();
					ptim = *(struct tm*)localtime(&TIME);
					ptim.tm_sec	 = BCD_2_Dec(ComX_Process_Buf[16]);
					ptim.tm_min    = BCD_2_Dec(ComX_Process_Buf[17]);
					ptim.tm_hour	 = BCD_2_Dec(ComX_Process_Buf[18]);
					
					TIME = mktime(&ptim);
					
					RTC_WaitForLastTask();
					RTC_EnterCfgMode(); // Enter In Configuration Mode
					RTC_WaitForLastTask(); // Wait For Last Task Completion
					RTC->CNTL = TIME & 0x0000FFFF;
					RTC_WaitForLastTask(); // Wait For Last Task Completion
					RTC->CNTH = ( TIME & 0xFFFF0000 ) >> 16;
					RTC_WaitForLastTask(); // Wait For Last Task Completion
					RTC_ExitCfgMode (); // Exit From Configuration Mode		

					break;
				case	DI_COLLECTION_INTERVAL:
					Base_ParaMeter_Bak.InterVal_For_Cycle_Collectin = ComX_Process_Buf[16] +ComX_Process_Buf[17]*256 ;

					//判断采集间隔是否在1－1440 之间，如果不是，报错
					//将BCD 码格式的循环采集间隔转换为10进制的数据
					temp_i  = Base_ParaMeter_Bak.InterVal_For_Cycle_Collectin&0xff;
					temp_j = (Base_ParaMeter_Bak.InterVal_For_Cycle_Collectin>>8)&0xff; 	
					InterVal_For_Cycle_Collectin_Global = (temp_i &0x0F) + ((temp_i>>4) &0x0F)*10 +
										((temp_j&0X0F) +((temp_j>>4)&0X0F)*10)*100 ;	

                                   SPI_Write_161d(BASE_ADDR_BASE_PARA,(u8*)&Base_ParaMeter_Bak,sizeof(Base_ParaMeter_Bak));    //2007-11-30  added by chengzhi

					break;
				case	DI_CYCLE_SHOW_INTERVAL:
					Base_ParaMeter_Bak.InterVal_For_Cycle_Show = ComX_Process_Buf[16] ;
					//将BCD 码格式的循环显示间隔转换为10进制的数据
					temp_i  = Base_ParaMeter_Bak.InterVal_For_Cycle_Show;
					InterVal_For_Cycle_Show_Global = (temp_i&0x0F) + ((temp_i>>4) &0x0F)*10 ;					
                                   SPI_Write_161d(BASE_ADDR_BASE_PARA,(u8*)&Base_ParaMeter_Bak,sizeof(Base_ParaMeter_Bak));    //2007-11-30  added by chengzhi

                                  break;
				case  DI_DATA_CLEAR:
					break;
					
				case	DI_FREEZE_DAY:
					break;	
				default:			
					return;

			}
			memcpy(ComX_Send_Buf,ComX_Process_Buf,8);
			ComX_Send_Buf[8]   = CTRL_CODE_ACK_WRITE_PARA;
			ComX_Send_Buf[9]   =0;	
			Send_Ack_Or_Data_To_Host_Uart_PC();
			memcpy((u8*)&Base_ParaMeter,(u8*)&Base_ParaMeter_Bak,sizeof(Base_ParaMeter));						
			
			break;
            
	}
	return;
	
}

/*******************************************************************************
* Function Name  : UART_Init
* Description    : This function initializes the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Output         : None
* Return         : None
*******************************************************************************/
void UART_Init( )
{


	UART_PC->IER = 0x00;
	UART_PC->CR = 0x00;
	(void)UART_PC->RxBUFR;
	UART_PC->RxRSTR = 0xFFFF;
	UART_PC->TxRSTR = 0xFFFF;

	UART_BJ->IER = 0x00;
	UART_BJ->CR = 0x00;
	(void)UART_BJ->RxBUFR;
	UART_BJ->RxRSTR = 0xFFFF;
	UART_BJ->TxRSTR = 0xFFFF;


	//初始化表计串口  -------UART3 
	GPIO_Config(GPIO0, UART_BJ_Tx_Pin, GPIO_AF_PP);
	GPIO_Config(GPIO0, UART_BJ_Rx_Pin, GPIO_IN_TRI_CMOS);
	UART_OnOffConfig(UART_BJ, ENABLE);						// Turn UARTX on
	UART_FifoReset(UART_BJ, UART_RxFIFO);						// Reset the UART_RxFIFO
	UART_FifoReset(UART_BJ, UART_TxFIFO);						// Reset the UART_TxFIFO
	UART_FifoConfig(UART_BJ,DISABLE);							// Disable FIFOs	
	UART_LoopBackConfig(UART_BJ, DISABLE);					// Disable Loop Back
	/* Configure the UARTX as following:
		- Baudrate = 1200 Bps
		- EVEN parity
		- 8 data bits
		- 1 stop bit */
	UART_Config(UART_BJ, 1200, UART_EVEN_PARITY, UART_1_StopBits, UARTM_8D_P);
	UART_RxConfig(UART_BJ, ENABLE);         						 // Enable Rx
 	UART_ItConfig(UART_BJ,UART_RxBufFull,DISABLE);				 //接收中断使能
	UART_ItConfig(UART_BJ,UART_TxEmpty,DISABLE);				 //发送中断不使能
	EIC_IRQChannelConfig(UART3_IRQChannel,ENABLE);
	EIC_IRQChannelPriorityConfig(UART3_IRQChannel, 15);
    	EIC_IRQConfig(ENABLE);	

	GPIO_Config(GPIO0,UART_BJ_TX_ENABLE_PIN , GPIO_OUT_PP);
	GPIO_WordWrite(GPIO0, GPIO_WordRead(GPIO0)&~UART_BJ_TX_ENABLE_PIN);	//允许接收 	 1 经过光电耦合反向 		


	PCU->BOOTCR &= ~(0X04);
	//初始化串口PC	...       PC机通信  UART0
	GPIO_Config(GPIO0, UART_PC_Tx_Pin, GPIO_AF_PP);
	GPIO_Config(GPIO0, UART_PC_Rx_Pin, GPIO_IN_TRI_CMOS);
	UART_OnOffConfig(UART_PC, ENABLE);						// Turn UARTX on
	UART_FifoReset(UART_PC, UART_RxFIFO);						// Reset the UART_RxFIFO
	UART_FifoReset(UART_PC, UART_TxFIFO);						// Reset the UART_TxFIFO
	UART_FifoConfig(UART_PC,ENABLE);							// Enable FIFOs	
	UART_LoopBackConfig(UART_PC, DISABLE);					// Disable Loop Back
	/* Configure the UARTX as following:
		- Baudrate = 38400 Bps
		- No parity
		- 8 data bits
		- 1 stop bit */
	UART_Config(UART_PC, 38400, UART_EVEN_PARITY, UART_1_StopBits, UARTM_8D_P);
	UART_RxConfig(UART_PC, ENABLE);          									// Enable Rx
	UART_ItConfig(UART_PC,UART_RxHalfFull |UART_TimeOutNotEmpty,ENABLE);		//接收中断使能
	UART_PC->TOR =  15;													//超时设置为1个字节多一点的时间
	EIC_IRQChannelConfig(UART0_IRQChannel,ENABLE);
	EIC_IRQChannelPriorityConfig(UART0_IRQChannel, 2);
    	EIC_IRQConfig(ENABLE);	

	GPIO_Config(GPIO0,UART_PC_TX_ENABLE_PIN , GPIO_OUT_PP);
	PC_RX_ENABLE;	//串口PC 转为接收方式

	GPIO_WordWrite(GPIO_LED_UP, GPIO_WordRead(GPIO_LED_UP) |LED_UP);
	
}

/*******************************************************************************
* Function Name  : UART_BaudRateConfig
* Description    : This function configures the baud rate of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : The baudrate value
* Output         : None
* Return         : None
*******************************************************************************/
void UART_BaudRateConfig(UART_TypeDef *UARTx, u32 BaudRate)
{
  UARTx->BR = (u16)(RCCU_FrequencyValue(RCCU_FCLK)/(16*BaudRate));
}

/*******************************************************************************
* Function Name  : UART_Config
* Description    : This function configures the baudrate, the mode, the data
*                  parity and the number of stop bits of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : The baudrate value
* Input 3        : The parity type
* Input 4        : The number of stop bits
* Input 5        : The UART mode
* Output         : None
* Return         : None
*******************************************************************************/
void UART_Config(UART_TypeDef *UARTx, u32 BaudRate, UARTParity_TypeDef Parity,
                 UARTStopBits_TypeDef StopBits, UARTMode_TypeDef Mode)
{
  UART_ModeConfig(UARTx, Mode);
  UART_BaudRateConfig(UARTx, BaudRate);
  UART_ParityConfig(UARTx, Parity);
  UART_StopBitsConfig(UARTx, StopBits);
}

/*******************************************************************************
* Function Name  : UART_ItConfig
* Description    : This function enables or disables the interrupts of the
*                  selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : The new interrupt flag
* Input 3        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_ItConfig(UART_TypeDef *UARTx, u16 UART_Flag, FunctionalState NewState)
{
  if (NewState==ENABLE) UARTx->IER|=UART_Flag; else UARTx->IER&=~UART_Flag;
}

/*******************************************************************************
* Function Name  : UART_FifoConfig
* Description    : This function enables or disables the Rx and Tx FIFOs of
*                  the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_FifoConfig(UART_TypeDef *UARTx, FunctionalState NewState)
{
  if (NewState==ENABLE) UARTx->CR|=0x0400; else UARTx->CR&=~0x0400;
}

/*******************************************************************************
* Function Name  : UART_FifoReset
* Description    : This function resets the Rx and the Tx FIFOs of the
*                  selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : UART_RxFIFO or UART_TxFIFO
* Output         : None
* Return         : None
*******************************************************************************/
void UART_FifoReset(UART_TypeDef *UARTx, UARTFIFO_TypeDef FIFO)
{
  if (FIFO==UART_RxFIFO) UARTx->RxRSTR=0xFFFF; else UARTx->TxRSTR=0xFFFF;
}

/*******************************************************************************
* Function Name  : UART_LoopBackConfig
* Description    : This function enables or disables the loop back mode of
*                  the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_LoopBackConfig(UART_TypeDef *UARTx, FunctionalState NewState)
{
  if (NewState==ENABLE) UARTx->CR|=0x0040; else UARTx->CR&=~0x0040;
}

/*******************************************************************************
* Function Name  : UART_RxConfig
* Description    : This function enables or disables the UART data reception.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_RxConfig(UART_TypeDef *UARTx, FunctionalState NewState)
{
  if (NewState==ENABLE) UARTx->CR|=0x0100; else UARTx->CR&=~0x0100;
}

/*******************************************************************************
* Function Name  : UART_OnOffConfig
* Description    : This function sets On/Off the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_OnOffConfig(UART_TypeDef *UARTx, FunctionalState NewState)
{
  if (NewState==ENABLE) UARTx->CR|=0x0080; else UARTx->CR&=~0x0080;
}


/*******************************************************************************
* Function Name  : UART_ByteSend
* Description    : This function sends a data byte to the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : A pointer to the data byte to send
* Output         : None
* Return         : None
*******************************************************************************/
void UART_ByteSend(UART_TypeDef *UARTx, u8 *Data)
{
  if (UARTx->CR & (0x0001<<UART_FIFOEnableBit))// if FIFO ENABLED
  {
   	while((UARTx->SR & UART_TxFull)); // while the UART_TxFIFO contain 16 characters.
  }
  else                  // if FIFO DISABLED
  {
  	while (!(UARTx->SR & UART_TxEmpty)); // while the transmit shift register not empty
  }
  UARTx->TxBUFR = *Data;
}

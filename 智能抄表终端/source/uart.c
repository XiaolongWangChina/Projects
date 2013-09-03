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


u8   	Set_Hour,Set_Minute,Set_Second;		//ʱ����
u8   	Set_Day,Set_Month;					//����
u16 	Set_Year;							//��

extern void	Send_Ack_Or_Data_To_Host_Uart_PC();


/***************************************************************************************************
* FunctionName : Parse_Com_Data
* Description  : ����COM���Ƿ���һ������������֡�յ�                  
* Parameter(s) :                 
*                Com_Number : ����һ��COM�� 2Ϊ���е�485���У�3Ϊ���е�PC��
*
* Return       : void
***************************************************************************************************/
void	Parse_Com_Data(u8 Com_Number)
{
	u8* Com_Recv_Buf;			/*ָ�򴮿ڽ��ջ�������ָ��*/
	u8	Com_Data_Len;	        /*��¼����֡�������򳤶�*/ 
	u8*	Com_Recv_Buf_Ptr_W;		/*���ڽ��ջ�������дָ��*/
	u8*	Com_Recv_Buf_Ptr_R;		/*���ڽ��ջ������Ķ�ָ��*/
	u16 COM_RECV_BUF_SIZE;      /*���ջ������Ĵ�С*/
	u32	TASK_FLAG_COM_RX_OK;    /*���յ�һ��������֡�ı�־λ*/
	u8*	Com_Process_Buf;        /*������յ�֡��������һ֡����ת�浽�������������Ϊ���洦����׼��*/
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
	

     /*ȥ�������򻺳����е�֡����Ϊ12���ֽڣ�������ʼ��1����ַ��6����ʼ��1��������1�������򳤶�1��У����1��������1*/
	if( (*Com_Recv_Buf_Ptr_W - *Com_Recv_Buf_Ptr_R + COM_RECV_BUF_SIZE)%COM_RECV_BUF_SIZE < MIN_COM_LENGTH)
	{
		return ;    /*˵�����ջ������л�û���γ�һ֡*/   
	}

	/*�������е������п����γ�һ����Ч֡*/
	if(Com_Recv_Buf[*Com_Recv_Buf_Ptr_R] != START_CHAR || Com_Recv_Buf[(*Com_Recv_Buf_Ptr_R+7)%COM_RECV_BUF_SIZE] != START_CHAR)
	{
		//��ָ���λ�õ��ַ�������ʼ�ַ������߶�Ӧ�ĵڶ�����ʼ�ַ���λ���ϲ�����ʼ�ַ�
		//���ָ����ƣ����˳�
		*Com_Recv_Buf_Ptr_R = (*Com_Recv_Buf_Ptr_R + 1) % COM_RECV_BUF_SIZE;
		return ;
	}
	//���յ���������ʼ��֮��,�����ȵ��ϼ�ʮ����. ��֤���ں���ĳ����ֽ��ܹ����յ�
	Com_Data_Len  = Com_Recv_Buf[(*Com_Recv_Buf_Ptr_R + 9)%COM_RECV_BUF_SIZE] ;	
	if( ( (*Com_Recv_Buf_Ptr_W - *Com_Recv_Buf_Ptr_R + COM_RECV_BUF_SIZE)%COM_RECV_BUF_SIZE)<  (Com_Data_Len +12) )
	{
		return;//�ܵ����ݳ��Ȼ�������ֱ���˳�
	}

	//�ӳ��������жϣ��п��ܴ���һ����Ч������֡
	if(Com_Recv_Buf[((*Com_Recv_Buf_Ptr_R + Com_Data_Len +11) % COM_RECV_BUF_SIZE)] != STOP_CHAR) 
	{
		//��ֹ�ַ���λ���ϣ�������ֹ�ַ������˳�,��ָ���1
		*Com_Recv_Buf_Ptr_R = (*Com_Recv_Buf_Ptr_R +1) % COM_RECV_BUF_SIZE;
		return;	
	}

	//ǰ���������ʼ�ַ���������ֹͣ�ַ��Ѿ��յ�
	for(i=0;i<Com_Data_Len+12 + 2; i++)
	{
	    Com_Process_Buf[i] = Com_Recv_Buf[ (*Com_Recv_Buf_Ptr_R+i)% COM_RECV_BUF_SIZE];
	}
	//��������������е�����. 
	memset(Com_Recv_Buf,0,COM_RECV_BUF_SIZE);
	//��дָ������Ҳ����. ��ʱ�Ȳ������
	*Com_Recv_Buf_Ptr_R = 0;
	*Com_Recv_Buf_Ptr_W = 0;

	Global_Task_Flag |= TASK_FLAG_COM_RX_OK;	//�����յ�����1��Ч����֡��־
	return;


}


/*******************************************************************************
* Function Name  : Process_ComX_Data
* Description       : ������PC������
* Input                : None
* Return              : None
*******************************************************************************/
void	Process_ComX_Data()			
{
	u8   Ctrl_Code;
	u16 Id_Val;
	u8  temp_i=0,temp_j=0;
	u8  Len_Of_Data;
	u8* ComX_Process_Buf;	/*���ڴ�������*/
	u8* ComX_Send_Buf;	/*���ڷ��ͻ�����*/
       
	ComX_Process_Buf 		= Com_PC_Process_Buf;
	ComX_Send_Buf           = Com_PC_Send_Buf;
			
        Len_Of_Data = ComX_Process_Buf[9];
	/*�������33h*/ 
	for(temp_i = 0 ;temp_i<Len_Of_Data;temp_i++)	/*�������е�*/
	{
		ComX_Process_Buf[10+temp_i]  -= 0X33;
	}
	Ctrl_Code = ComX_Process_Buf[8];
	Id_Val = (ComX_Process_Buf[11]<<8) | ComX_Process_Buf[10] ;
	switch(Ctrl_Code)
	{
		case	CTRL_CODE_READ_PARA:		/*������*/
			memcpy(ComX_Send_Buf,ComX_Process_Buf,12);	/* 12�����˿����֡������Լ�ID0 ID1 */
			switch(Id_Val)
			{
				case  DI_TERMINAL_ASSET_NUMBER:	//��ȡ�ɼ����ʲ��������
					ComX_Send_Buf[9] = 8;	        	// 2 + 6	����֡�����ݵĳ���
					memcpy(&ComX_Send_Buf[12],Base_ParaMeter.CJQ_485_Addr,6);
					break;
					
				case	DI_METER_485_ADDR_SET:	//��ȡ32����485��ַ���ϵ�����
					      ComX_Send_Buf[9] = 194;  //  2B + 192B 	����֡�����ݵĳ���
				             memset(&Meter_485_Addr_Bak[0][0],0x00,192);
		                           memcpy((u8*)&Meter_485_Addr_Bak[0][0],(u8*)&Meter_485_Addr[0],6);
                                        memcpy(&ComX_Send_Buf[12],&Meter_485_Addr_Bak[0][0],192);
                                        break;
					
				case	DI_METER_ASSET_NUMBER_SET:	//��ȡ32�����ʲ��ż���
					     ComX_Send_Buf[9] = 194;  //  2B + 192B ����֡�����ݵĳ���
					     memset(&Meter_Asset_Number_Bak[0][0],0x00,192);
		                          memcpy((u8*)&Meter_Asset_Number_Bak[0][0],(u8*)&Meter_Asset_Number[0],6);
                                        memcpy(&ComX_Send_Buf[12],&Meter_Asset_Number_Bak[0][0],192);
					break;
					
				case	DI_DATE:				//��ȡ��ǰ��������
					ComX_Send_Buf[9] = 6;		// 2 + 4	����֡�����ݵĳ���
					RTC_Read_Date_Time(&Time_Rtc); //��RTC �ж�����ǰ���ڣ����͸���λ��
					ComX_Send_Buf[12]  = 0x00;
					ComX_Send_Buf[13] = Time_Rtc.day;
					ComX_Send_Buf[14] = BCD_Inc(Time_Rtc.month);
					ComX_Send_Buf[15] = Time_Rtc.year;				
					break;
					
				case   DI_TIME:							//��ȡ��ǰʱ������
					ComX_Send_Buf[9] = 5;		// 2 + 3	����֡�����ݵĳ���
					RTC_Read_Date_Time(&Time_Rtc);//��RTC �еĶ�����ǰʱ���͸���λ��
					ComX_Send_Buf[12] =Time_Rtc.second;
					ComX_Send_Buf[13] =Time_Rtc.minute;
					ComX_Send_Buf[14] =Time_Rtc.hour;
					break;
					
				case	DI_METER_TOTAL:	//���������
					ComX_Send_Buf[9]    = 3 ;		// 2 + 1 	����֡�����ݵĳ���				
					ComX_Send_Buf[12]  = Base_ParaMeter.Meter_Total_Number;	//��flash�У����������ʮ����������ʽ�洢
					break;
					
				case	DI_CYCLE_SHOW_INTERVAL:	//��ȡѭ����ʾʱ��
					ComX_Send_Buf[9]    = 3 ;		// 2 + 1 	����֡�����ݵĳ���	
					ComX_Send_Buf[12]  = Base_ParaMeter.InterVal_For_Cycle_Show;	//�洢ΪBCD ��ĸ�ʽ
					break;
					
				case	DI_COLLECTION_INTERVAL:	//��ȡѭ���ɼ����
					ComX_Send_Buf[9]    = 4 ;		// 2 + 2 	����֡�����ݵĳ���
					ComX_Send_Buf[12]  = Base_ParaMeter.InterVal_For_Cycle_Collectin&0xff;//��flash�У�ѭ���ɼ������ʮ����������ʽ�洢
					ComX_Send_Buf[13]  = (Base_ParaMeter.InterVal_For_Cycle_Collectin>>8)&0xff;	//��flash�У�ѭ���ɼ������ʮ����������ʽ�洢
					break;
					
				case	DI_FREEZE_DAY:
					ComX_Send_Buf[9] = 3;
					ComX_Send_Buf[12]  = Base_ParaMeter.DL_Freeze_Day;	//��flash�У���������ʮ����������ʽ�洢
					break;	

				default:	
					return;

			}
			ComX_Send_Buf[8] = CTRL_CODE_ACK_READ_PARA;
			Send_Ack_Or_Data_To_Host_Uart_PC();
			break;
		case	CTRL_CODE_READ_DL:			//������
			RTC_Read_Date_Time(&Time_Rtc);
			switch(Id_Val)
			{
				case  DI_RD_DLS://���ܵ�������
                                                
					Read_Current_Dl(ComX_Process_Buf,ComX_Send_Buf);
					break;
				default:	//�쳣���������
				      return;
			}
			memcpy(ComX_Send_Buf,ComX_Process_Buf,9);
			ComX_Send_Buf[10] = ComX_Process_Buf[10];
			ComX_Send_Buf[11] = ComX_Process_Buf[11];
			ComX_Send_Buf[8] = CTRL_CODE_ACK_READ_DL;
			Send_Ack_Or_Data_To_Host_Uart_PC();
			break;

		case	CTRL_CODE_WRITE_PARA:		//д����
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
					//��RTC �ļĴ�����д��ʱ�������
					ptim.tm_mday  		=   ((ComX_Process_Buf[17]>>4)&0x0F)*10 + (ComX_Process_Buf[17]&0x0F);  //BCD ��ת����10������;
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
				       //��RTC �ļĴ�����д��ʱ�������
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

					//�жϲɼ�����Ƿ���1��1440 ֮�䣬������ǣ�����
					//��BCD ���ʽ��ѭ���ɼ����ת��Ϊ10���Ƶ�����
					temp_i  = Base_ParaMeter_Bak.InterVal_For_Cycle_Collectin&0xff;
					temp_j = (Base_ParaMeter_Bak.InterVal_For_Cycle_Collectin>>8)&0xff; 	
					InterVal_For_Cycle_Collectin_Global = (temp_i &0x0F) + ((temp_i>>4) &0x0F)*10 +
										((temp_j&0X0F) +((temp_j>>4)&0X0F)*10)*100 ;	

                                   SPI_Write_161d(BASE_ADDR_BASE_PARA,(u8*)&Base_ParaMeter_Bak,sizeof(Base_ParaMeter_Bak));    //2007-11-30  added by chengzhi

					break;
				case	DI_CYCLE_SHOW_INTERVAL:
					Base_ParaMeter_Bak.InterVal_For_Cycle_Show = ComX_Process_Buf[16] ;
					//��BCD ���ʽ��ѭ����ʾ���ת��Ϊ10���Ƶ�����
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


	//��ʼ����ƴ���  -------UART3 
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
 	UART_ItConfig(UART_BJ,UART_RxBufFull,DISABLE);				 //�����ж�ʹ��
	UART_ItConfig(UART_BJ,UART_TxEmpty,DISABLE);				 //�����жϲ�ʹ��
	EIC_IRQChannelConfig(UART3_IRQChannel,ENABLE);
	EIC_IRQChannelPriorityConfig(UART3_IRQChannel, 15);
    	EIC_IRQConfig(ENABLE);	

	GPIO_Config(GPIO0,UART_BJ_TX_ENABLE_PIN , GPIO_OUT_PP);
	GPIO_WordWrite(GPIO0, GPIO_WordRead(GPIO0)&~UART_BJ_TX_ENABLE_PIN);	//������� 	 1 ���������Ϸ��� 		


	PCU->BOOTCR &= ~(0X04);
	//��ʼ������PC	...       PC��ͨ��  UART0
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
	UART_ItConfig(UART_PC,UART_RxHalfFull |UART_TimeOutNotEmpty,ENABLE);		//�����ж�ʹ��
	UART_PC->TOR =  15;													//��ʱ����Ϊ1���ֽڶ�һ���ʱ��
	EIC_IRQChannelConfig(UART0_IRQChannel,ENABLE);
	EIC_IRQChannelPriorityConfig(UART0_IRQChannel, 2);
    	EIC_IRQConfig(ENABLE);	

	GPIO_Config(GPIO0,UART_PC_TX_ENABLE_PIN , GPIO_OUT_PP);
	PC_RX_ENABLE;	//����PC תΪ���շ�ʽ

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

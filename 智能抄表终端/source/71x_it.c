/*************************************************************************************************
*File Name   : 71x_it.c
*Description : ���ļ����������е��жϴ�����
*Node(s)     : 
*              �����ļ�
**************************************************************************************************/

#include "71x_it.h"
#include "71x_lib.h"
#include "LED.h"


/*******************************************************************************
* Function Name  : Undefined_Handler
* Description    : This function handles Undefined instruction exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Undefined_Handler(void)
{
}

/*******************************************************************************
* Function Name  : FIQ_Handler
* Description    : This function handles FIQ exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FIQ_Handler(void)		
{
}

/*******************************************************************************
* Function Name  : SWI_Handler
* Description    : This function handles SWI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SWI_Handler(void)
{
}

/*******************************************************************************
* Function Name  : Prefetch_Handler
* Description    : This function handles Prefetch Abort exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Prefetch_Handler(void)
{
}

/*******************************************************************************
* Function Name  : Abort_Handler
* Description    : This function handles Data Abort exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Abort_Handler(void)
{
}


/*******************************************************************************
* Function Name  : WDG_IRQHandler
* Description    : This function handles the Watchdog interrupt.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WDG_IRQHandler(void)
{
	WDG_ECFlagClear();
}
/*******************************************************************************
* Function Name  : UART0_IRQHandler
* Description    : This function handles the UART0 global interrupt.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


extern u8  echo_Buff[];
extern u32 echo_Counter ;
void UART0_IRQHandler(void)/* ��PC����ͨѶ */
{
	if (((UART_PC->SR) & (UART_RxHalfFull|UART_TimeOutNotEmpty)))
	{//����0�Ľ��ջ�������������
          echo_Counter = 0;
		while(((UART_PC->SR) & UART_RxBufFull))
			{
				Com_PC_Recv_Buf[Com_PC_Recv_Buf_Ptr_W]= (u8)UART_PC->RxBUFR; // then read the Receive Buffer Register
				
                                echo_Buff[echo_Counter ++ ] = Com_PC_Recv_Buf[Com_PC_Recv_Buf_Ptr_W];
                                 
                                Com_PC_Recv_Buf_Ptr_W++;
				
                                Com_PC_Recv_Buf_Ptr_W%=COM_RECV_BUF_SIZE_HW_PC;
			}
	}	

	if (((UART_PC->SR) & UART_TxHalfEmpty))		
	{				
		//��������
		while(!((UART_PC->SR) & UART_TxFull))
		{
			if(Com_PC_Send_Pos<Com_PC_Send_Total_Len)
			{
			 	UART_PC->TxBUFR = Com_PC_Send_Buf[Com_PC_Send_Pos];
				Com_PC_Send_Pos++;
			}
			else
			{
				break;
			}
		}
	}

	if (((UART_PC->SR) & UART_TxEmpty))		
	{				
		if( Com_Task_Flag&TASK_FLAG_COM_PC_SEND_TIME)
		{	//�������
			Com_Task_Flag |= TASK_FLAG_COM_PC_SEND_COMPLETE;
			//��ֹ�����ж�
			UART_PC->IER &= ~(UART_TxEmpty|UART_TxHalfEmpty);		//�����жϽ�ֹ
			UART_PC->IER |= (UART_RxHalfFull|UART_TimeOutNotEmpty);  //�����ж�ʹ��
			EIC_DISABLE;
	 		PC_RX_ENABLE;	//����PC תΪ���շ�ʽ
			EIC_ENABLE;
			Com_Task_Flag &=~TASK_FLAG_COM_PC_SEND_TIME;
		}
	}

	EIC_IRQPendingBitClear(UART0_IRQChannel);
}

/*******************************************************************************
* Function Name  : UART1_IRQHandler
* Description    : This function handles the UART1 global interrupt.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART1_IRQHandler(void)/* ��������ͨѶ */
{
}

/*******************************************************************************
* Function Name  : UART1_IRQHandler
* Description    : This function handles the UART1 global interrupt.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART3_IRQHandler(void)/* ���ƽ���ͨѶ */
{
	if (((UART_BJ->SR) & UART_RxBufFull))
	{//����BJ   (UART3)   �Ľ��ջ�������������
		while((UART_BJ->SR) & UART_RxBufFull)
		{
			Com_BJ_Recv_Buf[Com_BJ_Recv_Buf_Ptr_W]= (u8)UART_BJ->RxBUFR; // then read the Receive Buffer Register
			Com_BJ_Recv_Buf_Ptr_W++;
			Com_BJ_Recv_Buf_Ptr_W%=COM_RECV_BUF_SIZE_BJ;
		}
	}	
	
	if (((UART_BJ->SR) & UART_TxEmpty))		
	{									
		if(Com_BJ_FE_Number<1)	
		{
			while (!(UART_BJ->SR & UART_TxEmpty)); // while the transmit shift register not empty
  			UART_BJ->TxBUFR = 0xFE;
			Com_BJ_FE_Number++;
		}
		else		//��ʼ��������
		{
			if(Com_BJ_Send_Pos<Com_BJ_Send_Total_Len)
			{
				while (!(UART_BJ->SR & UART_TxEmpty)); // while the transmit shift register not empty
	  			UART_BJ->TxBUFR = Com_BJ_Send_Buf[Com_BJ_Send_Pos];
				Com_BJ_Send_Pos++;
			}
			else
			{	//�������
				Com_Task_Flag |= TASK_FLAG_COM_BJ_SEND_COMPLETE;
				//��ֹ�����жϣ��򿪽����ж�
			 	UART_BJ->IER &= (~UART_TxEmpty);
			 	UART_BJ->IER |= UART_RxBufFull;
				EIC_DISABLE;
				BJ_RX_ENABLE;	//����BJ תΪ����״̬
				EIC_ENABLE;
				Com_Task_Flag&=~TASK_FLAG_COM_BJ_SEND_TIME;
				Com_BJ_Send_Total_Len		= 0;
				Com_BJ_Send_Pos			= 0;					
			}
		}
	}
	EIC_IRQPendingBitClear(UART3_IRQChannel);
}

/*******************************************************************************
* Function Name  : T2TIMI_IRQHandler
* Description    : This function handles Timer2 global interrupt.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void T2TIMI_IRQHandler(void)
{//25 ms ����ú���һ��
	if(TIM2->SR & TIM_OCFA)
	{
	  	TIM_FlagClear(TIM2, TIM_OCFA);
		//��λ������
		//TIM2->CNTR = 0;					
		TIM2->OCAR += TIMER_A_OUTPUT_COMPARE_VALUE;
		Time0_25_ms_Counter++;
		if(Time0_25_ms_Counter>= 40)
		{
			Time0_25_ms_Counter = 0;
			//�ж��Ƿ���Ӧ�òɼ���ʱ��
			LunXun_1_S_Counter++;//�ñ����ڳ�ʼ��ʱ��Ӧ�ñ���ϵͳʱ��ͬ��
									//����������ʱ��ʱҲӦ�ñ�ͬ��
			if(LunXun_1_S_Counter >=(InterVal_For_Cycle_Collectin_Global*LUNXUN_1_S_NUMBERS_FOR_1_MIN + 1))	//��λ: s
			{
				Global_Task_Flag |= TASK_FLAG_BEGIN_LUNXUN;
				LunXun_1_S_Counter = 0;			
			}
		
		}		

		Global_Task_Flag |= TASK_FLAG_25MS_TASK;
	}
}

/*************************************************************************************************
*File Name   : 71x_it.c
*Description : 该文件定义了所有的中断处理函数
*Node(s)     : 
*              创建文件
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
void UART0_IRQHandler(void)/* 与PC进行通讯 */
{
	if (((UART_PC->SR) & (UART_RxHalfFull|UART_TimeOutNotEmpty)))
	{//串口0的接收缓冲区中有数据
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
		//发送数据
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
		{	//发送完毕
			Com_Task_Flag |= TASK_FLAG_COM_PC_SEND_COMPLETE;
			//禁止发送中断
			UART_PC->IER &= ~(UART_TxEmpty|UART_TxHalfEmpty);		//发送中断禁止
			UART_PC->IER |= (UART_RxHalfFull|UART_TimeOutNotEmpty);  //接收中断使能
			EIC_DISABLE;
	 		PC_RX_ENABLE;	//串口PC 转为接收方式
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
void UART1_IRQHandler(void)/* 与红外进行通讯 */
{
}

/*******************************************************************************
* Function Name  : UART1_IRQHandler
* Description    : This function handles the UART1 global interrupt.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART3_IRQHandler(void)/* 与表计进行通讯 */
{
	if (((UART_BJ->SR) & UART_RxBufFull))
	{//串口BJ   (UART3)   的接收缓冲区中有数据
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
		else		//开始发送数据
		{
			if(Com_BJ_Send_Pos<Com_BJ_Send_Total_Len)
			{
				while (!(UART_BJ->SR & UART_TxEmpty)); // while the transmit shift register not empty
	  			UART_BJ->TxBUFR = Com_BJ_Send_Buf[Com_BJ_Send_Pos];
				Com_BJ_Send_Pos++;
			}
			else
			{	//发送完毕
				Com_Task_Flag |= TASK_FLAG_COM_BJ_SEND_COMPLETE;
				//禁止发送中断，打开接收中断
			 	UART_BJ->IER &= (~UART_TxEmpty);
			 	UART_BJ->IER |= UART_RxBufFull;
				EIC_DISABLE;
				BJ_RX_ENABLE;	//串口BJ 转为接收状态
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
{//25 ms 进入该函数一次
	if(TIM2->SR & TIM_OCFA)
	{
	  	TIM_FlagClear(TIM2, TIM_OCFA);
		//复位计数器
		//TIM2->CNTR = 0;					
		TIM2->OCAR += TIMER_A_OUTPUT_COMPARE_VALUE;
		Time0_25_ms_Counter++;
		if(Time0_25_ms_Counter>= 40)
		{
			Time0_25_ms_Counter = 0;
			//判断是否到了应该采集的时间
			LunXun_1_S_Counter++;//该变量在初始化时，应该被与系统时间同步
									//在设置日期时间时也应该被同步
			if(LunXun_1_S_Counter >=(InterVal_For_Cycle_Collectin_Global*LUNXUN_1_S_NUMBERS_FOR_1_MIN + 1))	//单位: s
			{
				Global_Task_Flag |= TASK_FLAG_BEGIN_LUNXUN;
				LunXun_1_S_Counter = 0;			
			}
		
		}		

		Global_Task_Flag |= TASK_FLAG_25MS_TASK;
	}
}

/********************************************************************************
* File Name          : uart.h
* Description        : This file contains all the functions prototypes for the
*                      UART software library.
********************************************************************************
* History:
*  01/01/2004 : V1.2
*  14/07/2004 : V1.3
*******************************************************************************/
#ifndef _UART_H
#define _UART_H

#include "71x_map.h"
#include "71x_lib.h"
#include "Global_define.h"

#include "rccu.h"
#include "gpio.h"
#include "eic.h"

#ifndef _RCCU
  #error _RCCU must be defined
#endif

#ifdef DEBUG
  #ifndef _PCU
    #error _PCU must be defined
  #endif
#endif /* DEBUG */

typedef enum
{
  UART_RxFIFO,
  UART_TxFIFO
} UARTFIFO_TypeDef;

typedef enum
{
  UART_EVEN_PARITY = 0x0000,
  UART_ODD_PARITY  = 0x0020,
  UART_NO_PARITY
} UARTParity_TypeDef;

typedef enum
{
  UART_0_5_StopBits  = 0x00,
  UART_1_StopBits    = 0x08,
  UART_1_5_StopBits  = 0x10,
  UART_2_StopBits    = 0x18
} UARTStopBits_TypeDef;

typedef enum
{
  UARTM_8D   = 0x01,
  UARTM_7D_P = 0x03,
  UARTM_9D   = 0x04,
  UARTM_8D_W = 0x05,
  UARTM_8D_P = 0x07
} UARTMode_TypeDef;


#define DUMMY 0

/*  UART flags definition  */
#define UART_TxFull          0x0200
#define UART_RxHalfFull      0x0100
#define UART_TimeOutIdle     0x0080
#define UART_TimeOutNotEmpty 0x0040
#define UART_OverrunError    0x0020
#define UART_FrameError      0x0010
#define UART_ParityError     0x0008
#define UART_TxHalfEmpty     0x0004
#define UART_TxEmpty         0x0002
#define UART_RxBufFull       0x0001

/*   CR regiter bit definition  */
#define UART_FIFOEnableBit 10
#define UART_RxEnableBit   8
#define UART_RunBit        7
#define UART_LoopBackBit   6
#define UART_ParityOddBit  5
#define UART_StopBits      3

/*   Stop bits definition  */
#define UART_05StopBits     0x00
#define UART_1StopBit       (0x01<<3)
#define UART_15StopBits     (0x02<<3)
#define UART_2StopBits      (0x03<<3)

/*   Modes definition  */
#define UART_8BitsData       0x01
#define UART_7BitsData       0x03
#define UART_9BitsData       0x04
#define UART_8BitsDataWakeUp 0x05
#define UART_8BitsDataParity 0x07

/*   added by chengzhi 2007-04-04  */

#define UART0_Rx_Pin (0x0001<<8)   /*   TQFP 64: pin N?63 , TQFP 144 pin N?143  */
#define UART0_Tx_Pin (0x0001<<9)   /*   TQFP 64: pin N?64 , TQFP 144 pin N?144  */

#define UART1_Rx_Pin (0x0001<<10)  /*   TQFP 64: pin N?1  , TQFP 144 pin N?1  */
#define UART1_Tx_Pin (0x0001<<11)  /*   TQFP 64: pin N?2  , TQFP 144 pin N?3  */

#define UART2_Rx_Pin (0x0001<<13)  /*   TQFP 64: pin N?1  , TQFP 144 pin N?1  */
#define UART2_Tx_Pin (0x0001<<14)  /*   TQFP 64: pin N?2  , TQFP 144 pin N?3  */

#define	UART3_Rx_Pin (0x0001<<1)	/*   p0.1  */
#define	UART3_Tx_Pin (0x0001<<0)	/*   p0.0  */


#define 	UART_PC_Rx_Pin 	UART0_Rx_Pin   	/*   TQFP 64: pin N?63 	*/
#define 	UART_PC_Tx_Pin 	UART0_Tx_Pin 	/*   TQFP 64: pin N?64 	*/

#define 	UART_HW_Rx_Pin 	UART1_Rx_Pin	/*   TQFP 64: pin N?10  	*/
#define 	UART_HW_Tx_Pin	UART1_Tx_Pin  	/*   TQFP 64: pin N?11	*/

#define	UART_BJ_Rx_Pin 	UART3_Rx_Pin	/*   TQFP 64: pin N?1  	*/
#define	UART_BJ_Tx_Pin 	UART3_Tx_Pin		/*   TQFP 64: pin N?0  	*/



/*  PC  , 表计所对应的控制端口中的引脚*/
#define	UART_BJ_TX_ENABLE_PIN 			(1UL<<2)	/*   p0.2  */
#define	UART_PC_TX_ENABLE_PIN			(1UL<<12)	/*   P0.12  */

/*  PC  , 表计以及红外所对应的串口*/
#define	UART_PC  	  		UART0
#define	UART_BJ 	    		UART3

/*表计的发送和接收使能*/
#define	BJ_TX_ENABLE		GPIO_BJ_CTRL->PD |= UART_BJ_TX_ENABLE_PIN
#define	BJ_RX_ENABLE		GPIO_BJ_CTRL->PD &= ~UART_BJ_TX_ENABLE_PIN

/*PC       的发送和接收使能*/
#define	PC_TX_ENABLE		GPIO_PC_CTRL->PD |= UART_PC_TX_ENABLE_PIN
#define	PC_RX_ENABLE		GPIO_PC_CTRL->PD &= ~UART_PC_TX_ENABLE_PIN


/*******************************************************************************
* Function Name  : UART_Init
* Description    : This function initializes the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Output         : None
* Return         : None
*******************************************************************************/
void UART_Init();

/***************************************************************************************************
* FunctionName : Parse_Com_Data
* Description  : 解析COM口是否有一个完整的数据帧收到                  
* Parameter(s) :                 
*                Com_Number : 是哪一个COM口 2为下行的485口有；3为上行的PC口
*
* Return       : void
***************************************************************************************************/
void	Parse_Com_Data(u8 Com_Number);

/*******************************************************************************
* Function Name  : Process_ComX_Data
* Description       : 处理串口Pc的数据
* Input                : None
* Return              : None
*******************************************************************************/
//对上位机的应答数据在该函数中写入发送缓冲区，但是校验和的生成以及发送的过程在该函数后面进行
void	Process_ComX_Data();

/*******************************************************************************
* Function Name  : UART_ModeConfig
* Description    : This function configures the mode of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : The UART mode
* Output         : None
* Return         : None
*******************************************************************************/
inline void UART_ModeConfig(UART_TypeDef *UARTx, UARTMode_TypeDef UART_Mode)
{
  UARTx->CR = (UARTx->CR&0xFFF8)|(u16)UART_Mode;
}

/*******************************************************************************
* Function Name  : UART_BaudRateConfig
* Description    : This function configures the baud rate of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : The baudrate value
* Output         : None
* Return         : None
*******************************************************************************/
void UART_BaudRateConfig(UART_TypeDef *UARTx, u32 BaudRate);

/*******************************************************************************
* Function Name  : UART_ParityConfig
* Description    : This function configures the data parity of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : The parity type
* Output         : None
* Return         : None
*******************************************************************************/
inline void UART_ParityConfig(UART_TypeDef *UARTx, UARTParity_TypeDef Parity)
{
  UARTx->CR = (UARTx->CR&0xFFDF)|(u16)Parity;
}

/*******************************************************************************
* Function Name  : UART_StopBitsConfig
* Description    : This function configures the number of stop bits of the
*                  selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : The number of stop bits
* Output         : None
* Return         : None
*******************************************************************************/
inline void UART_StopBitsConfig(UART_TypeDef *UARTx, UARTStopBits_TypeDef StopBits)
{
  UARTx->CR = (UARTx->CR&0xFFE7)|(u16)StopBits;
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
                 UARTStopBits_TypeDef StopBits, UARTMode_TypeDef Mode);

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
void UART_ItConfig(UART_TypeDef *UARTx, u16 UART_Flag, FunctionalState NewState);

/*******************************************************************************
* Function Name  : UART_FifoConfig
* Description    : This function enables or disables the Rx and Tx FIFOs of
*                  the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_FifoConfig(UART_TypeDef *UARTx, FunctionalState NewState);

/*******************************************************************************
* Function Name  : UART_FifoReset
* Description    : This function resets the Rx and the Tx FIFOs of the
*                  selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : RxFIFO or TxFIFO
* Output         : None
* Return         : None
*******************************************************************************/
void UART_FifoReset(UART_TypeDef *UARTx, UARTFIFO_TypeDef FIFO);

/*******************************************************************************
* Function Name  : UART_LoopBackConfig
* Description    : This function enables or disables the loop back mode of
*                  the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_LoopBackConfig(UART_TypeDef *UARTx, FunctionalState NewState);

/*******************************************************************************
* Function Name  : UART_TimeOutPeriodConfig
* Description    : This function configure the Time Out Period.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : The time-out period value
* Output         : None
* Return         : None
*******************************************************************************/
inline void UART_TimeOutPeriodConfig(UART_TypeDef *UARTx, u16 TimeOutPeriod)
{
  UARTx->TOR = TimeOutPeriod;
}

/*******************************************************************************
* Function Name  : UART_GuardTimeConfig
* Description    : This function configure the Guard Time.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : The guard time value
* Output         : None
* Return         : None
*******************************************************************************/
inline void UART_GuardTimeConfig(UART_TypeDef *UARTx, u16 GuardTime)
{
  UARTx->GTR = GuardTime;
}

/*******************************************************************************
* Function Name  : UART_RxConfig
* Description    : This function enable and disable the UART data reception.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_RxConfig(UART_TypeDef *UARTx, FunctionalState NewState);

/*******************************************************************************
* Function Name  : UART_OnOffConfig
* Description    : This function sets On/Off the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_OnOffConfig(UART_TypeDef *UARTx, FunctionalState NewState);

/*******************************************************************************
* Function Name  : UART_ByteSend
* Description    : This function sends a data byte to the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : A pointer to the data byte to send
* Output         : None
* Return         : None
*******************************************************************************/
void UART_ByteSend(UART_TypeDef *UARTx, u8 *Data);

/*******************************************************************************
* Function Name  : UART_FlagStatus
* Description    : This function gets the flags status of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Output         : None
* Return         : None
*******************************************************************************/
inline u16 UART_FlagStatus(UART_TypeDef *UARTx)
{
  return UARTx->SR;
}

#endif /* _UART_H */

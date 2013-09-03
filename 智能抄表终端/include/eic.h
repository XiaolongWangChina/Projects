/*************************************************************************************************
*File Name   : Eic.h
*Description : 增强型中断控制器的驱动头文件
*Node(s)     : 中断控制器的最底层的驱动文件 
*              创建文件
**************************************************************************************************/

#ifndef __EIC_H
#define __EIC_H

#include "71x_map.h"

#define	EIC_ENABLE				EIC->ICR |= 0x0001
#define	EIC_DISABLE			 	EIC->ICR &= ~0x0001

typedef enum
{
  T0TIMI_IRQChannel    = 0,
  FLASH_IRQChannel     = 1,
  RCCU_IRQChannel      = 2,
  RTC_IRQChannel       = 3,
  WDG_IRQChannel       = 4,
  XTI_IRQChannel       = 5,
  USBHP_IRQChannel     = 6,
  I2C0ITERR_IRQChannel = 7,
  I2C1ITERR_IRQChannel = 8,
  UART0_IRQChannel     = 9,
  UART1_IRQChannel     = 10,
  UART2_IRQChannel     = 11,
  UART3_IRQChannel     = 12,
  SPI0_IRQChannel      = 13,
  SPI1_IRQChannel      = 14,
  I2C0_IRQChannel      = 15,
  I2C1_IRQChannel      = 16,
  CAN_IRQChannel       = 17,
  ADC_IRQChannel       = 18,
  T1TIMI_IRQChannel    = 19,
  T2TIMI_IRQChannel    = 20,
  T3TIMI_IRQChannel    = 21,
  HDLC_IRQChannel      = 25,
  USBLP_IRQChannel     = 26,
  T0TOI_IRQChannel     = 29,
  T0OC1_IRQChannel     = 30,
  T0OC2_IRQChannel     = 31
} IRQChannel_TypeDef;

typedef enum
{
  T0TIMI_FIQChannel     = 0x00000001,
  WDG_FIQChannel        = 0x00000002,
  WDGT0TIMI_FIQChannels = 0x00000003
} FIQChannel_TypeDef;

/*******************************************************************************
* Function Name  : EIC_IRQPendingBitClear
* Author               :chengzhi 
 *Date                 :2007-04-10
*******************************************************************************/
inline void EIC_IRQPendingBitClear(IRQChannel_TypeDef channel)
{
	EIC->IPR |= 1<<channel;
}
/*******************************************************************************
* Function Name  : EIC_Init
* Description    : Initialise the EIC using the load PC instruction
*                 (PC = PC +offset)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EIC_Init(void);

/*******************************************************************************
* Function Name  : EIC_IRQConfig
* Description    : Enable or Disable IRQ interrupts
* Input 1        : New status : can be ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
inline void EIC_IRQConfig(FunctionalState NewState)
{
  if (NewState==ENABLE) EIC->ICR |= 0x0001; else EIC->ICR &= ~0x0001;
}

/*******************************************************************************
* Function Name  : EIC_IRQChannelConfig
* Description    : Configure the IRQ Channel
* Input 1        : IRQ channel name
* Input 2        : Channel New status : can be ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
inline void EIC_IRQChannelConfig(IRQChannel_TypeDef IRQChannel, FunctionalState NewState)
{
  if (NewState==ENABLE) EIC->IER |= 0x0001 << IRQChannel;
  else EIC->IER &= ~(0x0001 << IRQChannel);
}

/*******************************************************************************
* Function Name  : EIC_IRQChannelPriorityConfig
* Description    : Configure the selected IRQ channel priority
* Input 1        : IRQ channel name
* Input 2        : IRQ channel priority
* Output         : None
* Return         : None
*******************************************************************************/
inline void EIC_IRQChannelPriorityConfig(IRQChannel_TypeDef IRQChannel, u8 Priority)
{
  EIC->SIR[IRQChannel] = (EIC->SIR[IRQChannel]&0xFFFF0000) | (u16)Priority & 0x000F;
}


#endif /* __EIC_H */


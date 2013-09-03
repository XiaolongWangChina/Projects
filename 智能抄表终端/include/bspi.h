/*************************************************************************************************
*File Name   : Bspi.h
*Description : 串口通信spi的驱动头文件
*Node(s)     : BSPI的最底层的驱动文件 
*              创建文件
**************************************************************************************************/

#ifndef __BSPI_H
#define __BSPI_H

#include "71x_map.h"


typedef enum {
  BSPI_BERR = 0x004,
  BSPI_RFNE = 0x008,
  BSPI_RFF  = 0x010,
  BSPI_ROFL = 0x020,
  BSPI_TFE  = 0x040,
  BSPI_TUFL = 0x080,
  BSPI_TFF  = 0x100,
  BSPI_TFNE = 0x200
} BSPI_Flags;


#define BSPI_PESET_Mask 0x01
#define BSPI_DFIFO_Mask 0xFFFE
#define BSPI_CPHA_Mask  0x200
#define BSPI_CPOL_Mask  0x100
#define BSPI_BSPI0_Base 0xA000
#define BSPI_BSPI1_Base 0xB000


/*******************************************************************************
* Function Name  : BSPI_BSPI0Conf
* Description    : configure STR71x on BSPI0 mode.
* Input 2        : NewState: specifies the status of the BSPI
*		 : NewState value must be either ENABLE or DISABLE:
*		 : ENABLE: the specified BSPI0 peripheral will be enabled.
*		 : DISABLE: the specified BSPI0 peripheral will be disabled..
* Output         : SPI0 EN  bit  in BOOTCR.
* Return         : None.
*******************************************************************************/
inline void BSPI_BSPI0Conf(FunctionalState NewState)
{
  if (NewState == ENABLE) PCU->BOOTCR |= 0x04; else PCU->BOOTCR &= ~0x04;
}

/*******************************************************************************
* Function Name  : BSPI_Init
* Description    : Initializes BSPI peripheral control and registers to their default reset values.
* Input          : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Output         : BSPI peripheral registers and BSPI global variables are initialized.
* Return         : None.
*******************************************************************************/
void BSPI_Init( BSPI_TypeDef *BSPIx);

/*******************************************************************************
* Function Name  : BSPI_Enable
* Description    : Enables/disables the specified BSPI peripheral.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral to be enabled or disabled.
* Input 2        : NewState: specifies the status of the BSPI
*		 : NewState value must be either ENABLE or DISABLE:
*		 : ENABLE: the specified BSPI peripheral will be enabled.
*		 : DISABLE: the specified BSPI peripheral will be disabled.
* Output         : BSPE bit  in BSPCSR1.
* Return         : None.
*******************************************************************************/
inline void BSPI_Enable( BSPI_TypeDef *BSPIx, FunctionalState NewState)
{
  if (NewState == ENABLE) BSPIx->CSR1 |= BSPI_PESET_Mask;
    else BSPIx->CSR1 &= ~BSPI_PESET_Mask;
}

/*******************************************************************************
* Function Name  : BSPI_MasterEnable
* Description    : Configures the BSPI as a Master or a Slave.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : NewState: specifies whether configuring BSPI as master is enabled or disabled.
*		 : ENABLE: the specified BSPI will be configured as a master.
*		 : DISABLE: the specified BSPI will be configured as a slave.
* Output         : MSTR bit in BSPCSR1 is modified register.
* Return         : None.
*******************************************************************************/
inline void BSPI_MasterEnable(BSPI_TypeDef *BSPIx, FunctionalState NewState)
{
  if (NewState == ENABLE) BSPIx->CSR1 |= 0x02; else BSPIx->CSR1 &= ~0x02;
}


/*******************************************************************************
* Function Name  : BSPI_8bLEn
* Description    : Sets the word length of the receive FIFO and transmit data registers to either 8 or 16 bits.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI.
* Input 2        : status: specifies if the word length is 8 or 16 bits.
*                : status value must be either ENABLE or DISABLE:
*                : ENABLE: to enable setting the word length to 8 bits.
*                : DISABLE: disables setting the word length to 8 bits: the wordlength will be configured to 16 bits.
* Output         : WL bits in BSPCSR1 register.
* Return         : None.
*******************************************************************************/
inline void BSPI_8bLEn(BSPI_TypeDef *BSPIx, FunctionalState NewState)
{
  if (NewState == DISABLE) BSPIx->CSR1 |= 0x0400; else BSPIx->CSR1 &= ~0x0C00;
}

/*******************************************************************************
* Function Name  : BSPI_ClkFEdge
* Description    : Enables capturing the first data sample on the first edge of SCK or on the second edge.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : NewState: specifies whether capturing the first data sample on the first edge of SCK is enabled or disabled.
*		 : status value must be either ENABLE or DISABLE.
* 		 :DISABLE: to enable capturing the first data sample on the first edge of SCK.
* 		 : ENABLE: to enable capturing the first data sample on the second edge of SCK.
* Output         : CPHA bits in BSPCSR1 register.
* Return         : None.
*******************************************************************************/
inline void BSPI_ClkFEdge(BSPI_TypeDef *BSPIx, FunctionalState NewState)
{
  if (NewState == ENABLE) BSPIx->CSR1 |= BSPI_CPHA_Mask;
  else BSPIx->CSR1&=~BSPI_CPHA_Mask;
}

/*******************************************************************************
* Function Name  : BSPI_ClkActiveHigh
* Description    : Configures the clock to be active high or low.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : NewState: specifies whether the clock is active high or low.
*		 : status value must be ENABLE or DISABLE.
*		 : ENABLE: configures the clock to be active high.
*		 : DISABLE: confgures the clock to be active low.
* Output         : CPOL bits in BSPCSR1 register.
* Return         : None.
*******************************************************************************/
inline void BSPI_ClkActiveHigh(BSPI_TypeDef *BSPIx, FunctionalState NewState)
{
  if (NewState == ENABLE) BSPIx->CSR1 |= BSPI_CPOL_Mask;
  else BSPIx->CSR1 &= ~BSPI_CPOL_Mask;
}

/*******************************************************************************
* Function Name  : BSPI_FifoDisable
* Description    : Configure the BSPI FIFO.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Output         : DFIFO bit in BSPCSR2 register.
* Return         : None.
*******************************************************************************/
inline void BSPI_FifoDisable(BSPI_TypeDef *BSPIx)
{
  BSPIx->CSR2 |= 0x0001;
}

/*******************************************************************************
* Function Name  : BSPI_ClockDividerConfig
* Description    : Configure BSPI clock divider
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI.
* Input 2        : Div: holds the value of the clock divider.
* Output         : Div [7:0] bits in BSPCLK register.
* Return         : None.
*******************************************************************************/
inline void BSPI_ClockDividerConfig(BSPI_TypeDef *BSPIx, u8 Div)
{
  BSPIx->CLK = Div;
}


#endif /* __BSPI_H */


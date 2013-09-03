/*************************************************************************************************
*File Name   : 71x_lib.c
*Description : 所有要用到的底层驱动库文件
*Node(s)     : 这个文件是和71x_conf文件配合使用的
*              创建文件
**************************************************************************************************/

#define EXT

#include "71x_map.h"

#ifdef DEBUG

extern u32 T0TIMI_Addr;

/*******************************************************************************
* Function Name  : debug
* Description    : Initialize the pointers to peripherals
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void debug(void)
{

  #ifdef _BSPI
  #ifdef _BSPI0
  BSPI0 = (BSPI_TypeDef *)BSPI0_BASE;
  #endif
  #ifdef _BSPI1
  BSPI1 = (BSPI_TypeDef *)BSPI1_BASE;
  #endif
  #endif

  #ifdef _EIC
  EIC = (EIC_TypeDef *)EIC_BASE;
  #endif

  #ifdef _FLASH
  FLASHR = (FLASHR_TypeDef *)FLASHR_BASE;
  FLASHPR = (FLASHPR_TypeDef *)FLASHPR_BASE;
  #endif

  #ifdef _GPIO
  #ifdef _GPIO0
  GPIO0 = (GPIO_TypeDef *)GPIO0_BASE;
  #endif
  #ifdef _GPIO1
  GPIO1 = (GPIO_TypeDef *)GPIO1_BASE;
  #endif
  #ifdef _GPIO2
  GPIO2 = (GPIO_TypeDef *)GPIO2_BASE;
  #endif
  #endif

  #ifdef _PCU
  PCU = (PCU_TypeDef *)PCU_BASE;
  #endif

  #ifdef _RCCU
  RCCU = (RCCU_TypeDef *)RCCU_BASE;
  #endif

  #ifdef _RTC
  RTC = (RTC_TypeDef *)RTC_BASE;
  #endif

  #ifdef _TIM
  #ifdef _TIM0
  TIM0 = (TIM_TypeDef *)TIM0_BASE;
  #endif
  #ifdef _TIM1
  TIM1 = (TIM_TypeDef *)TIM1_BASE;
  #endif
  #ifdef _TIM2
  TIM2 = (TIM_TypeDef *)TIM2_BASE;
  #endif
  #ifdef _TIM3
  TIM3 = (TIM_TypeDef *)TIM3_BASE;
  #endif
  #endif

  #ifdef _UART
  #ifdef _UART0
  UART0 = (UART_TypeDef *)UART0_BASE;
  #endif
  #ifdef _UART1
  UART1 = (UART_TypeDef *)UART1_BASE;
  #endif
  #ifdef _UART2
  UART2 = (UART_TypeDef *)UART2_BASE;
  #endif
  #ifdef _UART3
  UART3 = (UART_TypeDef *)UART3_BASE;
  #endif
  #endif
  
  #ifdef _WDG
  WDG = (WDG_TypeDef *)WDG_BASE;
  #endif

  #ifdef _IRQVectors
  IRQVectors = (IRQVectors_TypeDef *)&T0TIMI_Addr;
  #endif
}

#endif  /* DEBUG */


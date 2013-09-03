/*********************************************************************************
* File Name          : wdg.h
* Description        : This file contains all the functions prototypes for the
*                      WDG software library.
*******************************************************************************/
#ifndef __WDG_H
#define __WDG_H

#include "71x_map.h"
#include "rccu.h"


#define	WDG_PRESCALER_VALUE	199		//将看门狗预分频值设为200
#define	WDG_RELOAD_VALUE		5000	//将看门狗重载计数值设为5000
					//WATCH DOG 的时钟频率为10M (APB2的总线频率)
					//10M = 200 * 5000
					//200*50000/8 = 1.25M us = 1.25S 
					//200*5000/8   = 0.125M us = 0.125S 
					//200*25000/8   = 0.625M us = 0.625S 

/*******************************************************************************
* Function Name  : WDG_Enable
* Description    : Enable the Watchdog Mode
* Input          : None
* Return         : None
*******************************************************************************/
inline void WDG_Enable ( void )
{
  WDG->CR |= 0x01;
}
/*******************************************************************************
* Function Name  : WDG_CntRefresh
* Description    : Refresh and update the WDG counter to avoid a system reset.
* Input          : None
* Return         : None
*******************************************************************************/
inline void WDG_CntRefresh ( void )
{
  /* Write the first value in the key register */
  WDG->KR = 0xA55A;

  /* Write the second value in the key register */
  WDG->KR = 0x5AA5;

}
/*******************************************************************************
* Function Name  : WDG_PrescalerConfig
* Description    : Set the counter prescaler value.
*                  The clock to Timer Counter is divided by (Prescaler + 1)
* Input          : Prescaler data value (8 bit)
* Return         : None
*******************************************************************************/
inline void WDG_PrescalerConfig ( u8 Prescaler )
{
  WDG->PR = Prescaler;
}
/*******************************************************************************
* Function Name  : WDG_CntReloadUpdate
* Description    : Update the counter pre-load value.
* Input          : Pre-load data value (16 bit)
* Return         : None
*******************************************************************************/
inline void WDG_CntReloadUpdate ( u16 PreLoadValue )
{
  WDG->VR = PreLoadValue;
}
/*******************************************************************************
* Function Name  : WDG_ECFlagClear
* Description    : Clear the end of count flag.
* Input          : None
* Return         : None
*******************************************************************************/
inline void WDG_ECFlagClear ( void )
{
  WDG->SR = 0x0000;
}
/*******************************************************************************
* Function Name  	: WDG_Init()
* Description    	: Init WatchDog
* Input          		: None
* Return         	: None
*******************************************************************************/
void		WDG_Init();

#endif /* __WDG_H */

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/

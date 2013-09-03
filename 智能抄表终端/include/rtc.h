#ifndef __RTC_H
#define __RTC_H

#include "71x_lib.h"

typedef enum
{
  RTC_GIR  = 0x0008,
  RTC_OWIR = 0x0004,
  RTC_AIR  = 0x0002,
  RTC_SIR  = 0x0001
} RTC_FLAGS;

typedef enum
{
  RTC_GIT  = 0x0008,
  RTC_OWIT = 0x0004,
  RTC_AIT  = 0x0002,
  RTC_SIT  = 0x0001
} RTC_IT;
/*******************************************************************************
* Function Name  : RTC_CounterValue
* Description    : This routine is used to get the RTC counter value
* Input          : None
* Return         : The current counter value.
*******************************************************************************/
inline u32 RTC_CounterValue (void)
{
	return ( (u32)RTC->CNTH << 16 ) | RTC->CNTL;
}
/*******************************************************************************
* Function Name  : RTC_PrescalerValue
* Description    : This routine is used to get the RTC prescaler Value
* Input          : None
* Return         : an u32 value that holds the prescaler Value.
*******************************************************************************/
inline u32 RTC_PrescalerValue (void)
{
	return ( (u32)(RTC->PRLH & 0x000F) << 16 ) | RTC->PRLL;
}
/******************************************************************************
* Function Name  : RTC_PrescalerConfig
* Description    : This routine is used to set the Prescaler Value
* Input          : The New prescaler Value
* Return         : None
*******************************************************************************/
void RTC_PrescalerConfig (u32 Xprescaler);
/*******************************************************************************
* Function Name  : RTC_FlagClear
* Description    : This routine is used to clear the RTC flags
* Input          : an RTC flag RTC_GIR, RTC_OWIR, RTC_AIR or RTC_SIR
* Return         : None
*******************************************************************************/
void RTC_FlagClear (RTC_FLAGS Xflag);
/*******************************************************************************
* Function Name  : RTC_EnterCfgMode
* Description    : This routine is used to enter in the Configuration Mode
* Input          : None
* Return         : None
*******************************************************************************/
void RTC_EnterCfgMode(void);
/*******************************************************************************
* Function Name  : RTC_ExitCfgMode
* Description    : This routine is used to exit from the Configuration Mode
* Input          : None
* Return         : None
*******************************************************************************/
void RTC_ExitCfgMode(void);
/*******************************************************************************
* Function Name  : RTC_WaitForLastTask
* Description    : This routine waits for the last task completetion
* Input          : None
* Return         : None
*******************************************************************************/
void RTC_WaitForLastTask(void);
/*******************************************************************************
* Function Name  : RTC_Read_Date
* Description    : 读取RTC 当前的日期
* Output Para         : u8* Date   出参数,得到当前的日月年
* Return         : None
*******************************************************************************/
void		RTC_Read_Date_Time(RTC_TIME	*Date);
/*******************************************************************************
* Function Name  	: RTC_Init
* Description    	: 读取RTC 当前的日期
* Output Para        : None
* Return         	: None
*******************************************************************************/
void		RTC_Init();

#endif /* __RTC_H */


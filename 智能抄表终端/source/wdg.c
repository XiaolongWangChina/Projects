/********************************************************************************
* File Name          : WDG.c
* Description        : This file provides all the WDG software functions
*******************************************************************************/

#include "wdg.h"

#ifndef abs
	#define abs(x) ((x)>0 ? (x) : -(x))
#endif

/*******************************************************************************
* Function Name  	: WDG_Init()
* Description    	: Init WatchDog
* Input          		: None
* Return         	: None
*******************************************************************************/
void		WDG_Init()
{
	WDG_PrescalerConfig(WDG_PRESCALER_VALUE);		//设置分频因子值		10 M = 200 * 50000
	WDG_CntReloadUpdate(WDG_RELOAD_VALUE);		//设置计数器的值		10 M = 200 * 50000

	WDG_Enable();
	WDG_CntRefresh();				//刷新计数器值
}

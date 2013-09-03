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
	WDG_PrescalerConfig(WDG_PRESCALER_VALUE);		//���÷�Ƶ����ֵ		10 M = 200 * 50000
	WDG_CntReloadUpdate(WDG_RELOAD_VALUE);		//���ü�������ֵ		10 M = 200 * 50000

	WDG_Enable();
	WDG_CntRefresh();				//ˢ�¼�����ֵ
}

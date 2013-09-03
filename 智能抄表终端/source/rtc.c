#include "rtc.h"

/* Second interrupt */
#define RTC_SI_Mask   	0x0001

/* Alarm interrupt */
#define RTC_AI_Mask   	0x0002

/* OverFlow interrupt */
#define RTC_OWI_Mask  	0x0004

/* Global interrupt */
#define RTC_GI_Mask   	0x0008

/* Configuration Flag Mask */
#define RTC_CNF_Mask  	0x0010

/* Operation OFF flag */
#define RTC_RTOFF_Mask	0x0020

/*******************************************************************************
* Function Name  : RTC_PrescalerConfig
* Description    : This routine is used to set the RTC 20-bit Prescaler Value
* Input          : The New prescaler Value
* Return         : None
*******************************************************************************/
void RTC_PrescalerConfig (u32 Xprescaler)
{
  if ( RTC_PrescalerValue () != Xprescaler )
  {
    /* Wait For Last Task Completion */
    RTC_WaitForLastTask ();

    /* Enter In Configuration Mode */
    RTC_EnterCfgMode ();

    /* Set the prescaler MSB  part */
    RTC->PRLH = (Xprescaler & 0x000F0000) >> 16;

    /* Wait For Last Task Completion */
    RTC_WaitForLastTask();

    /* Set the prescaler LSB  part */
    RTC->PRLL = (Xprescaler & 0x0000FFFF);

    /* Wait For Last Task Completion */
    RTC_WaitForLastTask();

    /* Exit From Configuration Mode */
    RTC_ExitCfgMode ();
  }
}

/*******************************************************************************
* Function Name  : RTC_FlagClear
* Description    : This routine is used to clear the RTC flags
* Input          : an RTC flag RTC_GIR, RTC_OWIR, RTC_AIR or RTC_SIR
* Return         : None
*******************************************************************************/
void RTC_FlagClear (RTC_FLAGS Xflag)
{
  /* Wait For Last Task Completion */
  RTC_WaitForLastTask();

  /* Enter In Configuration Mode */
  RTC_EnterCfgMode();

  /* Clear an RTC flag */
 RTC->CRL &= ~Xflag;

  /* Wait For Last Task Completion */
  RTC_WaitForLastTask();

  /* Exit From Configuration Mode */
  RTC_ExitCfgMode ();
 }


/*******************************************************************************
* Function Name  : RTC_EnterCfgMode
* Description    : This routine is used to enter in the Configuration Mode
* Input          : None
* Return         : None
*******************************************************************************/
void RTC_EnterCfgMode(void)
{
  /* Set the CNF flag to enter in the Configuration Mode */
  RTC->CRL |= RTC_CNF_Mask;

  /* Wait For Last Task Completion */
  RTC_WaitForLastTask ();
}

/*******************************************************************************
* Function Name  : RTC_ExitCfgMode
* Description    : This routine is used to exit from the Configuration Mode
* Input          : None
* Return         : None
*******************************************************************************/
void RTC_ExitCfgMode(void)
{
  /* Reset the CNF flag to exit from the Configuration Mode */
  RTC->CRL &= ~RTC_CNF_Mask;

  /* Wait For Last Task Completion */
  RTC_WaitForLastTask ();
}

/*******************************************************************************
* Function Name  : RTC_WaitForLastTask
* Description    : This routine waits for the last task completion
* Input          : None
* Return         : None
*******************************************************************************/
void RTC_WaitForLastTask(void)
{
  /* Loop until the Last operation Completion */
  while (!(RTC->CRL & RTC_RTOFF_Mask));
}

/*******************************************************************************
* Function Name  : RTC_Read_Date
* Description    : 读取RTC 当前的日期
* Output Para         : u8* Date   出参数,得到当前的日月年
* Return         : None
*******************************************************************************/
void	RTC_Read_Date_Time(RTC_TIME* Date_Time)
{
	TIME=RTC_CounterValue();
	ptim = *(struct tm * )localtime(&TIME);
	ptim.tm_year		+=DATE_START_YEAR;
	
	Date_Time->day		= (ptim.tm_mday/10)*16 +   ptim.tm_mday%10;
	Date_Time->month	= (ptim.tm_mon/10)*16	+ ptim.tm_mon%10;	//注:tm_mon 范围0--11
 	Date_Time->year	= Dec_2_BCD((ptim.tm_year)%100);
	Date_Time->hour	= (ptim.tm_hour/10)*16 	+ ptim.tm_hour%10;
	Date_Time->minute	= (ptim.tm_min/10)*16 	+ ptim.tm_min%10;
	Date_Time->second	= (ptim.tm_sec/10)*16 	+ ptim.tm_sec%10;

	Year_Mon_Day.Day    = Date_Time->day;
	Year_Mon_Day.Month = BCD_Inc(Date_Time->month) ;	
	Year_Mon_Day.Year   = Date_Time->year;
	
}


/*******************************************************************************
* Function Name  	: RTC_Init
* Description    	: 读取RTC 当前的日期
* Output Para        : None
* Return         	: None
*******************************************************************************/
void	RTC_Init()
{

		RTC_FlagClear ( RTC_OWIR );
		RTC_FlagClear ( RTC_AIR );
		RTC_FlagClear ( RTC_SIR );
		RTC_FlagClear ( RTC_GIR );
		RTC_PrescalerConfig(RTC_PRESC);

		ptim.tm_sec 		= 	0x01;
		ptim.tm_min 		= 	0x01;
		ptim.tm_hour 	= 	0x01;
		ptim.tm_mday 	= 	0x01;
		ptim.tm_mon  	= 	0x00;	//tm 结	构体中 month 的范围是  0--11
		ptim.tm_year 	= 	2006;	//
		ptim.tm_year		-=	DATE_START_YEAR;
		
		TIME = mktime(&ptim);
		RTC_WaitForLastTask();
		RTC_EnterCfgMode(); // Enter In Configuration Mode
		RTC_WaitForLastTask(); // Wait For Last Task Completion
		RTC->CNTL = TIME & 0x0000FFFF;
		RTC_WaitForLastTask(); // Wait For Last Task Completion
		RTC->CNTH = ( TIME & 0xFFFF0000 ) >> 16;
		RTC_WaitForLastTask(); // Wait For Last Task Completion
		RTC_ExitCfgMode (); // Exit From Configuration Mode
	
}
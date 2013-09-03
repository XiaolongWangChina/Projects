#include "71x_lib.h" 
#include "LunXun.h"

/****************************************************************************
函数名称:   LunXun_Start()
功能描述:   开始轮循
****************************************************************************/
void	LunXun_Start()
{
	//首先判断有效表计的总数，如果等于0， 则退出轮循
	if(Base_ParaMeter.Meter_Total_Number <=0)
	{
		Global_Task_Flag &= ~TASK_FLAG_LUNXUN_ING;	//清除正在轮循标志
		return;
	}
	//设置允许轮循标志
	Global_Task_Flag |= (TASK_FLAG_LUNXUN_PERMIT|TASK_FLAG_LUNXUN_ING);

	GPIO_WordWrite(GPIO_LED_WARNING,GPIO_WordRead(GPIO_LED_WARNING) |(LED_WARNNING));//灭掉报报警灯
	LunXun_One_Meter(); 					 
}
/****************************************************************************
函数名称:   LunXun_One_Meter()
功能描述:   开始轮循某一块电表
****************************************************************************/
void	LunXun_One_Meter()
{
	u8	temp_i;
	//首先判断有没有轮循允许标志，
	if(!(Global_Task_Flag&TASK_FLAG_LUNXUN_PERMIT))
	{	//如果没有轮循允许标志的话，则退出 
		Global_Task_Flag &= ~(TASK_FLAG_LUNXUN_ING|TASK_FLAG_LUNXUN_COMMAND_SEND_COMPLETE);	//清正在轮循标志
		return;	 
	}
	//如果有轮循允许标志，则把将要发送的数据写入发送缓冲区
	//首先判断该位置上的电表是否存在

      if(Meter_Valid_Flag==METER_IS_VALID)
	{			
		//读取一次当前的时间
		RTC_Read_Date_Time(&Time_Rtc);
		Meter_32_Dl_And_Time.Hour     = Time_Rtc.hour;
		Meter_32_Dl_And_Time.Minute   = Time_Rtc.minute;
		Meter_32_Dl_And_Time.Second  = Time_Rtc.second;
		Meter_32_Date.Day 			= Year_Mon_Day.Day;
		Meter_32_Date.Month 			= Year_Mon_Day.Month;
		Meter_32_Date.Year			= Year_Mon_Day.Year;

              Com_BJ_Send_Buf[0] = START_CHAR;
			  
              ChangeAddrToAA(&Meter_485_Addr[0],&Com_BJ_Send_Buf[1]);
		Com_BJ_Send_Buf[7]   = START_CHAR;
		Com_BJ_Send_Buf[8]   = 0X01;
		Com_BJ_Send_Buf[9]   = 0X02;
		Com_BJ_Send_Buf[10] = 0X52;
		Com_BJ_Send_Buf[11] = 0XC3;
		Com_BJ_Send_Buf[12] = 0;
		for(temp_i = 0;temp_i<12;temp_i++)
		{
			Com_BJ_Send_Buf[12] += Com_BJ_Send_Buf[temp_i];
		}
		Com_BJ_Send_Buf[13] = 0X16;
		Com_BJ_Send_Buf[14] = 0X16;

		//采用中断的方式进行发送
		Com_BJ_FE_Number  =0;
		Com_BJ_Send_Pos    =0;
		Com_BJ_Send_Total_Len = 14;
		BJ_TX_ENABLE;	//串口BJ 转为发送状态
	 	temp_i = 0xFE;
	 	UART_ByteSend(UART_BJ,&temp_i);
 	 	UART_ItConfig(UART_BJ,UART_RxBufFull,DISABLE);		//接收中断禁止
	 	UART_ItConfig(UART_BJ,UART_TxEmpty,ENABLE);		//发送中断使能

		Com_Task_Flag  |= TASK_FLAG_COM_BJ_SEND_TIME;			
		Global_Task_Flag |= TASK_FLAG_LUNXUN_COMMAND_SEND_COMPLETE;
		//点亮下行灯 
		GPIO_WordWrite(GPIO_LED_DOWN, GPIO_WordRead(GPIO_LED_DOWN) &(~LED_DOWN));
		
              Global_Task_Flag &= ~TASK_FLAG_LUNXUN_ING;	//清除正在轮循标志
	}
	else
	{
		//本次轮循结束
		Global_Task_Flag &= ~TASK_FLAG_LUNXUN_ING;

		//清除轮循允许标志
		Global_Task_Flag &= ~TASK_FLAG_LUNXUN_PERMIT;
	}
}
/****************************************************************************
函数名称:   Process_Com_2_Data()
功能描述:   在收到电能表的回答(电量值)后进行处理
****************************************************************************/
void Process_Com_2_Data()
{
	u8 Data_Len;
	u8 i;
	Data_Len = Com_BJ_Process_Buf[9];	
 
	//判断收到的数据的长度是否是22 B   2B(DI1DI0) + 20B(电量值) 
	if(Data_Len<22)
	{	//当收到的电量的长度不是20个字节的时候,
		//则认为是只有前4个字节是真正的电量的数据,后面的16个字节清零(0x33)
		memset(&Com_BJ_Process_Buf[16],0x33,16);//将电量所在的位置上的后面的16个字节清零 
		//将总电量考到平电量的位置(好像是平电量的位置)
		memcpy(&Com_BJ_Process_Buf[24],&Com_BJ_Process_Buf[12],4);
		Data_Len = 22;
	}
	else
	{
		//判断后面的16个字节是否是全零
		for(i=0;i<16;i++)
		{
			if(Com_BJ_Process_Buf[16+i]!=0x33)
			{
				break;
			}
		}
		if(i>=16)
		{//如果是全零,则将总电量考到平电量的位置(好像是平电量的位置)
			memcpy(&Com_BJ_Process_Buf[24],&Com_BJ_Process_Buf[12],4);
		}
	}

	//将采集到的数据存入内存中相应的表计的位置上
        memcpy((u8*)&Meter_32_Dl_And_Time,&Com_BJ_Process_Buf[12],20);
	Global_Task_Flag &= ~TASK_FLAG_LUNXUN_ING;
	Global_Task_Flag &= ~TASK_FLAG_LUNXUN_PERMIT;//清除轮循允许标志	
}
/*********************************************************************************
函数名称:   ChangeAddrToAA()
功能描述:   由于对采集器进行了编程操作.所以停止当前轮循. 且本次轮循的成功失败次数作废
*********************************************************************************/
void	ChangeAddrToAA(u8* Meter_485_Addr,u8* Com_BJ_Send_Buf)
{
    u8 i =6;
    u8 len=0;
    u8 Meter485AddrBak[6]={0};
    
    memcpy(&Meter485AddrBak[0],Meter_485_Addr,6);
    len=(6-strlen((char*)Meter485AddrBak));
    for(i=6;i>0;i--)
    {
         if(len>0)
         {  
                Com_BJ_Send_Buf[i-1]=0xAA;  
		 len--;
	 }
         else
         {
                Com_BJ_Send_Buf[i-1]=Meter_485_Addr[i-1];
	 }
    }
}

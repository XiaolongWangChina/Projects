#ifndef	_EXTERN_VAR_H_
#define	_EXTERN_VAR_H_
#include <time.h>








extern u8  echo_Buff[];
extern u32 echo_Counter;










extern	u8    Time0_25_ms_Counter;       //25ms 计数器
extern 	u32  Global_Task_Flag ;		//全局任务标志
extern	u32  Com_Task_Flag;		//串口所用到的任务标志

/*关于SPI_FLASH	中的存储区域的变量*/
extern 	u8  Meter_485_Addr[6];			//表的485地址
extern 	u8  Meter_Asset_Number[6];		//表的资产编号
extern 	u8  Meter_485_Addr_Bak[32][6];		//32表的485备用地址,处理接收到的数据
extern 	u8  Meter_Asset_Number_Bak[32][6];	//32表的资产编号，处理接收到的数据
extern 	u8  Meter_Valid_Flag;			//表的有效标志
extern	u8  Meter_Valid_Flag_Bak;		//表的有效标志
extern 	BASE_PARAMETER		Base_ParaMeter;		//采集器的一些基本参数
extern 	BASE_PARAMETER		Base_ParaMeter_Bak;	//采集器的一些基本参数
extern 	YEAR_MON_DAY		Year_Mon_Day;		//当前电量的采集日期
extern 	METER_32_DL_And_TIME	Meter_32_Dl_And_Time;	//存放表的当前电量以及电量采集的时间
extern	YEAR_MON_DAY		Meter_32_Date;		//存放表的当前电量的日期

/*循环采集相关的变量*/
extern	u32		LunXun_1_S_Counter;			//当该值到达设定的采集时间时，则设置轮循标志
extern 	u32		InterVal_For_Cycle_Collectin_Global;	//轮循采集间隔
extern	u32		LunXun_Error_Flag;			//轮询时的错误标志

/*冻结相关的变量*/
extern 	u8		DL_Freeze_Day_Global;			//冻结日

/*关于串口的变量*/
//串口PC
extern 	u8		Com_PC_Recv_Buf_Ptr_W;				//串口PC 接收缓冲区的写指针
extern 	u8		Com_PC_Recv_Buf_Ptr_R;	       			//串口PC 接收缓冲区的读指针
extern 	u8		Com_PC_Recv_Buf[COM_RECV_BUF_SIZE_HW_PC];	//串口PC 接收缓冲区
extern 	u8		Com_PC_Process_Buf[COM_RECV_BUF_SIZE_HW_PC];	//串口PC 处理缓冲区
extern 	u8		Com_PC_Send_Buf[COM_SEND_BUF_SIZE_HW_PC];	//串口PC 发送缓冲区
extern 	u8 		Com_PC_Send_Data_Len;
extern 	u8 		Com_PC_Send_Total_Len;
extern 	u8 		Com_PC_Send_Check_Sum;
extern 	u8 		Com_PC_FE_Number ;				//发送数据帧之前的前导字符的个数
extern 	u8   		Com_PC_Send_Pos ;				//采用中断的方式发送时，发送的位置

//串口BJ
extern 	u8		Com_BJ_Recv_Buf_Ptr_W;				//串口BJ  接收缓冲区的写指针
extern 	u8		Com_BJ_Recv_Buf_Ptr_R;	       			//串口BJ  接收缓冲区的读指针
extern 	u8	 	Com_BJ_Recv_Buf[COM_RECV_BUF_SIZE_BJ];		//串口BJ  接收缓冲区
extern 	u8        	Com_BJ_Process_Buf[COM_RECV_BUF_SIZE_BJ];	//串口BJ  处理缓冲区
extern 	u8		Com_BJ_Send_Buf[COM_SEND_BUF_SIZE_BJ];		//串口BJ  发送缓冲区
extern 	u8 		Com_BJ_Send_Data_Len;
extern 	u8 		Com_BJ_Send_Total_Len;
extern 	u8 		Com_BJ_FE_Number;				//发送数据帧之前的前导字符的个数
extern 	u8   		Com_BJ_Send_Pos;				//采用中断的方式发送时，发送的位置

/*关于循显所使用的变量*/
extern	u8		Lcd_Display_Meter;			//范围0---34, 表的最大可能总数32+2
extern	u8 		Lcd_Display_Frame;			//范围0---表的实际总数+2
extern 	u8		Display_25_ms_Counter;			//循环显示25ms 计数器
extern 	u8		Display_1_S_Counter;			//循环显示1 S 计数器 
extern	u8 		InterVal_For_Cycle_Show_Global;		//循环显示间隔
extern 	u8 		lcd_buf[1024];
extern 	u8 		chinese_buf[32];
extern 	u8 		asc_buf[16];
extern 	u8		Lcd_Task_Flag;				//LCD 专用标志位字节,显示一段时间后，关闭显示

/*关于时间存储变量,包括ds12302 以及内部的RTC*/
extern 	time_t 			TIME;
extern 	struct tm 		ptim;
extern 	RTC_TIME		Time_Rtc;		

#endif

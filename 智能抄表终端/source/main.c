#include "71x_lib.h"
#include "led.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


u8   Time0_25_ms_Counter=0;						//25ms 计数器
u32  Global_Task_Flag = 0;						//全局任务标志
u32  Com_Task_Flag=0;							//串口所用到的任务标志

/*关于SPI_FLASH	中的存储区域的变量*/
u8  Meter_485_Addr[6]={0};						//表的485地址
u8  Meter_Asset_Number[6]={0};					//表的资产编号
u8  Meter_485_Addr_Bak[32][6]={0};				//32表的485地址，处理接收的485地址
u8  Meter_Asset_Number_Bak[32][6]={0};			//表的资产编号,处理接收到的资产编号
u8  Meter_Valid_Flag;							//表的有效标志
u8  Meter_Valid_Flag_Bak;						//表的有效标志
BASE_PARAMETER			Base_ParaMeter;			//采集器的一些基本参数
BASE_PARAMETER			Base_ParaMeter_Bak;		//采集器的一些基本参数
YEAR_MON_DAY			Year_Mon_Day;			//当前电量的采集日期

/*关于电量存储  变量*/
METER_32_DL_And_TIME	Meter_32_Dl_And_Time;	//存放32块表的当前电量以及电量采集的时间，需要冻结的时候，就将该值写入到SPI_FLASH里去
YEAR_MON_DAY		Meter_32_Date;			//存放32块表的当前电量的日期

/*冻结相关的变量*/
u8 	DL_Freeze_Day_Global = 1;				    //冻结日

/*循环采集相关的变量*/
u32	LunXun_1_S_Counter=0;				        //当该值到达设定的采集时间时，则设置轮循标志	
u32	InterVal_For_Cycle_Collectin_Global=1;	    //轮循采集间隔
u32	LunXun_Error_Flag;					    //轮询时的错误标志

/*串口PC相关变量（连接上位机）*/
u8		Com_PC_Recv_Buf_Ptr_W;						//串口PC  接收缓冲区的写指针
u8		Com_PC_Recv_Buf_Ptr_R;	       				//串口PC  接收缓冲区的读指针
u8		Com_PC_Recv_Buf[COM_RECV_BUF_SIZE_HW_PC];	//串口PC  接收缓冲区
u8		Com_PC_Process_Buf[COM_RECV_BUF_SIZE_HW_PC];	//串口PC  处理缓冲区
u8		Com_PC_Send_Buf[COM_SEND_BUF_SIZE_HW_PC];	//串口PC  发送缓冲区
u8 		Com_PC_Send_Data_Len;
u8 		Com_PC_Send_Total_Len;
u8 		Com_PC_Send_Check_Sum;
u8 		Com_PC_FE_Number =0XFF;						//发送数据帧之前的前导字符的个数
u8   	        Com_PC_Send_Pos =0XFF;						//采用中断的方式发送时，发送的位置

/*串口BJ相关变量（连接电能表）*/
u8		Com_BJ_Recv_Buf_Ptr_W;						//串口BJ 接收缓冲区的写指针
u8		Com_BJ_Recv_Buf_Ptr_R;	       					//串口BJ 接收缓冲区的读指针
u8	 	Com_BJ_Recv_Buf[COM_RECV_BUF_SIZE_BJ];		//串口BJ 接收缓冲区
u8              Com_BJ_Process_Buf[COM_RECV_BUF_SIZE_BJ];	//串口BJ 处理缓冲区
u8		Com_BJ_Send_Buf[COM_SEND_BUF_SIZE_BJ];		//串口BJ 发送缓冲区
u8 		Com_BJ_Send_Data_Len;
u8 		Com_BJ_Send_Total_Len;
u8 		Com_BJ_FE_Number =0xFF;						//发送数据帧之前的前导字符的个数
u8   	        Com_BJ_Send_Pos =0xFF;						//采用中断的方式发送时，发送的位置
u32		Com_BJ_Recv_Time_Counter=0;					//串口BJ 的接收时间计数器

/*关于循显所使用的变量*/
u8		Lcd_Display_Meter =1;					//范围0---34, 表的最大可能总数32+2
u8 		Lcd_Display_Frame =1;					//范围0---表的实际总数+2....    ,在显示电量的时候，表示表的位置   XX 表
u8		Display_25_ms_Counter=0;				//循环显示25ms 计数器
u8		Display_1_S_Counter=0;					//循环显示1 S 计数器 
u8 		InterVal_For_Cycle_Show_Global=6;		//循环显示间隔
u8 		lcd_buf[1024];
u8 		chinese_buf[32];
u8 		asc_buf[16];
u8		Lcd_Task_Flag;							//LCD 专用标志位字节

/*关于时间存储变量*/
time_t 					TIME;
struct tm 				ptim;
RTC_TIME				Time_Rtc;		

/*任务函数*/
void 	Task_1();			
void 	Task_2();			
void 	Task_3();
void 	Task_4();								

/*对主芯片STR711进行初始化*/
void	Str711_Init();




#define ECHO_BUFF_LEN 32
u8  echo_Buff[ECHO_BUFF_LEN];
u32 echo_Counter = 0;


void main(void)
{
	Str711_Init();	//对主芯片STR711进行初始化
        //Back_Light_On();
	/*从SPI_Flash中读出第一块电能表的485地址、资产编号.有效标志位即判断是否有该表*/
	SPI_Read_161d(BASE_ADDR_485_ADDR,&Meter_485_Addr[0],6);         
	SPI_Read_161d(BASE_ADDR_ASSET_NUMBER,&Meter_Asset_Number[0],6); 
	SPI_Read_161d(BASE_ADDR_VALID_FLAG,(u8*)(&Meter_Valid_Flag),1);

	/*从SPI_FLASH 中读出采集器的基本参数*/
	SPI_Read_161d(BASE_ADDR_BASE_PARA,(u8*)&Base_ParaMeter,sizeof(Base_ParaMeter));
	
	/*将循环采集周期由BCD码格式转换为10进制格式的数据*/
	InterVal_For_Cycle_Collectin_Global  = BCD_2_Dec(Base_ParaMeter.InterVal_For_Cycle_Collectin&0xFF)
	+BCD_2_Dec((Base_ParaMeter.InterVal_For_Cycle_Collectin>>8)&0xFF)*100;
	
	/*将循环显示间隔由BCD码格式转换为10进制格式的数据*/
	InterVal_For_Cycle_Show_Global = BCD_2_Dec(Base_ParaMeter.InterVal_For_Cycle_Show);
	
	/*将冻结日由BCD码格式的转换为10进制格式的数据*/
	DL_Freeze_Day_Global= BCD_2_Dec(Base_ParaMeter.DL_Freeze_Day);

    /*设置初始的与PC通信的波特率*/
	Base_ParaMeter.Baud_to_Pc = BAUD_UART_PC_ORDER_38400;
    /*因为更改了初始的与PC通信的波特率所以再将数据重新写回到SPI_Flash中去*/
	SPI_Write_161d(BASE_ADDR_BASE_PARA,(u8*)&Base_ParaMeter,sizeof(Base_ParaMeter));	
    /*配置与PC通信的UART*/
	UART_Config(UART_PC, BAUD_UART_PC_38400, UART_EVEN_PARITY, UART_1_StopBits, UARTM_8D_P);
	
	RTC_Read_Date_Time(&Time_Rtc);/*读取RTC 当前的日期*/

	//通电的时候马上采集一次当前电量
	Global_Task_Flag |= TASK_FLAG_BEGIN_LUNXUN;
    //主循环
	while(1)
	{
		WDG_CntRefresh();				//刷新看门狗的计数器值
		
		if(Global_Task_Flag&TASK_FLAG_25MS_TASK)
		{//25MS执行一次的任务
			Global_Task_Flag &=(~TASK_FLAG_25MS_TASK);
			WDG_CntRefresh();				//刷新看门狗的计数器值
			Task_3();
		}
        
		if(Global_Task_Flag &TASK_FLAG_BEGIN_LUNXUN)
		{//开始轮询
			Global_Task_Flag &=(~TASK_FLAG_BEGIN_LUNXUN);
			if((Global_Task_Flag&TASK_FLAG_LUNXUN_ING)==0)
			{    
                          WDG_CntRefresh(); //刷新看门狗的计数器值
			  LunXun_Start();   //开始轮询
			 }			
		}
        
                WDG_CntRefresh();   //刷新看门狗的计数器值
		Task_4();	
	}
}

/*********************************************************************************************
*任务函数:  Task1()
*功能:	    在收到表计的电量数据的应答帧时，进行相应的处理
*********************************************************************************************/
void 	Task_1()	
{
	Process_Com_2_Data();
}
/*********************************************************************************************
*任务函数:  Task2()
*功能:	    在收到上位机澹(485)命令时，执行相应的动作
*********************************************************************************************/
void 	Task_2()	
{
	Process_ComX_Data();	
}
/*********************************************************************************************
*任务函数:  Task3()
*功能:      25ms 执行一次的任务
*********************************************************************************************/
void 	Task_3()					
{
	WDG_CntRefresh();		//刷新看门狗的计数器值 
 	Parse_Com_Data(2);      //串口BJ
 	Parse_Com_Data(3);      //串口PC
	WDG_CntRefresh();		//刷新看门狗的计数器值
	
	Process_XunXian();      //处理循环显示

 	Key_Detect();           //检测按键
}
/*********************************************************************************************
*任务函数:  Task4()
*功能:	    判断串口数据的发送是否完成，如果完成将状态转换为接收状态
*********************************************************************************************/
void	Task_4()
{
	//判断串口PC的发送是否完成,如果完成,则将状态转换为接收状态
	if(Com_Task_Flag& TASK_FLAG_COM_PC_SEND_COMPLETE)
	{
		Com_Task_Flag &= (~TASK_FLAG_COM_PC_SEND_COMPLETE);
		Com_PC_Send_Total_Len		=0;
		Com_PC_Send_Pos			=0;
		GPIO_WordWrite(GPIO_LED_UP, GPIO_WordRead(GPIO_LED_UP) |LED_UP);//灭上行灯		
	}
		
	//判断串口BJ的发送是否完成,如果完成,则将状态转换为接收状态
	if(Com_Task_Flag& TASK_FLAG_COM_BJ_SEND_COMPLETE)
	{
		Com_Task_Flag &= (~TASK_FLAG_COM_BJ_SEND_COMPLETE);
		Com_BJ_Send_Total_Len		= 0;
		Com_BJ_Send_Pos			= 0;					
		GPIO_WordWrite(GPIO_LED_DOWN, GPIO_WordRead(GPIO_LED_DOWN) |(LED_DOWN));//灭下行灯
	}			
}

/*********************************************************************************************
*函数名称:  Send_Ack_Or_Data_To_Host_Uart_PC()
*功能描述:  对收到串口PC 的上位机命令进行应答
*********************************************************************************************/
void Send_Ack_Or_Data_To_Host_Uart_PC()
{
	u32 i;
	u8 ch;
	
	GPIO_WordWrite(GPIO_LED_UP,GPIO_WordRead(GPIO_LED_UP)&(~LED_UP));//点亮上行灯
	//向上位机发送应答数据帧
	Com_PC_Send_Data_Len = Com_PC_Send_Buf[9];
	Com_PC_Send_Check_Sum = 0;
	
	//对数据域加 0x33
	for(i=0;i<Com_PC_Send_Data_Len;i++)
	{
		Com_PC_Send_Buf[10+i] +=0X33;
	}
	for(i=0;i<10+Com_PC_Send_Data_Len;i++)
	{
		Com_PC_Send_Check_Sum += Com_PC_Send_Buf[i];
	}
	Com_PC_Send_Buf[i]= Com_PC_Send_Check_Sum;
	Com_PC_Send_Buf[i+1] = 0x16;
	Com_PC_Send_Buf[i+2] = 0x16;
	Com_PC_Send_Buf[i+3] = 0x16;
	
	Com_PC_FE_Number =0;
	
	//关闭接收中断，取得发送的总长度
	Com_PC_Send_Total_Len = Com_PC_Send_Data_Len +14;	//包括校验和以及0X16
	Com_PC_Send_Pos =0;
	ch = 0XFE;
	PC_TX_ENABLE;	//PC发送使能	
	
 	UART_ByteSend(UART_PC,&ch );
 	UART_ByteSend(UART_PC,&ch );
 	UART_ByteSend(UART_PC,&ch );
 	UART_ByteSend(UART_PC,&ch );

	do
	{
		if(Com_PC_Send_Pos<Com_PC_Send_Total_Len)
		{
		 	UART_ByteSend(UART_PC,&Com_PC_Send_Buf[Com_PC_Send_Pos]);
			Com_PC_Send_Pos++;
		}
		else
		{
			break;
		}
	}while (!(UART_FlagStatus(UART_PC) & UART_TxFull));
	
	Com_Task_Flag  |= TASK_FLAG_COM_PC_SEND_TIME;
 	UART_ItConfig(UART_PC,UART_TxEmpty|UART_TxHalfEmpty,ENABLE);		//发送中断使能
 	UART_ItConfig(UART_PC,UART_RxHalfFull|UART_TimeOutNotEmpty,DISABLE);		//接收中断禁止	
		
}

/*********************************************************************************************
*函数名称:  Str711_Init()
*功能描述:  对主芯片进行初始化
*********************************************************************************************/
void	Str711_Init()
{
    u32	Global_Delay =0 ;
	debug();	   //取得各外设的寄存器的基地址
    Global_Task_Flag = 0;
	Com_BJ_Recv_Buf_Ptr_W = 0;
	Com_BJ_Recv_Buf_Ptr_R = 0;
	Com_PC_Recv_Buf_Ptr_W = 0;
	Com_PC_Recv_Buf_Ptr_R = 0;
	Time0_25_ms_Counter  = 0;
	
	//初始化系统时钟
    RCCU_Div2Config(ENABLE);						// Enable DIV2
	RCCU_MCLKConfig(RCCU_DEFAULT);					// Configure MCLK = RCLK
	RCCU_FCLKConfig(RCCU_DEFAULT);					// Configure FCLK = RCLK
	RCCU_PCLKConfig(RCCU_RCLK_4);					// Configure PCLK = RCLK
 	RCCU_PLL1Config(RCCU_Mul_12, RCCU_Div_3) ;		// Configure the PLL1 ( * 12 , / 3 )
 	while(RCCU_FlagStatus(RCCU_PLL1_LOCK) == RESET);// Wait PLL to lock
	RCCU_RCLKSourceConfig(RCCU_PLL1_Output) ;		// Select PLL1_Output as RCLK clock
  	// at this step the CKOUT signal should be equal to 8 Mhz    16/2/4*12/3   = 8M
	for(Global_Delay=0;Global_Delay<0x200000;Global_Delay++);

  	/*  Configure the GPIO1 Port */
  	/*  Timer 1 Output Compare function pin (P1.7) configured as Alternate function pushpull. */
  	//#define T1_OCMP_A 0x80
  	GPIO_Config (GPIO_TIMER_1_OCMP , 0x80 , GPIO_AF_PP );
  	TIM_Init ( TIM1 );
  	TIM_PrescalerConfig ( TIM1, 0 );
  	/*  Generate a PWM Signal :
       	- Full Period  = 7FFF
       	- Duty Cycle   = 50 % ( 3FFF ) */
	//  TIM_PWMOModeConfig ( TIM1, 0x3FFF, TIM_HIGH, 0x7FFF, TIM_LOW );
  	TIM_PWMOModeConfig ( TIM1, 105, TIM_HIGH, 210, TIM_LOW );

	//Timer1 的预分频 值为 5, 5*40*40000 = 8M  得到40HZ 的频率，即每S 产生40次溢出中断
	TIM_Init(TIM2);								//初始化Timer1
	TIM_PrescalerConfig(TIM2, 4);				//预分频值为100. 每25ms 产生一次溢出中断
	TIM_ITConfig(TIM2, TIM_OCA_IT, ENABLE);		//Timer1 溢出中断使能

	EIC_IRQChannelConfig(T2TIMI_IRQChannel, ENABLE);//允许产生Timer1 快速中断
	EIC_IRQChannelPriorityConfig(T2TIMI_IRQChannel,15);
	EIC_IRQConfig(ENABLE);	//允许快速中断

	TIM_OCMPModeConfig(TIM2,TIM_CHANNEL_A,TIMER_A_OUTPUT_COMPARE_VALUE,TIM_TIMING,TIM_HIGH);	//当仅仅用作计时的时候,最后一个参数没有作用
	TIM_CounterConfig(TIM2, TIM_START);	//定时器 Timer1 开始工作

	/*其他初始化*/
	LCD_Init();	        //液晶显示屏
	Key_Init();         //按键
    FLASH_Init();       //flash存储器
	LED_Init();         //指示灯
    InitBSPIFlash();    //BSPI接口
    UART_Init();        //UART接口	
    RTC_Init();         //实时时钟
    WDG_Init();         //看门狗
        
}



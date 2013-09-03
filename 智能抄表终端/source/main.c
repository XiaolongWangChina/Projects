#include "71x_lib.h"
#include "led.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


u8   Time0_25_ms_Counter=0;						//25ms ������
u32  Global_Task_Flag = 0;						//ȫ�������־
u32  Com_Task_Flag=0;							//�������õ��������־

/*����SPI_FLASH	�еĴ洢����ı���*/
u8  Meter_485_Addr[6]={0};						//���485��ַ
u8  Meter_Asset_Number[6]={0};					//����ʲ����
u8  Meter_485_Addr_Bak[32][6]={0};				//32���485��ַ��������յ�485��ַ
u8  Meter_Asset_Number_Bak[32][6]={0};			//����ʲ����,������յ����ʲ����
u8  Meter_Valid_Flag;							//�����Ч��־
u8  Meter_Valid_Flag_Bak;						//�����Ч��־
BASE_PARAMETER			Base_ParaMeter;			//�ɼ�����һЩ��������
BASE_PARAMETER			Base_ParaMeter_Bak;		//�ɼ�����һЩ��������
YEAR_MON_DAY			Year_Mon_Day;			//��ǰ�����Ĳɼ�����

/*���ڵ����洢  ����*/
METER_32_DL_And_TIME	Meter_32_Dl_And_Time;	//���32���ĵ�ǰ�����Լ������ɼ���ʱ�䣬��Ҫ�����ʱ�򣬾ͽ���ֵд�뵽SPI_FLASH��ȥ
YEAR_MON_DAY		Meter_32_Date;			//���32���ĵ�ǰ����������

/*������صı���*/
u8 	DL_Freeze_Day_Global = 1;				    //������

/*ѭ���ɼ���صı���*/
u32	LunXun_1_S_Counter=0;				        //����ֵ�����趨�Ĳɼ�ʱ��ʱ����������ѭ��־	
u32	InterVal_For_Cycle_Collectin_Global=1;	    //��ѭ�ɼ����
u32	LunXun_Error_Flag;					    //��ѯʱ�Ĵ����־

/*����PC��ر�����������λ����*/
u8		Com_PC_Recv_Buf_Ptr_W;						//����PC  ���ջ�������дָ��
u8		Com_PC_Recv_Buf_Ptr_R;	       				//����PC  ���ջ������Ķ�ָ��
u8		Com_PC_Recv_Buf[COM_RECV_BUF_SIZE_HW_PC];	//����PC  ���ջ�����
u8		Com_PC_Process_Buf[COM_RECV_BUF_SIZE_HW_PC];	//����PC  ��������
u8		Com_PC_Send_Buf[COM_SEND_BUF_SIZE_HW_PC];	//����PC  ���ͻ�����
u8 		Com_PC_Send_Data_Len;
u8 		Com_PC_Send_Total_Len;
u8 		Com_PC_Send_Check_Sum;
u8 		Com_PC_FE_Number =0XFF;						//��������֮֡ǰ��ǰ���ַ��ĸ���
u8   	        Com_PC_Send_Pos =0XFF;						//�����жϵķ�ʽ����ʱ�����͵�λ��

/*����BJ��ر��������ӵ��ܱ�*/
u8		Com_BJ_Recv_Buf_Ptr_W;						//����BJ ���ջ�������дָ��
u8		Com_BJ_Recv_Buf_Ptr_R;	       					//����BJ ���ջ������Ķ�ָ��
u8	 	Com_BJ_Recv_Buf[COM_RECV_BUF_SIZE_BJ];		//����BJ ���ջ�����
u8              Com_BJ_Process_Buf[COM_RECV_BUF_SIZE_BJ];	//����BJ ��������
u8		Com_BJ_Send_Buf[COM_SEND_BUF_SIZE_BJ];		//����BJ ���ͻ�����
u8 		Com_BJ_Send_Data_Len;
u8 		Com_BJ_Send_Total_Len;
u8 		Com_BJ_FE_Number =0xFF;						//��������֮֡ǰ��ǰ���ַ��ĸ���
u8   	        Com_BJ_Send_Pos =0xFF;						//�����жϵķ�ʽ����ʱ�����͵�λ��
u32		Com_BJ_Recv_Time_Counter=0;					//����BJ �Ľ���ʱ�������

/*����ѭ����ʹ�õı���*/
u8		Lcd_Display_Meter =1;					//��Χ0---34, �������������32+2
u8 		Lcd_Display_Frame =1;					//��Χ0---���ʵ������+2....    ,����ʾ������ʱ�򣬱�ʾ���λ��   XX ��
u8		Display_25_ms_Counter=0;				//ѭ����ʾ25ms ������
u8		Display_1_S_Counter=0;					//ѭ����ʾ1 S ������ 
u8 		InterVal_For_Cycle_Show_Global=6;		//ѭ����ʾ���
u8 		lcd_buf[1024];
u8 		chinese_buf[32];
u8 		asc_buf[16];
u8		Lcd_Task_Flag;							//LCD ר�ñ�־λ�ֽ�

/*����ʱ��洢����*/
time_t 					TIME;
struct tm 				ptim;
RTC_TIME				Time_Rtc;		

/*������*/
void 	Task_1();			
void 	Task_2();			
void 	Task_3();
void 	Task_4();								

/*����оƬSTR711���г�ʼ��*/
void	Str711_Init();




#define ECHO_BUFF_LEN 32
u8  echo_Buff[ECHO_BUFF_LEN];
u32 echo_Counter = 0;


void main(void)
{
	Str711_Init();	//����оƬSTR711���г�ʼ��
        //Back_Light_On();
	/*��SPI_Flash�ж�����һ����ܱ��485��ַ���ʲ����.��Ч��־λ���ж��Ƿ��иñ�*/
	SPI_Read_161d(BASE_ADDR_485_ADDR,&Meter_485_Addr[0],6);         
	SPI_Read_161d(BASE_ADDR_ASSET_NUMBER,&Meter_Asset_Number[0],6); 
	SPI_Read_161d(BASE_ADDR_VALID_FLAG,(u8*)(&Meter_Valid_Flag),1);

	/*��SPI_FLASH �ж����ɼ����Ļ�������*/
	SPI_Read_161d(BASE_ADDR_BASE_PARA,(u8*)&Base_ParaMeter,sizeof(Base_ParaMeter));
	
	/*��ѭ���ɼ�������BCD���ʽת��Ϊ10���Ƹ�ʽ������*/
	InterVal_For_Cycle_Collectin_Global  = BCD_2_Dec(Base_ParaMeter.InterVal_For_Cycle_Collectin&0xFF)
	+BCD_2_Dec((Base_ParaMeter.InterVal_For_Cycle_Collectin>>8)&0xFF)*100;
	
	/*��ѭ����ʾ�����BCD���ʽת��Ϊ10���Ƹ�ʽ������*/
	InterVal_For_Cycle_Show_Global = BCD_2_Dec(Base_ParaMeter.InterVal_For_Cycle_Show);
	
	/*����������BCD���ʽ��ת��Ϊ10���Ƹ�ʽ������*/
	DL_Freeze_Day_Global= BCD_2_Dec(Base_ParaMeter.DL_Freeze_Day);

    /*���ó�ʼ����PCͨ�ŵĲ�����*/
	Base_ParaMeter.Baud_to_Pc = BAUD_UART_PC_ORDER_38400;
    /*��Ϊ�����˳�ʼ����PCͨ�ŵĲ����������ٽ���������д�ص�SPI_Flash��ȥ*/
	SPI_Write_161d(BASE_ADDR_BASE_PARA,(u8*)&Base_ParaMeter,sizeof(Base_ParaMeter));	
    /*������PCͨ�ŵ�UART*/
	UART_Config(UART_PC, BAUD_UART_PC_38400, UART_EVEN_PARITY, UART_1_StopBits, UARTM_8D_P);
	
	RTC_Read_Date_Time(&Time_Rtc);/*��ȡRTC ��ǰ������*/

	//ͨ���ʱ�����ϲɼ�һ�ε�ǰ����
	Global_Task_Flag |= TASK_FLAG_BEGIN_LUNXUN;
    //��ѭ��
	while(1)
	{
		WDG_CntRefresh();				//ˢ�¿��Ź��ļ�����ֵ
		
		if(Global_Task_Flag&TASK_FLAG_25MS_TASK)
		{//25MSִ��һ�ε�����
			Global_Task_Flag &=(~TASK_FLAG_25MS_TASK);
			WDG_CntRefresh();				//ˢ�¿��Ź��ļ�����ֵ
			Task_3();
		}
        
		if(Global_Task_Flag &TASK_FLAG_BEGIN_LUNXUN)
		{//��ʼ��ѯ
			Global_Task_Flag &=(~TASK_FLAG_BEGIN_LUNXUN);
			if((Global_Task_Flag&TASK_FLAG_LUNXUN_ING)==0)
			{    
                          WDG_CntRefresh(); //ˢ�¿��Ź��ļ�����ֵ
			  LunXun_Start();   //��ʼ��ѯ
			 }			
		}
        
                WDG_CntRefresh();   //ˢ�¿��Ź��ļ�����ֵ
		Task_4();	
	}
}

/*********************************************************************************************
*������:  Task1()
*����:	    ���յ���Ƶĵ������ݵ�Ӧ��֡ʱ��������Ӧ�Ĵ���
*********************************************************************************************/
void 	Task_1()	
{
	Process_Com_2_Data();
}
/*********************************************************************************************
*������:  Task2()
*����:	    ���յ���λ���(485)����ʱ��ִ����Ӧ�Ķ���
*********************************************************************************************/
void 	Task_2()	
{
	Process_ComX_Data();	
}
/*********************************************************************************************
*������:  Task3()
*����:      25ms ִ��һ�ε�����
*********************************************************************************************/
void 	Task_3()					
{
	WDG_CntRefresh();		//ˢ�¿��Ź��ļ�����ֵ 
 	Parse_Com_Data(2);      //����BJ
 	Parse_Com_Data(3);      //����PC
	WDG_CntRefresh();		//ˢ�¿��Ź��ļ�����ֵ
	
	Process_XunXian();      //����ѭ����ʾ

 	Key_Detect();           //��ⰴ��
}
/*********************************************************************************************
*������:  Task4()
*����:	    �жϴ������ݵķ����Ƿ���ɣ������ɽ�״̬ת��Ϊ����״̬
*********************************************************************************************/
void	Task_4()
{
	//�жϴ���PC�ķ����Ƿ����,������,��״̬ת��Ϊ����״̬
	if(Com_Task_Flag& TASK_FLAG_COM_PC_SEND_COMPLETE)
	{
		Com_Task_Flag &= (~TASK_FLAG_COM_PC_SEND_COMPLETE);
		Com_PC_Send_Total_Len		=0;
		Com_PC_Send_Pos			=0;
		GPIO_WordWrite(GPIO_LED_UP, GPIO_WordRead(GPIO_LED_UP) |LED_UP);//�����е�		
	}
		
	//�жϴ���BJ�ķ����Ƿ����,������,��״̬ת��Ϊ����״̬
	if(Com_Task_Flag& TASK_FLAG_COM_BJ_SEND_COMPLETE)
	{
		Com_Task_Flag &= (~TASK_FLAG_COM_BJ_SEND_COMPLETE);
		Com_BJ_Send_Total_Len		= 0;
		Com_BJ_Send_Pos			= 0;					
		GPIO_WordWrite(GPIO_LED_DOWN, GPIO_WordRead(GPIO_LED_DOWN) |(LED_DOWN));//�����е�
	}			
}

/*********************************************************************************************
*��������:  Send_Ack_Or_Data_To_Host_Uart_PC()
*��������:  ���յ�����PC ����λ���������Ӧ��
*********************************************************************************************/
void Send_Ack_Or_Data_To_Host_Uart_PC()
{
	u32 i;
	u8 ch;
	
	GPIO_WordWrite(GPIO_LED_UP,GPIO_WordRead(GPIO_LED_UP)&(~LED_UP));//�������е�
	//����λ������Ӧ������֡
	Com_PC_Send_Data_Len = Com_PC_Send_Buf[9];
	Com_PC_Send_Check_Sum = 0;
	
	//��������� 0x33
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
	
	//�رս����жϣ�ȡ�÷��͵��ܳ���
	Com_PC_Send_Total_Len = Com_PC_Send_Data_Len +14;	//����У����Լ�0X16
	Com_PC_Send_Pos =0;
	ch = 0XFE;
	PC_TX_ENABLE;	//PC����ʹ��	
	
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
 	UART_ItConfig(UART_PC,UART_TxEmpty|UART_TxHalfEmpty,ENABLE);		//�����ж�ʹ��
 	UART_ItConfig(UART_PC,UART_RxHalfFull|UART_TimeOutNotEmpty,DISABLE);		//�����жϽ�ֹ	
		
}

/*********************************************************************************************
*��������:  Str711_Init()
*��������:  ����оƬ���г�ʼ��
*********************************************************************************************/
void	Str711_Init()
{
    u32	Global_Delay =0 ;
	debug();	   //ȡ�ø�����ļĴ����Ļ���ַ
    Global_Task_Flag = 0;
	Com_BJ_Recv_Buf_Ptr_W = 0;
	Com_BJ_Recv_Buf_Ptr_R = 0;
	Com_PC_Recv_Buf_Ptr_W = 0;
	Com_PC_Recv_Buf_Ptr_R = 0;
	Time0_25_ms_Counter  = 0;
	
	//��ʼ��ϵͳʱ��
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

	//Timer1 ��Ԥ��Ƶ ֵΪ 5, 5*40*40000 = 8M  �õ�40HZ ��Ƶ�ʣ���ÿS ����40������ж�
	TIM_Init(TIM2);								//��ʼ��Timer1
	TIM_PrescalerConfig(TIM2, 4);				//Ԥ��ƵֵΪ100. ÿ25ms ����һ������ж�
	TIM_ITConfig(TIM2, TIM_OCA_IT, ENABLE);		//Timer1 ����ж�ʹ��

	EIC_IRQChannelConfig(T2TIMI_IRQChannel, ENABLE);//�������Timer1 �����ж�
	EIC_IRQChannelPriorityConfig(T2TIMI_IRQChannel,15);
	EIC_IRQConfig(ENABLE);	//��������ж�

	TIM_OCMPModeConfig(TIM2,TIM_CHANNEL_A,TIMER_A_OUTPUT_COMPARE_VALUE,TIM_TIMING,TIM_HIGH);	//������������ʱ��ʱ��,���һ������û������
	TIM_CounterConfig(TIM2, TIM_START);	//��ʱ�� Timer1 ��ʼ����

	/*������ʼ��*/
	LCD_Init();	        //Һ����ʾ��
	Key_Init();         //����
    FLASH_Init();       //flash�洢��
	LED_Init();         //ָʾ��
    InitBSPIFlash();    //BSPI�ӿ�
    UART_Init();        //UART�ӿ�	
    RTC_Init();         //ʵʱʱ��
    WDG_Init();         //���Ź�
        
}



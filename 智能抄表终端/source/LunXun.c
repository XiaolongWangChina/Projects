#include "71x_lib.h" 
#include "LunXun.h"

/****************************************************************************
��������:   LunXun_Start()
��������:   ��ʼ��ѭ
****************************************************************************/
void	LunXun_Start()
{
	//�����ж���Ч��Ƶ��������������0�� ���˳���ѭ
	if(Base_ParaMeter.Meter_Total_Number <=0)
	{
		Global_Task_Flag &= ~TASK_FLAG_LUNXUN_ING;	//���������ѭ��־
		return;
	}
	//����������ѭ��־
	Global_Task_Flag |= (TASK_FLAG_LUNXUN_PERMIT|TASK_FLAG_LUNXUN_ING);

	GPIO_WordWrite(GPIO_LED_WARNING,GPIO_WordRead(GPIO_LED_WARNING) |(LED_WARNNING));//�����������
	LunXun_One_Meter(); 					 
}
/****************************************************************************
��������:   LunXun_One_Meter()
��������:   ��ʼ��ѭĳһ����
****************************************************************************/
void	LunXun_One_Meter()
{
	u8	temp_i;
	//�����ж���û����ѭ�����־��
	if(!(Global_Task_Flag&TASK_FLAG_LUNXUN_PERMIT))
	{	//���û����ѭ�����־�Ļ������˳� 
		Global_Task_Flag &= ~(TASK_FLAG_LUNXUN_ING|TASK_FLAG_LUNXUN_COMMAND_SEND_COMPLETE);	//��������ѭ��־
		return;	 
	}
	//�������ѭ�����־����ѽ�Ҫ���͵�����д�뷢�ͻ�����
	//�����жϸ�λ���ϵĵ���Ƿ����

      if(Meter_Valid_Flag==METER_IS_VALID)
	{			
		//��ȡһ�ε�ǰ��ʱ��
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

		//�����жϵķ�ʽ���з���
		Com_BJ_FE_Number  =0;
		Com_BJ_Send_Pos    =0;
		Com_BJ_Send_Total_Len = 14;
		BJ_TX_ENABLE;	//����BJ תΪ����״̬
	 	temp_i = 0xFE;
	 	UART_ByteSend(UART_BJ,&temp_i);
 	 	UART_ItConfig(UART_BJ,UART_RxBufFull,DISABLE);		//�����жϽ�ֹ
	 	UART_ItConfig(UART_BJ,UART_TxEmpty,ENABLE);		//�����ж�ʹ��

		Com_Task_Flag  |= TASK_FLAG_COM_BJ_SEND_TIME;			
		Global_Task_Flag |= TASK_FLAG_LUNXUN_COMMAND_SEND_COMPLETE;
		//�������е� 
		GPIO_WordWrite(GPIO_LED_DOWN, GPIO_WordRead(GPIO_LED_DOWN) &(~LED_DOWN));
		
              Global_Task_Flag &= ~TASK_FLAG_LUNXUN_ING;	//���������ѭ��־
	}
	else
	{
		//������ѭ����
		Global_Task_Flag &= ~TASK_FLAG_LUNXUN_ING;

		//�����ѭ�����־
		Global_Task_Flag &= ~TASK_FLAG_LUNXUN_PERMIT;
	}
}
/****************************************************************************
��������:   Process_Com_2_Data()
��������:   ���յ����ܱ�Ļش�(����ֵ)����д���
****************************************************************************/
void Process_Com_2_Data()
{
	u8 Data_Len;
	u8 i;
	Data_Len = Com_BJ_Process_Buf[9];	
 
	//�ж��յ������ݵĳ����Ƿ���22 B   2B(DI1DI0) + 20B(����ֵ) 
	if(Data_Len<22)
	{	//���յ��ĵ����ĳ��Ȳ���20���ֽڵ�ʱ��,
		//����Ϊ��ֻ��ǰ4���ֽ��������ĵ���������,�����16���ֽ�����(0x33)
		memset(&Com_BJ_Process_Buf[16],0x33,16);//���������ڵ�λ���ϵĺ����16���ֽ����� 
		//���ܵ�������ƽ������λ��(������ƽ������λ��)
		memcpy(&Com_BJ_Process_Buf[24],&Com_BJ_Process_Buf[12],4);
		Data_Len = 22;
	}
	else
	{
		//�жϺ����16���ֽ��Ƿ���ȫ��
		for(i=0;i<16;i++)
		{
			if(Com_BJ_Process_Buf[16+i]!=0x33)
			{
				break;
			}
		}
		if(i>=16)
		{//�����ȫ��,���ܵ�������ƽ������λ��(������ƽ������λ��)
			memcpy(&Com_BJ_Process_Buf[24],&Com_BJ_Process_Buf[12],4);
		}
	}

	//���ɼ��������ݴ����ڴ�����Ӧ�ı�Ƶ�λ����
        memcpy((u8*)&Meter_32_Dl_And_Time,&Com_BJ_Process_Buf[12],20);
	Global_Task_Flag &= ~TASK_FLAG_LUNXUN_ING;
	Global_Task_Flag &= ~TASK_FLAG_LUNXUN_PERMIT;//�����ѭ�����־	
}
/*********************************************************************************
��������:   ChangeAddrToAA()
��������:   ���ڶԲɼ��������˱�̲���.����ֹͣ��ǰ��ѭ. �ұ�����ѭ�ĳɹ�ʧ�ܴ�������
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

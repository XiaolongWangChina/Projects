#ifndef	_EXTERN_VAR_H_
#define	_EXTERN_VAR_H_
#include <time.h>








extern u8  echo_Buff[];
extern u32 echo_Counter;










extern	u8    Time0_25_ms_Counter;       //25ms ������
extern 	u32  Global_Task_Flag ;		//ȫ�������־
extern	u32  Com_Task_Flag;		//�������õ��������־

/*����SPI_FLASH	�еĴ洢����ı���*/
extern 	u8  Meter_485_Addr[6];			//���485��ַ
extern 	u8  Meter_Asset_Number[6];		//����ʲ����
extern 	u8  Meter_485_Addr_Bak[32][6];		//32���485���õ�ַ,������յ�������
extern 	u8  Meter_Asset_Number_Bak[32][6];	//32����ʲ���ţ�������յ�������
extern 	u8  Meter_Valid_Flag;			//�����Ч��־
extern	u8  Meter_Valid_Flag_Bak;		//�����Ч��־
extern 	BASE_PARAMETER		Base_ParaMeter;		//�ɼ�����һЩ��������
extern 	BASE_PARAMETER		Base_ParaMeter_Bak;	//�ɼ�����һЩ��������
extern 	YEAR_MON_DAY		Year_Mon_Day;		//��ǰ�����Ĳɼ�����
extern 	METER_32_DL_And_TIME	Meter_32_Dl_And_Time;	//��ű�ĵ�ǰ�����Լ������ɼ���ʱ��
extern	YEAR_MON_DAY		Meter_32_Date;		//��ű�ĵ�ǰ����������

/*ѭ���ɼ���صı���*/
extern	u32		LunXun_1_S_Counter;			//����ֵ�����趨�Ĳɼ�ʱ��ʱ����������ѭ��־
extern 	u32		InterVal_For_Cycle_Collectin_Global;	//��ѭ�ɼ����
extern	u32		LunXun_Error_Flag;			//��ѯʱ�Ĵ����־

/*������صı���*/
extern 	u8		DL_Freeze_Day_Global;			//������

/*���ڴ��ڵı���*/
//����PC
extern 	u8		Com_PC_Recv_Buf_Ptr_W;				//����PC ���ջ�������дָ��
extern 	u8		Com_PC_Recv_Buf_Ptr_R;	       			//����PC ���ջ������Ķ�ָ��
extern 	u8		Com_PC_Recv_Buf[COM_RECV_BUF_SIZE_HW_PC];	//����PC ���ջ�����
extern 	u8		Com_PC_Process_Buf[COM_RECV_BUF_SIZE_HW_PC];	//����PC ��������
extern 	u8		Com_PC_Send_Buf[COM_SEND_BUF_SIZE_HW_PC];	//����PC ���ͻ�����
extern 	u8 		Com_PC_Send_Data_Len;
extern 	u8 		Com_PC_Send_Total_Len;
extern 	u8 		Com_PC_Send_Check_Sum;
extern 	u8 		Com_PC_FE_Number ;				//��������֮֡ǰ��ǰ���ַ��ĸ���
extern 	u8   		Com_PC_Send_Pos ;				//�����жϵķ�ʽ����ʱ�����͵�λ��

//����BJ
extern 	u8		Com_BJ_Recv_Buf_Ptr_W;				//����BJ  ���ջ�������дָ��
extern 	u8		Com_BJ_Recv_Buf_Ptr_R;	       			//����BJ  ���ջ������Ķ�ָ��
extern 	u8	 	Com_BJ_Recv_Buf[COM_RECV_BUF_SIZE_BJ];		//����BJ  ���ջ�����
extern 	u8        	Com_BJ_Process_Buf[COM_RECV_BUF_SIZE_BJ];	//����BJ  ��������
extern 	u8		Com_BJ_Send_Buf[COM_SEND_BUF_SIZE_BJ];		//����BJ  ���ͻ�����
extern 	u8 		Com_BJ_Send_Data_Len;
extern 	u8 		Com_BJ_Send_Total_Len;
extern 	u8 		Com_BJ_FE_Number;				//��������֮֡ǰ��ǰ���ַ��ĸ���
extern 	u8   		Com_BJ_Send_Pos;				//�����жϵķ�ʽ����ʱ�����͵�λ��

/*����ѭ����ʹ�õı���*/
extern	u8		Lcd_Display_Meter;			//��Χ0---34, �������������32+2
extern	u8 		Lcd_Display_Frame;			//��Χ0---���ʵ������+2
extern 	u8		Display_25_ms_Counter;			//ѭ����ʾ25ms ������
extern 	u8		Display_1_S_Counter;			//ѭ����ʾ1 S ������ 
extern	u8 		InterVal_For_Cycle_Show_Global;		//ѭ����ʾ���
extern 	u8 		lcd_buf[1024];
extern 	u8 		chinese_buf[32];
extern 	u8 		asc_buf[16];
extern 	u8		Lcd_Task_Flag;				//LCD ר�ñ�־λ�ֽ�,��ʾһ��ʱ��󣬹ر���ʾ

/*����ʱ��洢����,����ds12302 �Լ��ڲ���RTC*/
extern 	time_t 			TIME;
extern 	struct tm 		ptim;
extern 	RTC_TIME		Time_Rtc;		

#endif

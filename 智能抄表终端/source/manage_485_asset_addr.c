/*******************************************************************************
* File Name          : manage_485_asset_addr.c
* Description        : ����485��ַ���ʲ���ŵ����衢���ӡ�ɾ�����滻
********************************************************************************/

#include <stdio.h>
#include <string.h>
#include "manage_485_asset_addr.h"


/*******************************************************************************
* Function Name              : Set_Meter_485_Addr
* Description                   : ����485��ַ
* inputpara                     :���뻺����
* Return                          : u8
*******************************************************************************/
u8 Set_Meter_485_Addr(u8* Process_Buf_Ptr)
{
	u8	temp_i=0,temp_j=0;
	bool 	Is_All_0_485_Addr;//485��ַȫ��
	bool	Is_All_0_Asset_Number;//�ʲ����ȫ��
	temp_i = Process_Buf_Ptr[9]-6	;//485��ַ�ĳ���  ,6�ı���. ����2���ֽڵ�id ,4���ֽڵ�����
	memset(&Meter_485_Addr_Bak[0][0],0X00,192);
	memcpy(&Meter_485_Addr_Bak[0][0],&Process_Buf_Ptr[16],temp_i);
	Global_Task_Flag|=TASK_FLAG_SET_485_ADDRESS_7213;
	if(Global_Task_Flag & TASK_FLAG_SET_ASSET_NUMBER_9213)
	{
            //�����жϴ��͹�����485��ַ���ʲ������û������ֻ��һ����ȫ������������еĻ�������
            Base_ParaMeter_Bak.Meter_Total_Number = 0;
            Meter_Valid_Flag_Bak=METER_NOT_VALID;
            for(temp_i=0;temp_i<32;temp_i++)
            {
              Is_All_0_485_Addr =TRUE;
              Is_All_0_Asset_Number = TRUE;
              for(temp_j=0;temp_j<6;temp_j++)
              {
                      if(Meter_485_Addr_Bak[temp_i][temp_j]!=0)
                      {
                              Is_All_0_485_Addr = FALSE;
                      }
                      if(Meter_Asset_Number_Bak[temp_i][temp_j] !=0)
                      {
                              Is_All_0_Asset_Number = FALSE;
                      }
              }
              if(Is_All_0_485_Addr==FALSE&& Is_All_0_Asset_Number==false)//�ñ����Ч
              {
                      Base_ParaMeter_Bak.Meter_Total_Number ++;
                      Meter_Valid_Flag_Bak= METER_IS_VALID;	
                      if(Base_ParaMeter_Bak.Meter_Total_Number-1>0)
                      {
                               memset(&Meter_485_Addr_Bak[temp_i][0],0X00,6);
                           memset(&Meter_Asset_Number_Bak[temp_i][0],0X00,6);
                           break;
                      }
                      Meter_Valid_Flag=Meter_Valid_Flag_Bak;
                      SPI_Write_161d(BASE_ADDR_VALID_FLAG,(u8*)(&Meter_Valid_Flag),1);
                      Base_ParaMeter.Meter_Total_Number  = Base_ParaMeter_Bak.Meter_Total_Number;
                      SPI_Write_161d(BASE_ADDR_BASE_PARA,(u8*)&Base_ParaMeter,sizeof(Base_ParaMeter));
                      memcpy(&Meter_485_Addr[0],&Meter_485_Addr_Bak[temp_i][0],6);
                      SPI_Write_161d(BASE_ADDR_485_ADDR,&Meter_485_Addr[0],6);
                                
                      memcpy(&Meter_Asset_Number[0],&Meter_Asset_Number_Bak[temp_i][0],6);
                      SPI_Write_161d(BASE_ADDR_ASSET_NUMBER,&Meter_Asset_Number[0],6);

              }
			
	}
		LunXun_Error_Flag =0;
		//������Ͽ�ʼ���߲ɼ�
		Global_Task_Flag |= TASK_FLAG_BEGIN_LUNXUN;
              Global_Task_Flag &= ~TASK_FLAG_LUNXUN_ING;	//���������ѭ��־
		Global_Task_Flag &= ~(TASK_FLAG_SET_ASSET_NUMBER_9213|TASK_FLAG_SET_485_ADDRESS_7213);		
	}


	return 1;
}

/*******************************************************************************
* Function Name              : Set_Meter_Asset_Addr
* Description                   : �����ʲ����
* inputpara                     : ���뻺����
* Return                          : u8
*******************************************************************************/
u8 Set_Meter_Asset_Addr(u8* Process_Buf_Ptr)
{
	u8		temp_i=0,temp_j=0;
	bool 	Is_All_0_485_Addr;//485��ַȫ��
	bool		Is_All_0_Asset_Number;//�ʲ����ȫ��

	temp_i = Process_Buf_Ptr[9] - 6;//�ʲ���ŵĸ�����6�ı���.����2���ֽڵ�id ,4���ֽڵ�����
	memset(&Meter_Asset_Number_Bak[0][0],0X00,192);
	memcpy(&Meter_Asset_Number_Bak[0][0],&Process_Buf_Ptr[16],temp_i);
	Global_Task_Flag |=TASK_FLAG_SET_ASSET_NUMBER_9213;
	
	if(Global_Task_Flag & TASK_FLAG_SET_485_ADDRESS_7213)
	{
		//�����жϴ��͹�����485��ַ���ʲ������û������ֻ��һ����ȫ������������еĻ�������
		Base_ParaMeter_Bak.Meter_Total_Number = 0;
                Meter_Valid_Flag_Bak=METER_NOT_VALID;
		for(temp_i=0;temp_i<32;temp_i++)
		{
			Is_All_0_485_Addr =TRUE;
			Is_All_0_Asset_Number = TRUE;
			for(temp_j=0;temp_j<6;temp_j++)
			{
				if(Meter_485_Addr_Bak[temp_i][temp_j]!=0)
				{
					Is_All_0_485_Addr = FALSE;
				}
				if(Meter_Asset_Number_Bak[temp_i][temp_j] !=0)
				{
					Is_All_0_Asset_Number = FALSE;
				}
			}
			if(Is_All_0_485_Addr==FALSE&& Is_All_0_Asset_Number==false)
			{
				//�ñ����Ч
				Base_ParaMeter_Bak.Meter_Total_Number ++;
				Meter_Valid_Flag_Bak= METER_IS_VALID;
				if(Base_ParaMeter_Bak.Meter_Total_Number-1>0)
				{           
                                     memset(&Meter_485_Addr_Bak[temp_i][0],0X00,6);
				         memset(&Meter_Asset_Number_Bak[temp_i][0],0X00,6);
                                     break;
				}
  
                            Meter_Valid_Flag=Meter_Valid_Flag_Bak;
			       SPI_Write_161d(BASE_ADDR_VALID_FLAG,(u8*)(&Meter_Valid_Flag),sizeof(Meter_Valid_Flag));
		              Base_ParaMeter.Meter_Total_Number  = Base_ParaMeter_Bak.Meter_Total_Number;
				SPI_Write_161d(BASE_ADDR_BASE_PARA,(u8*)&Base_ParaMeter,sizeof(Base_ParaMeter));

				memcpy(&Meter_485_Addr[0],&Meter_485_Addr_Bak[temp_i][0],6);
		              SPI_Write_161d(BASE_ADDR_485_ADDR,&Meter_485_Addr[0],6);
			
                           	memcpy(&Meter_Asset_Number[0],&Meter_Asset_Number_Bak[temp_i][0],6);
		              SPI_Write_161d(BASE_ADDR_ASSET_NUMBER,&Meter_Asset_Number[0],6);
				
			}
			
		}						
	
		LunXun_Error_Flag =0;
		//������Ͽ�ʼ���߲ɼ�
		Global_Task_Flag |= TASK_FLAG_BEGIN_LUNXUN;
		Global_Task_Flag &= ~TASK_FLAG_LUNXUN_ING;	//���������ѭ��־
		Global_Task_Flag &= ~(TASK_FLAG_SET_ASSET_NUMBER_9213|TASK_FLAG_SET_485_ADDRESS_7213);
	}
	
	return 1;
}


/*******************************************************************************
* Function Name              : Delete_Meter_485_Asset_Addr
* Description                   : ɾ��485��ַ���ʲ����
* inputpara                     :���뻺����
* Return                          : u8
*******************************************************************************/
u8 Delete_Meter_485_Asset_Addr(u8* Process_Buf_Ptr)
{
	bool 	Is_All_0_485_Addr;//485��ַȫ��
	u8		temp_i=0;
		
	//�����485��ַ��ȫ����
	Is_All_0_485_Addr =TRUE;
	for(temp_i=0;temp_i<6;temp_i++)
	{
		//485��ַȫ��?
		if(Process_Buf_Ptr[16+temp_i]!=0)
		{
			Is_All_0_485_Addr = FALSE;
		}
	}
	//485��ַΪ���򱨴�
	if(Is_All_0_485_Addr)
	{
		return 0;
	}	
	
	//�ҵ���485��ַ��flash���Ƿ����,���ж��Ƿ���Ч
	if(Meter_Valid_Flag==METER_IS_VALID&&memcmp(&Meter_485_Addr_Bak[0],&Process_Buf_Ptr[16],6)==0)
	{
	
		LunXun_Error_Flag = 0;	//�����Ӧλ���ϵĴ����־
		Meter_Valid_Flag= METER_NOT_VALID;	//����ñ����Ч��־
		memset(&Meter_485_Addr[0],0xFF,6);
		memset(&Meter_Asset_Number[0],0xFF,6);
		memcpy((u8*)&Base_ParaMeter_Bak,(u8*)&Base_ParaMeter,sizeof(Base_ParaMeter));
		Base_ParaMeter_Bak.Meter_Total_Number-=1;
		memcpy((u8*)&Base_ParaMeter,(u8*)&Base_ParaMeter_Bak,sizeof(Base_ParaMeter));//�޸Ļ�������

		SPI_Write_161d(BASE_ADDR_485_ADDR,&Meter_485_Addr[0],6);//����485��ַ��Flash
		SPI_Write_161d(BASE_ADDR_ASSET_NUMBER,&Meter_Asset_Number[0],6);//�����ʲ���ŵ�Flash
		SPI_Write_161d(BASE_ADDR_VALID_FLAG,(u8*)(&Meter_Valid_Flag),1);/*������Ч��־��Flash*/
		SPI_Write_161d(BASE_ADDR_BASE_PARA,(u8*)&Base_ParaMeter,sizeof(Base_ParaMeter));/*�����޸ĵĻ�������*/
		return 1;
	}
	 return  0;
	
}



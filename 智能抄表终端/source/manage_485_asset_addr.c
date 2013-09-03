/*******************************************************************************
* File Name          : manage_485_asset_addr.c
* Description        : 处理485地址、资产编号的新设、增加、删除、替换
********************************************************************************/

#include <stdio.h>
#include <string.h>
#include "manage_485_asset_addr.h"


/*******************************************************************************
* Function Name              : Set_Meter_485_Addr
* Description                   : 新设485地址
* inputpara                     :输入缓冲区
* Return                          : u8
*******************************************************************************/
u8 Set_Meter_485_Addr(u8* Process_Buf_Ptr)
{
	u8	temp_i=0,temp_j=0;
	bool 	Is_All_0_485_Addr;//485地址全零
	bool	Is_All_0_Asset_Number;//资产编号全零
	temp_i = Process_Buf_Ptr[9]-6	;//485地址的长度  ,6的倍数. 包括2个字节的id ,4个字节的密码
	memset(&Meter_485_Addr_Bak[0][0],0X00,192);
	memcpy(&Meter_485_Addr_Bak[0][0],&Process_Buf_Ptr[16],temp_i);
	Global_Task_Flag|=TASK_FLAG_SET_485_ADDRESS_7213;
	if(Global_Task_Flag & TASK_FLAG_SET_ASSET_NUMBER_9213)
	{
            //首先判断传送过来的485地址和资产编号有没有其中只有一个是全零的情况，如果有的话，报错
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
              if(Is_All_0_485_Addr==FALSE&& Is_All_0_Asset_Number==false)//该表计有效
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
		//新设完毕开始轮徇采集
		Global_Task_Flag |= TASK_FLAG_BEGIN_LUNXUN;
              Global_Task_Flag &= ~TASK_FLAG_LUNXUN_ING;	//清除正在轮循标志
		Global_Task_Flag &= ~(TASK_FLAG_SET_ASSET_NUMBER_9213|TASK_FLAG_SET_485_ADDRESS_7213);		
	}


	return 1;
}

/*******************************************************************************
* Function Name              : Set_Meter_Asset_Addr
* Description                   : 新设资产编号
* inputpara                     : 输入缓冲区
* Return                          : u8
*******************************************************************************/
u8 Set_Meter_Asset_Addr(u8* Process_Buf_Ptr)
{
	u8		temp_i=0,temp_j=0;
	bool 	Is_All_0_485_Addr;//485地址全零
	bool		Is_All_0_Asset_Number;//资产编号全零

	temp_i = Process_Buf_Ptr[9] - 6;//资产编号的个数，6的倍数.包括2个字节的id ,4个字节的密码
	memset(&Meter_Asset_Number_Bak[0][0],0X00,192);
	memcpy(&Meter_Asset_Number_Bak[0][0],&Process_Buf_Ptr[16],temp_i);
	Global_Task_Flag |=TASK_FLAG_SET_ASSET_NUMBER_9213;
	
	if(Global_Task_Flag & TASK_FLAG_SET_485_ADDRESS_7213)
	{
		//首先判断传送过来的485地址和资产编号有没有其中只有一个是全零的情况，如果有的话，报错
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
				//该表计有效
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
		//新设完毕开始轮徇采集
		Global_Task_Flag |= TASK_FLAG_BEGIN_LUNXUN;
		Global_Task_Flag &= ~TASK_FLAG_LUNXUN_ING;	//清除正在轮循标志
		Global_Task_Flag &= ~(TASK_FLAG_SET_ASSET_NUMBER_9213|TASK_FLAG_SET_485_ADDRESS_7213);
	}
	
	return 1;
}


/*******************************************************************************
* Function Name              : Delete_Meter_485_Asset_Addr
* Description                   : 删除485地址、资产编号
* inputpara                     :输入缓冲区
* Return                          : u8
*******************************************************************************/
u8 Delete_Meter_485_Asset_Addr(u8* Process_Buf_Ptr)
{
	bool 	Is_All_0_485_Addr;//485地址全零
	u8		temp_i=0;
		
	//传入的485地址的全零检查
	Is_All_0_485_Addr =TRUE;
	for(temp_i=0;temp_i<6;temp_i++)
	{
		//485地址全零?
		if(Process_Buf_Ptr[16+temp_i]!=0)
		{
			Is_All_0_485_Addr = FALSE;
		}
	}
	//485地址为零则报错
	if(Is_All_0_485_Addr)
	{
		return 0;
	}	
	
	//找到该485地址在flash中是否存在,并判断是否有效
	if(Meter_Valid_Flag==METER_IS_VALID&&memcmp(&Meter_485_Addr_Bak[0],&Process_Buf_Ptr[16],6)==0)
	{
	
		LunXun_Error_Flag = 0;	//清除对应位置上的错误标志
		Meter_Valid_Flag= METER_NOT_VALID;	//清除该表的有效标志
		memset(&Meter_485_Addr[0],0xFF,6);
		memset(&Meter_Asset_Number[0],0xFF,6);
		memcpy((u8*)&Base_ParaMeter_Bak,(u8*)&Base_ParaMeter,sizeof(Base_ParaMeter));
		Base_ParaMeter_Bak.Meter_Total_Number-=1;
		memcpy((u8*)&Base_ParaMeter,(u8*)&Base_ParaMeter_Bak,sizeof(Base_ParaMeter));//修改基本参数

		SPI_Write_161d(BASE_ADDR_485_ADDR,&Meter_485_Addr[0],6);//保存485地址到Flash
		SPI_Write_161d(BASE_ADDR_ASSET_NUMBER,&Meter_Asset_Number[0],6);//保存资产编号到Flash
		SPI_Write_161d(BASE_ADDR_VALID_FLAG,(u8*)(&Meter_Valid_Flag),1);/*保存有效标志到Flash*/
		SPI_Write_161d(BASE_ADDR_BASE_PARA,(u8*)&Base_ParaMeter,sizeof(Base_ParaMeter));/*保存修改的基本参数*/
		return 1;
	}
	 return  0;
	
}



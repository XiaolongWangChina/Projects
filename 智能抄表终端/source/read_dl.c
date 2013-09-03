#include <stdio.h>
#include <string.h>
#include "read_dl.h"

/*******************************************************************************
* Function Name              : Read_Current_485_Dl
* Description                   : 读取当前电量
* inputpara                     :输入缓冲区
* outputpara                     :输出缓冲区
* Return                          : u8
*******************************************************************************/
u8 Read_Current_Dl(u8* Process_Buf_Ptr,u8* Send_Buf_Ptr)
{
        u8		temp_j=0;
        u8 		Len_Of_Data = Process_Buf_Ptr[9];
	switch(Len_Of_Data)
	{
  	    case DI_RD_ADDR_485://根据传入的485地址抄单表数据
                Send_Buf_Ptr[9] = 2 + 32;	//回传时的数据长度
      
	         if((Meter_Valid_Flag==METER_IS_VALID) && (memcmp(&Meter_485_Addr[0],&Process_Buf_Ptr[15],6)==0))
                {
		              memcpy(&Send_Buf_Ptr[12],(u8*)&Meter_Asset_Number[0],6);//传入资产号
                      memcpy(&Send_Buf_Ptr[18],(u8*)&Meter_32_Dl_And_Time,23);//传入电量数据、秒、分、时
                      memcpy(&Send_Buf_Ptr[41],(u8*)&Meter_32_Date,3);//日月年
                      
                      for(temp_j=0;temp_j<20;temp_j++)						
                      {
                            Send_Buf_Ptr[18+temp_j]-=0x33;
                      }
                      if(LunXun_Error_Flag==1)	//如果该表计上前没有采集到电量值
                      {//如果上次没有采集到电量值,则将电量部分设置为特殊值
                             memset(&Send_Buf_Ptr[18],CURRENT_DL_HAVE_NOT_COLLECTED,23);
                      }
                      break;
                }
	  default:	//异常情况，报错				
		return 0;
	}	
	return 1;
}

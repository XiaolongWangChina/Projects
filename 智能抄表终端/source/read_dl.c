#include <stdio.h>
#include <string.h>
#include "read_dl.h"

/*******************************************************************************
* Function Name              : Read_Current_485_Dl
* Description                   : ��ȡ��ǰ����
* inputpara                     :���뻺����
* outputpara                     :���������
* Return                          : u8
*******************************************************************************/
u8 Read_Current_Dl(u8* Process_Buf_Ptr,u8* Send_Buf_Ptr)
{
        u8		temp_j=0;
        u8 		Len_Of_Data = Process_Buf_Ptr[9];
	switch(Len_Of_Data)
	{
  	    case DI_RD_ADDR_485://���ݴ����485��ַ����������
                Send_Buf_Ptr[9] = 2 + 32;	//�ش�ʱ�����ݳ���
      
	         if((Meter_Valid_Flag==METER_IS_VALID) && (memcmp(&Meter_485_Addr[0],&Process_Buf_Ptr[15],6)==0))
                {
		              memcpy(&Send_Buf_Ptr[12],(u8*)&Meter_Asset_Number[0],6);//�����ʲ���
                      memcpy(&Send_Buf_Ptr[18],(u8*)&Meter_32_Dl_And_Time,23);//����������ݡ��롢�֡�ʱ
                      memcpy(&Send_Buf_Ptr[41],(u8*)&Meter_32_Date,3);//������
                      
                      for(temp_j=0;temp_j<20;temp_j++)						
                      {
                            Send_Buf_Ptr[18+temp_j]-=0x33;
                      }
                      if(LunXun_Error_Flag==1)	//����ñ����ǰû�вɼ�������ֵ
                      {//����ϴ�û�вɼ�������ֵ,�򽫵�����������Ϊ����ֵ
                             memset(&Send_Buf_Ptr[18],CURRENT_DL_HAVE_NOT_COLLECTED,23);
                      }
                      break;
                }
	  default:	//�쳣���������				
		return 0;
	}	
	return 1;
}

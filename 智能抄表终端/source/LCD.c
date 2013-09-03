#include <stdio.h>
#include <stdarg.h>
#include "LCD.h"

#define	Lcd_Delay(time) 	do{asm("nop");  asm("nop");  asm("nop");  asm("nop");  }while(0)
#define	USE_OLD_LCD_JICUI         //����������ʾ��
//#define	USE_NEW_LCD_JIYA	//�ӱ�������ʾ��

/***************************************************************************************
��������:   LCD_Init 
��������:   Lcd ��ʼ��
***************************************************************************************/
void LCD_Init()
{
	//��Ƭѡ�����������ѡ��ʱ�ӣ������߳�ʼ���䷽��
	GPIO_Config(GPIO_LCD_CS,	LCD_CS_BIT,		GPIO_OUT_PP);
	GPIO_Config(GPIO_LCD_A0,	LCD_A0_BIT,		GPIO_OUT_PP);
	GPIO_Config(GPIO_LCD_SCL,	LCD_SCL_BIT,	GPIO_OUT_PP);
	GPIO_Config(GPIO_LCD_SI,	LCD_SI_BIT,		GPIO_OUT_PP);
#ifdef	USE_OLD_LCD_JICUI
	LCD_Write_Command(0xAF);	// lcd display on 
	LCD_Write_Command(0x40);	// set start line address
	LCD_Write_Command(0xA1);	//ADC select ,reverse  131-->4
	LCD_Write_Command(0xA6);	// set ������ʾ
	LCD_Write_Command(0xA4);	//ȫ����ʾ����ģʽ
	LCD_Write_Command(0xA2);	// 1/9 bias
	LCD_Write_Command(0xC0);	// COMMON OUTPUT DIRECTION
	LCD_Write_Command(0x2F);	// POWER CONTROL
	LCD_Write_Command(0x24);	//RESISTER RATIO
	LCD_Write_Command(0x81);	// VOLUME MODE SET  
	LCD_Write_Command(0x24);	// RESISTER RATIO
#else
	LCD_Write_Command(0x40);	// set start line address
	LCD_Write_Command(0xA1);	//ADC Select    --->  Reverse
	LCD_Write_Command(0xA3);	//Bias    1/7
	LCD_Write_Command(0xA4);	//Display all point   ---> normal
	LCD_Write_Command(0xA6);	//Display normal    ---> normal		0 : ʹ��һ���㲻��  1: ʹ��һ�����
	LCD_Write_Command(0xC0);	//Common output mode select   ---> normal  direction

	LCD_Write_Command(0x2C);	
	Lcd_Delay(1);
	LCD_Write_Command(0x2E);	
	Lcd_Delay(1);
	LCD_Write_Command(0x2F);	
	Lcd_Delay(1);
	LCD_Write_Command(0x25);	
	Lcd_Delay(1);
        LCD_Write_Command(0x81);	
	Lcd_Delay(1);
        LCD_Write_Command(22);	
	Lcd_Delay(1); 
#endif
}
/***************************************************************************************
��������:   LCD_Write_Command
��������:   ��Һ����д����
����:	    һ���ֽڵ�����
***************************************************************************************/
void LCD_Write_Command(u8 lcd_command)
{
	u8  temp_ii;
	u8 command_temp;
	EIC_DISABLE;
	GPIO_LCD_A0->PD &= (~LCD_A0_BIT);	//������������,��ʾ��������
	Lcd_Delay(1);	//��ʱ
	GPIO_LCD_CS->PD &= (~LCD_CS_BIT);
	Lcd_Delay(1);
	for( temp_ii=0;temp_ii<8;temp_ii++)
	{
		command_temp = lcd_command<<temp_ii;
		GPIO_LCD_SCL->PD &= (~LCD_SCL_BIT);
		Lcd_Delay(1);
		if((command_temp&0x80) ==0x00)
		{
			GPIO_LCD_SI->PD &= (~ LCD_SI_BIT);
			Lcd_Delay(1);
		}
		else
		{
			GPIO_LCD_SI->PD |= (LCD_SI_BIT);
			Lcd_Delay(1);
		}
		GPIO_LCD_SCL->PD |= (LCD_SCL_BIT);
		Lcd_Delay(1);
	}
	GPIO_LCD_CS->PD |= (LCD_CS_BIT);
	Lcd_Delay(1);
	EIC_ENABLE;
}
/***************************************************************************************
��������:   LCD_Write_Data
��������:   ��Һ����д����
����:       һ���ֽڵ�����
***************************************************************************************/
void LCD_Write_Data(u8 lcd_data)
{
	u8  	temp_ii;
	u8 	command_temp;
	EIC_DISABLE;
	GPIO_LCD_A0->PD  |= LCD_A0_BIT;	//������������,��ʾ��������
	Lcd_Delay(1);	//��ʱ
	GPIO_LCD_CS->PD &= (~LCD_CS_BIT);
	Lcd_Delay(1);
	for( temp_ii=0;temp_ii<8;temp_ii++)
	{
		command_temp = lcd_data<<temp_ii;
		GPIO_LCD_SCL->PD &= (~LCD_SCL_BIT);
		Lcd_Delay(1);
		if((command_temp&0x80)==0X80)
		{
			GPIO_LCD_SI->PD |= LCD_SI_BIT;
			Lcd_Delay(1);
		}
		else
		{
			GPIO_LCD_SI->PD &= (~LCD_SI_BIT);
			Lcd_Delay(1);
		}
		GPIO_LCD_SCL->PD  |=(LCD_SCL_BIT);
		Lcd_Delay(1);
	}
	GPIO_LCD_CS->PD |= LCD_CS_BIT;
	Lcd_Delay(1);
	EIC_ENABLE;
}
/*****************************************************************************************
��������:   LCD_Show
��������:   ��LCD��ʾ��������������(ָ���ڴ������е�����)
����:       lcd ������
*****************************************************************************************/
u32	lcd_show_delay;
void	LCD_Show(u8 * lcd_buf_ptr)
{
    	u8 temp,page,column;
	
	for(page=0;page<8;page++)
	{
		WDG_CntRefresh();				//ˢ�¿��Ź��ļ�����ֵ
		LCD_Write_Command(page +0XB0);	//дҳ��ַ
		//�����е�ַ
		LCD_Write_Command(0x10);	//�����е�ַmsb	
#ifdef USE_OLD_LCD_JICUI
		LCD_Write_Command(0x04);	//�����е�ַlsb
#else		
		LCD_Write_Command(0x00);	//�����е�ַlsb
#endif		
		for(column = 0;column<128;column++)
		{
			temp = lcd_buf_ptr[page*128+column];
			LCD_Write_Data(temp);
		}
	}
}
/*****************************************************************************************
��������:   LCD_Show_Time
��������:   ����ѭ����ʾ��ʱ�䡣
*����:      lcd ������
*****************************************************************************************/
void	LCD_Show_Time(u8 * lcd_buf_ptr)
{
    u8 temp,page,column , i;
	for(page=6;page<8;page++)
	{
		LCD_Write_Command(page +0XB0);	//дҳ��ַ
		//�����е�ַ.  ʱ�����������ֵ��׵�ַ�ֱ�Ϊ  ��32(0x20)��. ��56(0x38)��. ��80(0x50)��

		for(i = 0; i<3;i++)		//�����ü��飬��֤���óɹ�
		{
				//�����е�ַ����32��
				LCD_Write_Command(0x12);	//�����е�ַmsb	
#ifdef	USE_OLD_LCD_JICUI
				LCD_Write_Command(0x04);	//�����е�ַlsb
#else
				LCD_Write_Command(0x00);	//�����е�ַlsb
#endif
		}
		for(column = 32;column<48;column++)
		{
			temp = lcd_buf_ptr[page*128+column];
			LCD_Write_Data(temp);
		}

		for(i = 0; i<3;i++)		//�����ü��飬��֤���óɹ�
		{
				//�����е�ַ����56��
				LCD_Write_Command(0x13);	//�����е�ַmsb	
#ifdef	USE_OLD_LCD_JICUI
				LCD_Write_Command(0x0C);	//�����е�ַlsb
#else
				LCD_Write_Command(0x08);	//�����е�ַlsb
#endif		
		}			
		for(column = 56;column<72;column++)
		{
			temp = lcd_buf_ptr[page*128+column];
			LCD_Write_Data(temp);
		}
		for(i = 0; i<3;i++)		//�����ü��飬��֤���óɹ�
		{
				//�����е�ַ����80��
				LCD_Write_Command(0x15);	//�����е�ַmsb	
#ifdef	USE_OLD_LCD_JICUI
				LCD_Write_Command(0x04);	//�����е�ַlsb
#else
		 		LCD_Write_Command(0x00);	//�����е�ַlsb
#endif
		}
		for(column = 80;column<104;column++)
		{
			temp = lcd_buf_ptr[page*128+column];
			LCD_Write_Data(temp);
		}
	}
}
/***************************************************************************************
��������:   Lcd_Display_XunXian
��������:   ��ʾ�����������
***************************************************************************************/
void 	Lcd_Display_XunXian()
{
	u8	Year_Mon_Day_tmp[3];
	u8	Hour_Min_Second_tmp[3];

	while(1)
	{
		switch(Lcd_Display_Meter)
		{
			case	1:
				displayfirst(2);	//��ʾ��һ��
				return;
			case	2:
				displaysecond(2);	//��ʾ�ڶ���
				return;
			default:
				while(1)
				{

				    if(Lcd_Display_Meter>3 || Lcd_Display_Meter<=2)
					{
						Lcd_Display_Meter  = 1;
						Lcd_Display_Frame = 1;

                                          break;
					}
					if(Meter_Valid_Flag== METER_IS_VALID)
					{//��ʾ����
						Year_Mon_Day_tmp[0] = (ptim.tm_year)%100;
						Year_Mon_Day_tmp[0] =  Dec_2_BCD(Year_Mon_Day_tmp[0]);				
						Year_Mon_Day_tmp[1] = Dec_2_BCD(ptim.tm_mon+1);
						Year_Mon_Day_tmp[2] = Dec_2_BCD(ptim.tm_mday);

						Hour_Min_Second_tmp[0]  = Dec_2_BCD(ptim.tm_hour);
						Hour_Min_Second_tmp[1]  = Dec_2_BCD(ptim.tm_min);
						Hour_Min_Second_tmp[2]  = Dec_2_BCD(ptim.tm_sec);
						
						displaythird(Lcd_Display_Frame-2,(u8*)(Meter_32_Dl_And_Time.Current_DL),Year_Mon_Day_tmp, Hour_Min_Second_tmp);
                                      
						return;
					}
					if(Global_Task_Flag & TASK_FLAG_KEY_UP_PRESSED)	
					{	
						Lcd_Display_Meter--;
						break;		//������while(1)ѭ��. �������ѭ����������
					}
					Lcd_Display_Meter++;	//����ѭ��ʱ,�����ǰλ�ò�����Ч���,������ж���һ���

				}
				break;
		}
	}	
}
/***************************************************************************************
��������: Process_XunXian()
��������: ����ѭ����ʾ
***************************************************************************************/


extern u8  echo_Buff[];
extern u32 echo_Counter;

#define MY_BASE 0X4000F000

u8 current_colum_count = 0;

void Process_XunXian()
{
  /*
  // our function : can display ascii.
  u8 usr_char;
  u8 index = 0;
  
  for(index = 0; index < echo_Counter; index++ ){
     usr_char = echo_Buff[ index ];
     find_asc( MY_BASE + (usr_char - 32)*16 );
     
     write_lcd_buf_ascii( 2*(current_colum_count/16) , 8*(current_colum_count%16));
     current_colum_count ++;
     if(current_colum_count > 64)
       current_colum_count = 0;
  }
  
  echo_Counter = 0;
  
  LCD_Show(lcd_buf);
  */


  u32 pictrue_address = MY_BASE + (128 - 32)*16;
  u32 i;
  u32 temp_u32;
  for(i=0;i<256;i++)
  {
    temp_u32 = FLASH_WordRead(pictrue_address + i*4);
    lcd_buf[ i*4 ]   = temp_u32 & 0x000000FF;
    lcd_buf[ i*4+1 ] = (temp_u32>>8) & 0x000000FF;
    lcd_buf[ i*4+2 ] = (temp_u32>>16) & 0x000000FF;
    lcd_buf[ i*4+3 ] = (temp_u32>>24) & 0x000000FF;
  }

  LCD_Show(lcd_buf);
  
}











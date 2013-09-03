#include <stdio.h>
#include <stdarg.h>
#include "LCD.h"

#define	Lcd_Delay(time) 	do{asm("nop");  asm("nop");  asm("nop");  asm("nop");  }while(0)
#define	USE_OLD_LCD_JICUI         //北京集粹显示屏
//#define	USE_NEW_LCD_JIYA	//河北冀雅显示屏

/***************************************************************************************
函数名称:   LCD_Init 
功能描述:   Lcd 初始化
***************************************************************************************/
void LCD_Init()
{
	//将片选，命令或数据选择，时钟，数据线初始化其方向
	GPIO_Config(GPIO_LCD_CS,	LCD_CS_BIT,		GPIO_OUT_PP);
	GPIO_Config(GPIO_LCD_A0,	LCD_A0_BIT,		GPIO_OUT_PP);
	GPIO_Config(GPIO_LCD_SCL,	LCD_SCL_BIT,	GPIO_OUT_PP);
	GPIO_Config(GPIO_LCD_SI,	LCD_SI_BIT,		GPIO_OUT_PP);
#ifdef	USE_OLD_LCD_JICUI
	LCD_Write_Command(0xAF);	// lcd display on 
	LCD_Write_Command(0x40);	// set start line address
	LCD_Write_Command(0xA1);	//ADC select ,reverse  131-->4
	LCD_Write_Command(0xA6);	// set 正常显示
	LCD_Write_Command(0xA4);	//全屏显示正常模式
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
	LCD_Write_Command(0xA6);	//Display normal    ---> normal		0 : 使得一个点不黑  1: 使得一个点黑
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
函数名称:   LCD_Write_Command
功能描述:   向液晶屏写命令
参数:	    一个字节的命令
***************************************************************************************/
void LCD_Write_Command(u8 lcd_command)
{
	u8  temp_ii;
	u8 command_temp;
	EIC_DISABLE;
	GPIO_LCD_A0->PD &= (~LCD_A0_BIT);	//命令类型拉低,表示发送命令
	Lcd_Delay(1);	//延时
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
函数名称:   LCD_Write_Data
功能描述:   向液晶屏写数据
参数:       一个字节的数据
***************************************************************************************/
void LCD_Write_Data(u8 lcd_data)
{
	u8  	temp_ii;
	u8 	command_temp;
	EIC_DISABLE;
	GPIO_LCD_A0->PD  |= LCD_A0_BIT;	//命令类型拉高,表示发送数据
	Lcd_Delay(1);	//延时
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
函数名称:   LCD_Show
功能描述:   向LCD显示屏送整屏的数据(指定内存区域中的内容)
参数:       lcd 缓冲区
*****************************************************************************************/
u32	lcd_show_delay;
void	LCD_Show(u8 * lcd_buf_ptr)
{
    	u8 temp,page,column;
	
	for(page=0;page<8;page++)
	{
		WDG_CntRefresh();				//刷新看门狗的计数器值
		LCD_Write_Command(page +0XB0);	//写页地址
		//设置列地址
		LCD_Write_Command(0x10);	//设置列地址msb	
#ifdef USE_OLD_LCD_JICUI
		LCD_Write_Command(0x04);	//设置列地址lsb
#else		
		LCD_Write_Command(0x00);	//设置列地址lsb
#endif		
		for(column = 0;column<128;column++)
		{
			temp = lcd_buf_ptr[page*128+column];
			LCD_Write_Data(temp);
		}
	}
}
/*****************************************************************************************
函数名称:   LCD_Show_Time
功能描述:   处理循环显示的时间。
*参数:      lcd 缓冲区
*****************************************************************************************/
void	LCD_Show_Time(u8 * lcd_buf_ptr)
{
    u8 temp,page,column , i;
	for(page=6;page<8;page++)
	{
		LCD_Write_Command(page +0XB0);	//写页地址
		//设置列地址.  时分秒三个数字的首地址分别为  第32(0x20)列. 第56(0x38)列. 第80(0x50)列

		for(i = 0; i<3;i++)		//多设置几遍，保证设置成功
		{
				//设置列地址到第32列
				LCD_Write_Command(0x12);	//设置列地址msb	
#ifdef	USE_OLD_LCD_JICUI
				LCD_Write_Command(0x04);	//设置列地址lsb
#else
				LCD_Write_Command(0x00);	//设置列地址lsb
#endif
		}
		for(column = 32;column<48;column++)
		{
			temp = lcd_buf_ptr[page*128+column];
			LCD_Write_Data(temp);
		}

		for(i = 0; i<3;i++)		//多设置几遍，保证设置成功
		{
				//设置列地址到第56列
				LCD_Write_Command(0x13);	//设置列地址msb	
#ifdef	USE_OLD_LCD_JICUI
				LCD_Write_Command(0x0C);	//设置列地址lsb
#else
				LCD_Write_Command(0x08);	//设置列地址lsb
#endif		
		}			
		for(column = 56;column<72;column++)
		{
			temp = lcd_buf_ptr[page*128+column];
			LCD_Write_Data(temp);
		}
		for(i = 0; i<3;i++)		//多设置几遍，保证设置成功
		{
				//设置列地址到第80列
				LCD_Write_Command(0x15);	//设置列地址msb	
#ifdef	USE_OLD_LCD_JICUI
				LCD_Write_Command(0x04);	//设置列地址lsb
#else
		 		LCD_Write_Command(0x00);	//设置列地址lsb
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
函数名称:   Lcd_Display_XunXian
功能描述:   显示具体的屏数据
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
				displayfirst(2);	//显示第一屏
				return;
			case	2:
				displaysecond(2);	//显示第二屏
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
					{//显示电量
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
						break;		//跳出本while(1)循环. 由外面的循环再来处理
					}
					Lcd_Display_Meter++;	//正常循显时,如果当前位置不是有效表计,则继续判断下一块表

				}
				break;
		}
	}	
}
/***************************************************************************************
函数名称: Process_XunXian()
功能描述: 处理循环显示
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











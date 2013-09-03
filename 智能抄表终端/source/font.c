#include "font.h"
#include <stdio.h>
#include <stdarg.h>

#define	SHOW_0			0
#define	SHOW_SPACE		1

/*******************************************************************
函数名称:   find_chinese
功能描述:   在中文字库中寻找中文字符
*******************************************************************/
void find_chinese(u32 address)
{
 	u8 i;
	u32 tmp[8];
        
	for(i=0;i<8;i++)
	{
		tmp[i]=FLASH_WordRead(address+i*4);
		chinese_buf[i*4]=tmp[i]&0x000000FF;
		chinese_buf[i*4+1]=(tmp[i]>>8)&0x000000FF;
		chinese_buf[i*4+2]=(tmp[i]>>16)&0x000000FF;
		chinese_buf[i*4+3]=(tmp[i]>>24)&0x000000FF;
	}
}

/*******************************************************************
函数名称:find_chinese
功能描述:在ASCII库中寻找ASCII码
*******************************************************************/
void find_asc(u32 address)
{
  	u8 i;
  	u32 tmp[4];
 
	for(i=0;i<4;i++)
	{
		tmp[i]=FLASH_WordRead(address+i*4);
		asc_buf[i*4]=tmp[i]&0x000000FF;
		asc_buf[i*4+1]=(tmp[i]>>8)&0x000000FF;
		asc_buf[i*4+2]=(tmp[i]>>16)&0x000000FF;
		asc_buf[i*4+3]=(tmp[i]>>24)&0x000000FF;
	}
}

/*******************************************************************
函数名称:   write_lcd_buf_chinese
功能描述:   向LCD显示屏上写一个中文字符
*******************************************************************/
void write_lcd_buf_chinese(u8 page,u8 col)
{
	u8 i;
	for(i=0;i<16;i++)
	{
		lcd_buf[page*128+col+i]=chinese_buf[i];
		lcd_buf[(page+1)*128+col+i]=chinese_buf[i+16];
	}
}

/*******************************************************************
函数名称:   write_lcd_buf_ascii
功能描述:   向LCD显示屏上写一个ASCII码
*******************************************************************/
void write_lcd_buf_ascii(u8 page,u8 col)
{
	u8 i;

	for(i=0;i<8;i++)
	{
		lcd_buf[page*128+col+i]=asc_buf[i];
		lcd_buf[(page+1)*128+col+i]=asc_buf[i+8];
	}
}

/*******************************************************************
函数名称:   displayfirst
功能描述:   显示第一屏数据
参数:       lcd上的行显示位置
*******************************************************************/
void displayfirst(u8 lcdrow)
{
    memset(lcd_buf,0x00,1024);
    u8 Version[3] = {0x01 , 0x03, 0x08 };	//程序版本号
    
    /*****低压电量采集器********************/
    find_asc(0x4000F000);		/* -        - */
    write_lcd_buf_ascii(0,0);
    find_chinese(0x4000F6C0);	/* -    低  - */
    write_lcd_buf_chinese(0,8);
    find_chinese(0x4000F6E0);	/* -    压  - */
    write_lcd_buf_chinese(0,24);
    find_chinese(0x4000F7E0);	/* -    电  - */
    write_lcd_buf_chinese(0,40);
    find_chinese(0x4000F800);	/* -    量  - */
    write_lcd_buf_chinese(0,56);
    find_chinese(0x4000F700);	/* -    采  - */
    write_lcd_buf_chinese(0,72);
    find_chinese(0x4000F720);	/* -    集  - */
    write_lcd_buf_chinese(0,88);
    find_chinese(0x4000F740);	/* -    器  - */
    write_lcd_buf_chinese(0,104);
    find_asc(0x4000F000);		/* -            - */
    write_lcd_buf_ascii(3,120);
     
    /*DW710C**************************/
    find_asc(0x4000F000);		/* -  	- */
    write_lcd_buf_ascii(3,0);
    find_asc(0x4000F000);		/* -  	- */
    write_lcd_buf_ascii(3,8);
    find_asc(0x4000F000);		/* -  	- */
    write_lcd_buf_ascii(3,16);
    find_asc(0x4000F000);		/* -  	- */
    write_lcd_buf_ascii(3,24);
    find_asc(0x4000F000);		/* -  	- */
    write_lcd_buf_ascii(3,32);
    find_asc(0x4000F240);		/* - D     - */
    write_lcd_buf_ascii(3,40);
    find_asc(0x4000F370);		/* - W     - */
    write_lcd_buf_ascii(3,48);
    find_asc(0x4000F170);		/* - 7     - */
    write_lcd_buf_ascii(3,56);
    find_asc(0x4000F110);		/* - 1     - */
    write_lcd_buf_ascii(3,64);
    find_asc(0x4000F100);		/* - 0     - */
    write_lcd_buf_ascii(3,72);
    find_asc(0x4000F230);		/* - C     - */
    write_lcd_buf_ascii(3,80);
    find_asc(0x4000F000);		/* -  	- */
    write_lcd_buf_ascii(3,88);
    find_asc(0x4000F000);		/* -        - */
    write_lcd_buf_ascii(3,96);
    find_asc(0x4000F000);		/* -        - */
    write_lcd_buf_ascii(3,104);
    find_asc(0x4000F000);		/* -        - */
    write_lcd_buf_ascii(3,112);
    find_asc(0x4000F000);		/* -        - */
    write_lcd_buf_ascii(3,120);
    
    /*********Ver 1.3.1*****************/
    find_asc(0x4000F000);		/* -            - */
    write_lcd_buf_ascii(6,8);
    find_asc(0x4000F000);		/* -            - */
    write_lcd_buf_ascii(6,16);
    find_asc(0x4000F000);		/* -            - */
    write_lcd_buf_ascii(6,24);
    find_asc(0x4000F360);		/* -    V     - */
    write_lcd_buf_ascii(6,32);
    find_asc(0x4000F450);		/* -    e      - */
    write_lcd_buf_ascii(6,40);
    find_asc(0x4000F520);		/* -    r      - */
    write_lcd_buf_ascii(6,48);
    find_asc(0x4000F000);		/* -           - */
    write_lcd_buf_ascii(6,56);
     
    find_asc(0x4000F100 + ASCII_SIZE *Version[0]);		/* -    Version[0]       - */
    write_lcd_buf_ascii(6,64);
    find_asc(0x4000F0E0);		/* -     .      - */
    write_lcd_buf_ascii(6,72);
     
    find_asc(0x4000F100 + ASCII_SIZE *Version[1]);		/* -    Version[1]       - */
    write_lcd_buf_ascii(6,80);
    find_asc(0x4000F0E0);		/* -      .     - */
    write_lcd_buf_ascii(6,88);
    find_asc(0x4000F100 + ASCII_SIZE* Version[2]);		/* -    Version[2]       - */
    write_lcd_buf_ascii(6,96);
    
    find_asc(0x4000F000);		/* -            - */
    write_lcd_buf_ascii(6,120);
    
    LCD_Show(lcd_buf);	
}
/*******************************************************************
函数名称:   displaysecond
功能描述:   显示第二屏数据
参数:       lcd上的行显示位置
*******************************************************************/
void displaysecond(u8 lcdrow)
{
    u8 u8_temp;
    u8 Addr_485;
    memset(lcd_buf,0x00,1024);
    /***********地址000000000000***********************/
    find_chinese(0x4000F600);	/* -      地      - */
    write_lcd_buf_chinese(0,0);
    find_chinese(0x4000F820);	/* -      址      - */
    write_lcd_buf_chinese(0,16);
    
    for(u8_temp=0;u8_temp<6;u8_temp++)
    {	//数字在字库中的起始位置为 0x4000F100
      Addr_485 = Base_ParaMeter.CJQ_485_Addr[5-u8_temp];
    
      if(((Addr_485>>4)&0x0F) >=0x0A)	
      {//如果显示0X0A -- 0X0F 
              find_asc(0x4000F210 + (((Addr_485>>4)&0x0F)-0x0A)*16);
      }
      else
      {//如果显示0X00 -- 0X09 
              find_asc(0x4000F100 + ((Addr_485>>4)&0x0F)*16);
      }
     write_lcd_buf_ascii(0,32+(u8_temp*2)*8);
    
      if((Addr_485&0x0F) >=0x0A)
      {//如果显示0X0A -- 0X0F
              find_asc(0x4000F210 + ((Addr_485&0x0F)-0x0A)*16);
      }
      else
      {//如果显示0X00 -- 0X09
              find_asc(0x4000F100 + (Addr_485&0x0F)*16);
      }
     write_lcd_buf_ascii(0,32+(u8_temp*2 + 1)*8);
    }
    /*************************冻结日（1－28）: 27**********/  
    find_chinese(0x4000F840);	/* -      冻      - */
    write_lcd_buf_chinese(2,0);
    find_chinese(0x4000F860);	/* -      结      - */
    write_lcd_buf_chinese(2,16);
    find_chinese(0x4000F880);	/* -      日      - */
    write_lcd_buf_chinese(2,32);
    find_asc(0x4000F080);		/* -      (      - */
    write_lcd_buf_ascii(2,48);
    find_asc(0x4000F110);		/* -      1      - */ 
    write_lcd_buf_ascii(2,56);
    find_asc(0x4000F0D0);		/* -      -      - */ 		
    write_lcd_buf_ascii(2,64);
    find_asc(0x4000F120);		/* -      2      - */ 
    write_lcd_buf_ascii(2,72);
    find_asc(0x4000F180);		/* -      8      - */ 
    write_lcd_buf_ascii(2,80);
    find_asc(0x4000F090);		/* -      )      - */ 
    write_lcd_buf_ascii(2,88);
    find_asc(0x4000F1A0);		/* -      :      - */ 
    write_lcd_buf_ascii(2,96);
    find_asc(0x4000F000);		/* -            - */ 
    write_lcd_buf_ascii(2,104);
    
    u8_temp=DL_Freeze_Day_Global/10;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(2,112);		//显示冻结日的十位 
    u8_temp=DL_Freeze_Day_Global%10;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(2,120);		//显示冻结日的个位 
    
    /*******************采集间隔（M):0050************/
    find_chinese(0x4000F700);	/* -       采     - */ 
    write_lcd_buf_chinese(4,0);
    find_chinese(0x4000F720);	/* -       集     - */ 
    write_lcd_buf_chinese(4,16);
    find_chinese(0x4000F8A0);	/* -       间     - */ 
    write_lcd_buf_chinese(4,32);
    find_chinese(0x4000F8C0);	/* -       隔     - */ 
    write_lcd_buf_chinese(4,48);
    find_asc(0x4000F080);		/* -       (         - */ 
    write_lcd_buf_ascii(4,64);
    find_asc(0x4000F2D0);		/* -       M        - */ 
    write_lcd_buf_ascii(4,72);
    find_asc(0x4000F090);		/* -       )        - */ 
    write_lcd_buf_ascii(4,80);
    find_asc(0x4000F1A0);		/* -       :        - */ 
    write_lcd_buf_ascii(4,88);
    
    // 循环采集间隔的 时间 4 个 BCD 码 
    u8_temp=InterVal_For_Cycle_Collectin_Global/1000;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(4,96);		//显示采集间隔的千位 
    u8_temp=(InterVal_For_Cycle_Collectin_Global%1000)/100;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(4,104);		//显示采集间隔的百位
    u8_temp=(InterVal_For_Cycle_Collectin_Global%100)/10;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(4,112);		//显示采集间隔的十位
    u8_temp=InterVal_For_Cycle_Collectin_Global%10;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(4,120);		//显示采集间隔的个位
    
    /********************共 03 表**************************************/
    find_asc(0x4000F000);			/* -                 - */ 
    write_lcd_buf_ascii(6,0);	
    find_asc(0x4000F000);			/* -                 - */ 
    write_lcd_buf_ascii(6,8);
    find_asc(0x4000F000);			/* -                 - */ 
    write_lcd_buf_ascii(6,16);
    find_asc(0x4000F000);			/* -                 - */ 
    write_lcd_buf_ascii(6,24);
    find_chinese(0x4000F780);		/* -      共      - */ 
    write_lcd_buf_chinese(6,32);
    find_asc(0x4000F000);			/* -                  - */ 
    write_lcd_buf_ascii(6,48);
    
    //显示总表数 
    u8_temp=(Base_ParaMeter.Meter_Total_Number)/10;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(6,56);		//显示总表数的十位 
    u8_temp =(Base_ParaMeter.Meter_Total_Number)%10;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(6,64);	   //显示总表数的个位 
    
    find_asc(0x4000F000);			/* -      	           - */ 
    write_lcd_buf_ascii(6,72);
    find_chinese(0x4000F7A0);		/* -       表     - */ 
    write_lcd_buf_chinese(6,80);
    find_asc(0x4000F000);			/* -                - */ 
    write_lcd_buf_ascii(6,96);
    find_asc(0x4000F000);			/* -                - */ 
    write_lcd_buf_ascii(6,104);
    find_asc(0x4000F000);			/* -               - */ 
    write_lcd_buf_ascii(6,112);
    LCD_Show(lcd_buf);

}
/*******************************************************************
函数名称:   displaythird
功能描述:   显示第x块电能表的数据
参数:       Meter_Number: 表号
            elec:         电量
            date:         日期
            time:         时间
*******************************************************************/
void displaythird(u16 Meter_Number,u8 *elec,u8 *date,u8 *time)
{
	memset(lcd_buf,0x00,1024);
 	/*XX表0000000000XX*/
  	u8 i;
  	u16 tmp[2];
  	u32 addr;//偏移地址
  	u8 electmp;
  	//在显示电量时,在显示第一个非零值之前的零都应该显示为空格
  	bool	Show_0_or_Space = (bool)SHOW_SPACE;	//默认显示空格	. 
  	tmp[0]=Meter_Number/10;
  	tmp[1]=Meter_Number%10;

  	addr=0x4000F100+tmp[0]*16;
  	find_asc(addr);				/*  -- 表号的高位 --  */
  	write_lcd_buf_ascii(0,0);
  	addr=0x4000F100+tmp[1]*16;
  	find_asc(addr);				/*  -- 表号的低位 --  */
  	write_lcd_buf_ascii(0,8);
  	find_chinese(0x4000F7A0);	/*  -- 表--  */
  	write_lcd_buf_chinese(0,16);

  	for(i=0;i<6;i++)
  	{

	  	tmp[1]=Meter_Asset_Number[5-i]&0x0F;
	  	tmp[0]=(Meter_Asset_Number[5-i]>>4)&0x0F;

	  	if(tmp[0]>=0x0A)
  		{
	  		find_asc(0x4000F210 + (tmp[0]-0x0A)*16 );		/*  -- 资产编号的高位 --  */
  		}
		else
		{
	  		find_asc(0x4000F100 + tmp[0]*16 );				/*  -- 资产编号的高位 --  */
		}
	  	write_lcd_buf_ascii(0,32+(2*i)*8);					/*  -- 写资产编号的高位 --  */


	  	if(tmp[1]>=0x0A)
  		{
	  		find_asc(0x4000F210 + (tmp[1]-0x0A)*16 );		/*  -- 资产编号的高位 --  */
  		}
		else
		{
	  		find_asc(0x4000F100 + tmp[1]*16 );				/*  -- 资产编号的高位 --  */
		}
		
	  	write_lcd_buf_ascii(0,32+(2*i+1)*8);
  	}

 	/****************总电量：EEEEEEEEE*********/
  	find_chinese(0x4000F760);		/*  --总--  */
  	write_lcd_buf_chinese(2,0);
  	find_chinese(0x4000F7E0);		/*  --电--  */
  	write_lcd_buf_chinese(2,16); 
  	find_chinese(0x4000F800);		/*  --量--  */
  	write_lcd_buf_chinese(2,32);
  	find_asc(0x4000F1A0);			/*  --:	--  */
  	write_lcd_buf_ascii(2,48);
	
  	if(LunXun_Error_Flag==1)
  	{
    		for(i=0;i<9;i++)
	    	{
	      		find_asc(0x4000F250);			/*  --E--  */
	      		write_lcd_buf_ascii(2,56+i*8);
	    	}
  	}
  	else
  	{
   		for(i=0;i<3;i++)
   		{
   			/*  --电量的整数部分第 i 个值的高位--  */
    			electmp=((elec[3-i]-0x33)>>4)&0x0F;	
   			if(electmp>0x09)
	   		{
	   			electmp = 0x00;
	   		}
			
			if(Show_0_or_Space==SHOW_SPACE && electmp==0x00)
			{	//在还没遇到非零数字的时候,则如果本字符是0,则显示空格
			        find_asc(0x4000F000);		/* -  	- */
			    	 write_lcd_buf_ascii(2,56+i*16);
			}
			else	//在此之前已经遇到过非零字符或者本字符为非零字符,   则如果是0,则显示0
			{
				Show_0_or_Space = (bool)SHOW_0;
				addr=0x4000F100+electmp*16;
			    	find_asc(addr);					/*  --电量的整数部分的第 i 个值的高位--  */
			    	write_lcd_buf_ascii(2,56+i*16);
	 		}

    			/*  --电量的整数部分第 i 个值的低位--  */
    			electmp=(elec[3-i]-0x33)&0x0F;	
	   		if(electmp>0x09)
	   		{
	   			electmp = 0x00;
	   		}
			if(Show_0_or_Space==SHOW_SPACE && electmp==0x00 && i<2)
			{	//在还没遇到非零数字的时候,则如果本字符是0,则显示空格
			        find_asc(0x4000F000);		/* -  	- */
			    	 write_lcd_buf_ascii(2,64+i*16);
			}
			else	//在此之前已经遇到过非零字符或者本字符为非零字符,   则如果是0,则显示0
			{
				Show_0_or_Space = (bool)SHOW_0;
				addr=0x4000F100+electmp*16;
			    	find_asc(addr);					/*  --电量的整数部分的第 i 个值的高位--  */
			    	write_lcd_buf_ascii(2,64+i*16);
	 		}
   		}
   		find_asc(0x4000F0E0);				/*  --. --  */
   		write_lcd_buf_ascii(2,104);
  
   		electmp=((elec[0]-0x33)>>4)&0x0F;	
      		if(electmp>0x09)
   		{
   			electmp = 0x00;
   		}
   		addr=0x4000F100+electmp*16;
   		find_asc(addr);					/*  --电量的小数部分的高位--  */
   		write_lcd_buf_ascii(2,112);
  
   		electmp=(elec[0]-0x33)&0x0F;
   		if(electmp>0x09)
   		{
   			electmp = 0x00;
   		}   
   		addr=0x4000F100+electmp*16;
   		find_asc(addr);					/*  --电量的小数部分的低位--  */
   		write_lcd_buf_ascii(2,120);
  	}
	Display_Date_Time(date,time);
}

/*******************************************************************
函数名称:   Display_Date_Time
功能描述:   显示日期和时间
*******************************************************************/
void	Display_Date_Time(u8 * date, u8* time )
{
	/**********07-05-21 KWH***************************/
  	u8 datetmp,i;
  	u32 addr;//偏移地址
  	u8 timetmp;

  	find_chinese(0x4000F7C0);		/*  --  --*/
  	write_lcd_buf_chinese(4,0);
  	find_chinese(0x4000F7C0);		/*  --  --*/
  	write_lcd_buf_chinese(4,16);  
  	for(i=0;i<2;i++)/*07-05-*/
  	{
    		datetmp=(date[i]>>4)&0x0F;
    		addr=0x4000F100+datetmp*16;
    		find_asc(addr);				/*年月的高位*/
    		write_lcd_buf_ascii(4,32+i*24);
    
    		datetmp=(date[i])&0x0F;
    		addr=0x4000F100+datetmp*16;
    		find_asc(addr);				/*年月的低位*/
    		write_lcd_buf_ascii(4,40+i*24);
    		find_asc(0x4000F0D0);				/*  --  -  --*/
    		write_lcd_buf_ascii(4,48+i*24);    
  	}
	
  	datetmp=(date[2]>>4)&0x0F;		
  	addr=0x4000F100+datetmp*16;
  	find_asc(addr);						/*-- 日的高位 --*/
  	write_lcd_buf_ascii(4,80);
  	datetmp=(date[2])&0x0F;
  	addr=0x4000F100+datetmp*16;
  	find_asc(addr);						/*-- 日的低位 --*/
  	write_lcd_buf_ascii(4,88);
  
  	find_asc(0x4000F000);				/*--      -- */
  	write_lcd_buf_ascii(4,96); 
  	find_asc(0x4000F4B0);				/*--  k   --*/
  	write_lcd_buf_ascii(4,104);
  	find_asc(0x4000F370);				/*--  W --*/
  	write_lcd_buf_ascii(4,112);
  	find_asc(0x4000F480);
  	write_lcd_buf_ascii(4,120);			/*--   h --*/

	/*  XX:XX:XX  */
  	find_chinese(0x4000F7C0);
  	write_lcd_buf_chinese(6,0);/*--     --*/
  	find_chinese(0x4000F7C0);
  	write_lcd_buf_chinese(6,16);/*--     --*/
  
  	for(i=0;i<3;i++)
  	{
    		timetmp=(time[i]>>4)&0x0F;
    		addr=0x4000F100+timetmp*16;
    		find_asc(addr);				/*---  时分秒的高位 ---*/
    		write_lcd_buf_ascii(6,32+i*24);
    
    		timetmp=time[i]&0x0F;
    		addr=0x4000F100+timetmp*16;
    		find_asc(addr);				/*---  时分秒的低位 ---*/
    		write_lcd_buf_ascii(6,40+i*24);
    
    		if(i<2)
    		{
      			find_asc(0x4000F1A0);			/*---  :	---*/
      			write_lcd_buf_ascii(6,48+i*24);
    		}
  	}
	
  	find_chinese(0x4000F7C0);
  	write_lcd_buf_chinese(6,96);		/*--   --*/
  	find_chinese(0x4000F7C0);
  	write_lcd_buf_chinese(6,112);	/*--   --*/
	LCD_Show(lcd_buf);
}

#include "font.h"
#include <stdio.h>
#include <stdarg.h>

#define	SHOW_0			0
#define	SHOW_SPACE		1

/*******************************************************************
��������:   find_chinese
��������:   �������ֿ���Ѱ�������ַ�
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
��������:find_chinese
��������:��ASCII����Ѱ��ASCII��
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
��������:   write_lcd_buf_chinese
��������:   ��LCD��ʾ����дһ�������ַ�
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
��������:   write_lcd_buf_ascii
��������:   ��LCD��ʾ����дһ��ASCII��
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
��������:   displayfirst
��������:   ��ʾ��һ������
����:       lcd�ϵ�����ʾλ��
*******************************************************************/
void displayfirst(u8 lcdrow)
{
    memset(lcd_buf,0x00,1024);
    u8 Version[3] = {0x01 , 0x03, 0x08 };	//����汾��
    
    /*****��ѹ�����ɼ���********************/
    find_asc(0x4000F000);		/* -        - */
    write_lcd_buf_ascii(0,0);
    find_chinese(0x4000F6C0);	/* -    ��  - */
    write_lcd_buf_chinese(0,8);
    find_chinese(0x4000F6E0);	/* -    ѹ  - */
    write_lcd_buf_chinese(0,24);
    find_chinese(0x4000F7E0);	/* -    ��  - */
    write_lcd_buf_chinese(0,40);
    find_chinese(0x4000F800);	/* -    ��  - */
    write_lcd_buf_chinese(0,56);
    find_chinese(0x4000F700);	/* -    ��  - */
    write_lcd_buf_chinese(0,72);
    find_chinese(0x4000F720);	/* -    ��  - */
    write_lcd_buf_chinese(0,88);
    find_chinese(0x4000F740);	/* -    ��  - */
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
��������:   displaysecond
��������:   ��ʾ�ڶ�������
����:       lcd�ϵ�����ʾλ��
*******************************************************************/
void displaysecond(u8 lcdrow)
{
    u8 u8_temp;
    u8 Addr_485;
    memset(lcd_buf,0x00,1024);
    /***********��ַ000000000000***********************/
    find_chinese(0x4000F600);	/* -      ��      - */
    write_lcd_buf_chinese(0,0);
    find_chinese(0x4000F820);	/* -      ַ      - */
    write_lcd_buf_chinese(0,16);
    
    for(u8_temp=0;u8_temp<6;u8_temp++)
    {	//�������ֿ��е���ʼλ��Ϊ 0x4000F100
      Addr_485 = Base_ParaMeter.CJQ_485_Addr[5-u8_temp];
    
      if(((Addr_485>>4)&0x0F) >=0x0A)	
      {//�����ʾ0X0A -- 0X0F 
              find_asc(0x4000F210 + (((Addr_485>>4)&0x0F)-0x0A)*16);
      }
      else
      {//�����ʾ0X00 -- 0X09 
              find_asc(0x4000F100 + ((Addr_485>>4)&0x0F)*16);
      }
     write_lcd_buf_ascii(0,32+(u8_temp*2)*8);
    
      if((Addr_485&0x0F) >=0x0A)
      {//�����ʾ0X0A -- 0X0F
              find_asc(0x4000F210 + ((Addr_485&0x0F)-0x0A)*16);
      }
      else
      {//�����ʾ0X00 -- 0X09
              find_asc(0x4000F100 + (Addr_485&0x0F)*16);
      }
     write_lcd_buf_ascii(0,32+(u8_temp*2 + 1)*8);
    }
    /*************************�����գ�1��28��: 27**********/  
    find_chinese(0x4000F840);	/* -      ��      - */
    write_lcd_buf_chinese(2,0);
    find_chinese(0x4000F860);	/* -      ��      - */
    write_lcd_buf_chinese(2,16);
    find_chinese(0x4000F880);	/* -      ��      - */
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
    write_lcd_buf_ascii(2,112);		//��ʾ�����յ�ʮλ 
    u8_temp=DL_Freeze_Day_Global%10;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(2,120);		//��ʾ�����յĸ�λ 
    
    /*******************�ɼ������M):0050************/
    find_chinese(0x4000F700);	/* -       ��     - */ 
    write_lcd_buf_chinese(4,0);
    find_chinese(0x4000F720);	/* -       ��     - */ 
    write_lcd_buf_chinese(4,16);
    find_chinese(0x4000F8A0);	/* -       ��     - */ 
    write_lcd_buf_chinese(4,32);
    find_chinese(0x4000F8C0);	/* -       ��     - */ 
    write_lcd_buf_chinese(4,48);
    find_asc(0x4000F080);		/* -       (         - */ 
    write_lcd_buf_ascii(4,64);
    find_asc(0x4000F2D0);		/* -       M        - */ 
    write_lcd_buf_ascii(4,72);
    find_asc(0x4000F090);		/* -       )        - */ 
    write_lcd_buf_ascii(4,80);
    find_asc(0x4000F1A0);		/* -       :        - */ 
    write_lcd_buf_ascii(4,88);
    
    // ѭ���ɼ������ ʱ�� 4 �� BCD �� 
    u8_temp=InterVal_For_Cycle_Collectin_Global/1000;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(4,96);		//��ʾ�ɼ������ǧλ 
    u8_temp=(InterVal_For_Cycle_Collectin_Global%1000)/100;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(4,104);		//��ʾ�ɼ�����İ�λ
    u8_temp=(InterVal_For_Cycle_Collectin_Global%100)/10;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(4,112);		//��ʾ�ɼ������ʮλ
    u8_temp=InterVal_For_Cycle_Collectin_Global%10;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(4,120);		//��ʾ�ɼ�����ĸ�λ
    
    /********************�� 03 ��**************************************/
    find_asc(0x4000F000);			/* -                 - */ 
    write_lcd_buf_ascii(6,0);	
    find_asc(0x4000F000);			/* -                 - */ 
    write_lcd_buf_ascii(6,8);
    find_asc(0x4000F000);			/* -                 - */ 
    write_lcd_buf_ascii(6,16);
    find_asc(0x4000F000);			/* -                 - */ 
    write_lcd_buf_ascii(6,24);
    find_chinese(0x4000F780);		/* -      ��      - */ 
    write_lcd_buf_chinese(6,32);
    find_asc(0x4000F000);			/* -                  - */ 
    write_lcd_buf_ascii(6,48);
    
    //��ʾ�ܱ��� 
    u8_temp=(Base_ParaMeter.Meter_Total_Number)/10;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(6,56);		//��ʾ�ܱ�����ʮλ 
    u8_temp =(Base_ParaMeter.Meter_Total_Number)%10;
    find_asc(0x4000F100+u8_temp*16);
    write_lcd_buf_ascii(6,64);	   //��ʾ�ܱ����ĸ�λ 
    
    find_asc(0x4000F000);			/* -      	           - */ 
    write_lcd_buf_ascii(6,72);
    find_chinese(0x4000F7A0);		/* -       ��     - */ 
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
��������:   displaythird
��������:   ��ʾ��x����ܱ������
����:       Meter_Number: ���
            elec:         ����
            date:         ����
            time:         ʱ��
*******************************************************************/
void displaythird(u16 Meter_Number,u8 *elec,u8 *date,u8 *time)
{
	memset(lcd_buf,0x00,1024);
 	/*XX��0000000000XX*/
  	u8 i;
  	u16 tmp[2];
  	u32 addr;//ƫ�Ƶ�ַ
  	u8 electmp;
  	//����ʾ����ʱ,����ʾ��һ������ֵ֮ǰ���㶼Ӧ����ʾΪ�ո�
  	bool	Show_0_or_Space = (bool)SHOW_SPACE;	//Ĭ����ʾ�ո�	. 
  	tmp[0]=Meter_Number/10;
  	tmp[1]=Meter_Number%10;

  	addr=0x4000F100+tmp[0]*16;
  	find_asc(addr);				/*  -- ��ŵĸ�λ --  */
  	write_lcd_buf_ascii(0,0);
  	addr=0x4000F100+tmp[1]*16;
  	find_asc(addr);				/*  -- ��ŵĵ�λ --  */
  	write_lcd_buf_ascii(0,8);
  	find_chinese(0x4000F7A0);	/*  -- ��--  */
  	write_lcd_buf_chinese(0,16);

  	for(i=0;i<6;i++)
  	{

	  	tmp[1]=Meter_Asset_Number[5-i]&0x0F;
	  	tmp[0]=(Meter_Asset_Number[5-i]>>4)&0x0F;

	  	if(tmp[0]>=0x0A)
  		{
	  		find_asc(0x4000F210 + (tmp[0]-0x0A)*16 );		/*  -- �ʲ���ŵĸ�λ --  */
  		}
		else
		{
	  		find_asc(0x4000F100 + tmp[0]*16 );				/*  -- �ʲ���ŵĸ�λ --  */
		}
	  	write_lcd_buf_ascii(0,32+(2*i)*8);					/*  -- д�ʲ���ŵĸ�λ --  */


	  	if(tmp[1]>=0x0A)
  		{
	  		find_asc(0x4000F210 + (tmp[1]-0x0A)*16 );		/*  -- �ʲ���ŵĸ�λ --  */
  		}
		else
		{
	  		find_asc(0x4000F100 + tmp[1]*16 );				/*  -- �ʲ���ŵĸ�λ --  */
		}
		
	  	write_lcd_buf_ascii(0,32+(2*i+1)*8);
  	}

 	/****************�ܵ�����EEEEEEEEE*********/
  	find_chinese(0x4000F760);		/*  --��--  */
  	write_lcd_buf_chinese(2,0);
  	find_chinese(0x4000F7E0);		/*  --��--  */
  	write_lcd_buf_chinese(2,16); 
  	find_chinese(0x4000F800);		/*  --��--  */
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
   			/*  --�������������ֵ� i ��ֵ�ĸ�λ--  */
    			electmp=((elec[3-i]-0x33)>>4)&0x0F;	
   			if(electmp>0x09)
	   		{
	   			electmp = 0x00;
	   		}
			
			if(Show_0_or_Space==SHOW_SPACE && electmp==0x00)
			{	//�ڻ�û�����������ֵ�ʱ��,��������ַ���0,����ʾ�ո�
			        find_asc(0x4000F000);		/* -  	- */
			    	 write_lcd_buf_ascii(2,56+i*16);
			}
			else	//�ڴ�֮ǰ�Ѿ������������ַ����߱��ַ�Ϊ�����ַ�,   �������0,����ʾ0
			{
				Show_0_or_Space = (bool)SHOW_0;
				addr=0x4000F100+electmp*16;
			    	find_asc(addr);					/*  --�������������ֵĵ� i ��ֵ�ĸ�λ--  */
			    	write_lcd_buf_ascii(2,56+i*16);
	 		}

    			/*  --�������������ֵ� i ��ֵ�ĵ�λ--  */
    			electmp=(elec[3-i]-0x33)&0x0F;	
	   		if(electmp>0x09)
	   		{
	   			electmp = 0x00;
	   		}
			if(Show_0_or_Space==SHOW_SPACE && electmp==0x00 && i<2)
			{	//�ڻ�û�����������ֵ�ʱ��,��������ַ���0,����ʾ�ո�
			        find_asc(0x4000F000);		/* -  	- */
			    	 write_lcd_buf_ascii(2,64+i*16);
			}
			else	//�ڴ�֮ǰ�Ѿ������������ַ����߱��ַ�Ϊ�����ַ�,   �������0,����ʾ0
			{
				Show_0_or_Space = (bool)SHOW_0;
				addr=0x4000F100+electmp*16;
			    	find_asc(addr);					/*  --�������������ֵĵ� i ��ֵ�ĸ�λ--  */
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
   		find_asc(addr);					/*  --������С�����ֵĸ�λ--  */
   		write_lcd_buf_ascii(2,112);
  
   		electmp=(elec[0]-0x33)&0x0F;
   		if(electmp>0x09)
   		{
   			electmp = 0x00;
   		}   
   		addr=0x4000F100+electmp*16;
   		find_asc(addr);					/*  --������С�����ֵĵ�λ--  */
   		write_lcd_buf_ascii(2,120);
  	}
	Display_Date_Time(date,time);
}

/*******************************************************************
��������:   Display_Date_Time
��������:   ��ʾ���ں�ʱ��
*******************************************************************/
void	Display_Date_Time(u8 * date, u8* time )
{
	/**********07-05-21 KWH***************************/
  	u8 datetmp,i;
  	u32 addr;//ƫ�Ƶ�ַ
  	u8 timetmp;

  	find_chinese(0x4000F7C0);		/*  --  --*/
  	write_lcd_buf_chinese(4,0);
  	find_chinese(0x4000F7C0);		/*  --  --*/
  	write_lcd_buf_chinese(4,16);  
  	for(i=0;i<2;i++)/*07-05-*/
  	{
    		datetmp=(date[i]>>4)&0x0F;
    		addr=0x4000F100+datetmp*16;
    		find_asc(addr);				/*���µĸ�λ*/
    		write_lcd_buf_ascii(4,32+i*24);
    
    		datetmp=(date[i])&0x0F;
    		addr=0x4000F100+datetmp*16;
    		find_asc(addr);				/*���µĵ�λ*/
    		write_lcd_buf_ascii(4,40+i*24);
    		find_asc(0x4000F0D0);				/*  --  -  --*/
    		write_lcd_buf_ascii(4,48+i*24);    
  	}
	
  	datetmp=(date[2]>>4)&0x0F;		
  	addr=0x4000F100+datetmp*16;
  	find_asc(addr);						/*-- �յĸ�λ --*/
  	write_lcd_buf_ascii(4,80);
  	datetmp=(date[2])&0x0F;
  	addr=0x4000F100+datetmp*16;
  	find_asc(addr);						/*-- �յĵ�λ --*/
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
    		find_asc(addr);				/*---  ʱ����ĸ�λ ---*/
    		write_lcd_buf_ascii(6,32+i*24);
    
    		timetmp=time[i]&0x0F;
    		addr=0x4000F100+timetmp*16;
    		find_asc(addr);				/*---  ʱ����ĵ�λ ---*/
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

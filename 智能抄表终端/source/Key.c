#include "Key.h"
#include "Lcd.h"

/**************************************************************************
函数名 		: Key_Init() 
功能描述	: 对按键所用到的I/O口进行初始化
**************************************************************************/
void Key_Init(void)
{
	//初始化按键的方向为向内
	GPIO_Config(GPIO_KEY_DOWN,KEY_DOWN, GPIO_IN_TRI_CMOS);
	GPIO_Config(GPIO_KEY_UP,KEY_UP,GPIO_IN_TRI_TTL);	
}

/**************************************************************************
函数名 		: Key_Detect() 
功能描述	: 对按键进行检测
**************************************************************************/
void	Key_Detect()		
{
	u16		Gpio_Up, Gpio_Down;
	static 	u8  	Key_Pressed_Times_Up=0;	//三个键均使用同一个变量即可.因为人的手不会那么快，如果真有那么快的话，就应该使用三个不同的变量
	static	u8	Key_Pressed_Times_Down=0;
	Gpio_Up			= GPIO_WordRead(GPIO_KEY_UP);
	Gpio_Down 		= GPIO_WordRead(GPIO_KEY_DOWN);
	if((Gpio_Up&KEY_UP)==0x00)
	{	
		//检测是否有向上的键被按下,如果有则次数加1
		Key_Pressed_Times_Up++;
		if(Key_Pressed_Times_Up==KEY_PRESSED_TIME)
		{
			Global_Task_Flag |=TASK_FLAG_KEY_UP_PRESSED;
			Process_Key_Up();   //调用向上键的子程序
			Global_Task_Flag &= ~TASK_FLAG_KEY_UP_PRESSED;
		}
		return;
	}
	 if((Gpio_Down& KEY_DOWN) ==0x00)
	 {	
		//检测是否有向下的键被按下,如果有则次数加1
		Key_Pressed_Times_Down++;
		if(Key_Pressed_Times_Down==KEY_PRESSED_TIME)
		{
                     Process_Key_Down();    //调用向下键的子程序
		}
		return;
	 }
	Key_Pressed_Times_Up	=0;
	Key_Pressed_Times_Down	=0;
}

/**************************************************************************
函数名 		: Process_Key_Up() 
功能描述	: 对上行键进行处理
**************************************************************************/
void	Process_Key_Up()
{
	if(Lcd_Display_Meter==1)
	{//当前显示第一屏，按键后，应该显示最后一屏
		if(Base_ParaMeter.Meter_Total_Number==0)
		{//如果表的总数为0，则显示第二屏
			Lcd_Display_Meter  = 2;
			Lcd_Display_Frame = 2;
			Lcd_Display_XunXian();
			return;
		}
		else	//总表数不为零，显示最后一块表的电量
		{
			Lcd_Display_Meter  = 3;
			Lcd_Display_Frame = Base_ParaMeter.Meter_Total_Number+2;
			Lcd_Display_XunXian();
			return;						
		}
	}
	else	//当前显示的不是第一屏
	{
		Lcd_Display_Meter--;
		Lcd_Display_Frame--;
		Lcd_Display_XunXian();
              return;	
	}
}

/**************************************************************************
函数名 		: Process_Key_Down() 
功能描述	: 对下行键进行处理
**************************************************************************/
void	Process_Key_Down()
{
	Lcd_Display_Meter++;
	Lcd_Display_Frame++;

	if(Lcd_Display_Meter>3)
       {
		Lcd_Display_Meter=1;
	      Lcd_Display_Frame=1;
	}
 	Lcd_Display_XunXian();
}





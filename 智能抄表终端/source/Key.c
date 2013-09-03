#include "Key.h"
#include "Lcd.h"

/**************************************************************************
������ 		: Key_Init() 
��������	: �԰������õ���I/O�ڽ��г�ʼ��
**************************************************************************/
void Key_Init(void)
{
	//��ʼ�������ķ���Ϊ����
	GPIO_Config(GPIO_KEY_DOWN,KEY_DOWN, GPIO_IN_TRI_CMOS);
	GPIO_Config(GPIO_KEY_UP,KEY_UP,GPIO_IN_TRI_TTL);	
}

/**************************************************************************
������ 		: Key_Detect() 
��������	: �԰������м��
**************************************************************************/
void	Key_Detect()		
{
	u16		Gpio_Up, Gpio_Down;
	static 	u8  	Key_Pressed_Times_Up=0;	//��������ʹ��ͬһ����������.��Ϊ�˵��ֲ�����ô�죬���������ô��Ļ�����Ӧ��ʹ��������ͬ�ı���
	static	u8	Key_Pressed_Times_Down=0;
	Gpio_Up			= GPIO_WordRead(GPIO_KEY_UP);
	Gpio_Down 		= GPIO_WordRead(GPIO_KEY_DOWN);
	if((Gpio_Up&KEY_UP)==0x00)
	{	
		//����Ƿ������ϵļ�������,������������1
		Key_Pressed_Times_Up++;
		if(Key_Pressed_Times_Up==KEY_PRESSED_TIME)
		{
			Global_Task_Flag |=TASK_FLAG_KEY_UP_PRESSED;
			Process_Key_Up();   //�������ϼ����ӳ���
			Global_Task_Flag &= ~TASK_FLAG_KEY_UP_PRESSED;
		}
		return;
	}
	 if((Gpio_Down& KEY_DOWN) ==0x00)
	 {	
		//����Ƿ������µļ�������,������������1
		Key_Pressed_Times_Down++;
		if(Key_Pressed_Times_Down==KEY_PRESSED_TIME)
		{
                     Process_Key_Down();    //�������¼����ӳ���
		}
		return;
	 }
	Key_Pressed_Times_Up	=0;
	Key_Pressed_Times_Down	=0;
}

/**************************************************************************
������ 		: Process_Key_Up() 
��������	: �����м����д���
**************************************************************************/
void	Process_Key_Up()
{
	if(Lcd_Display_Meter==1)
	{//��ǰ��ʾ��һ����������Ӧ����ʾ���һ��
		if(Base_ParaMeter.Meter_Total_Number==0)
		{//����������Ϊ0������ʾ�ڶ���
			Lcd_Display_Meter  = 2;
			Lcd_Display_Frame = 2;
			Lcd_Display_XunXian();
			return;
		}
		else	//�ܱ�����Ϊ�㣬��ʾ���һ���ĵ���
		{
			Lcd_Display_Meter  = 3;
			Lcd_Display_Frame = Base_ParaMeter.Meter_Total_Number+2;
			Lcd_Display_XunXian();
			return;						
		}
	}
	else	//��ǰ��ʾ�Ĳ��ǵ�һ��
	{
		Lcd_Display_Meter--;
		Lcd_Display_Frame--;
		Lcd_Display_XunXian();
              return;	
	}
}

/**************************************************************************
������ 		: Process_Key_Down() 
��������	: �����м����д���
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





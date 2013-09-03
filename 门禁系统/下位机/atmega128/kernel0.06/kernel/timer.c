#include "timer.h"
#include "../drivers/beep.h"
#include "../tasks.h"

TICK TickCount = 0;

void TickInit(void)
{
	//ֹͣ��ʱ��
	TCCR1B = 0x00;
	TickCount = 0;
	
	//��������ֵ 
	TCNT1L = TICK_COUNTER_LOW;
    TCNT1H = TICK_COUNTER_HIGH;
	
	TIMSK |= 0b00000000|BM(TOIE1);
	//��ʱ����Ƶ,��ʼ��ʱ
	TCCR1A = 0x00;
	TCCR1B = TIMER_PRESCALE;

}

//ʱ���жϴ������
SIGNAL(TIMER1_OVF_vect) {
	TickUpdate();
	
	if (TickCount >= 50) {
		
		tasks_init(); //����״̬��λ
		
		TickCount = 0;
		BEEP(1,25000);
		TickDisable();
	}
}

TICK TickGet(void)
{
	TICK temp_count;
	TIMSK &=~ BM(TOIE1);
	temp_count = TickCount;
	TIMSK |= BM(TOIE1);
	
    return temp_count;
}

void TickUpdate(void)
{	
	TCNT1H = TICK_COUNTER_HIGH;
    TCNT1L = TICK_COUNTER_LOW;
	TickCount++;
}

void TickDisable(void){
	TIMSK &= ~BM(TOIE1);
}
	
void TickEnable(void){
	TIMSK |= BM(TOIE1);
}







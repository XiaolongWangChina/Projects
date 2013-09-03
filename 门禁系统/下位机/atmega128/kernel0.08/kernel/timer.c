#include "timer.h"
#include "../drivers/beep.h"
#include "tasks.h"
#include "scheduler.h"

TICK TickCount = 0;

void TickInit(void)
{
	TCCR1B = 0x00;
	TickCount = 0;

	TCNT1L = TICK_COUNTER_LOW;
	TCNT1H = TICK_COUNTER_HIGH;

	TIMSK |= 0b00000000|BM(TOIE1);
	TCCR1A = 0x00;
	TCCR1B = TIMER_PRESCALE;

}

SIGNAL(TIMER1_OVF_vect) {
	TickUpdate();

	if (TickCount >= 5000) {

		tasks_init();

		TickCount = 0;

		BEEP(2, 250900);

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


// 计时器0，用于更新等待进程时间片
void Tick0Init(void)
{
	TCCR0 = 0;
	TCCR0 |= (1<<CS02);  // 256预分频
	TIMSK |= (1<< TOIE0); // T0溢出中断允许                  
	TCNT0 = 100;         // 置计数起始值
}


SIGNAL(SIG_OVERFLOW0)
{
	//更新等待任务的时间片
	update_ticks();
	TCNT0=100;
}

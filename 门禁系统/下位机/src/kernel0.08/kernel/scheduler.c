#include "scheduler.h"
#include "../drivers/beep.h"

void task_create(void (*task)(void), unsigned char *stack, unsigned char task_id) {
	unsigned char i;
	//任务低位地址入栈
	*stack-- = (unsigned int)task;
	//任务高位地址入栈
	*stack-- = (unsigned int)task >> 8;

	*stack-- = 0x00; // R1 __zero_reg__
	*stack-- = 0x00; // R0 __tmp_reg__
	*stack-- = 0x80; //SREG 开启全局中断

	//其他寄存器
	for (i = 0; i < 14; i++) {
		*stack-- = i;
	}

	tcb[task_id].stack_top = (unsigned int)stack;

	ready_table |= (0x01 << task_id);
}

//开始任务调度
//从空闲时调度任务开始
void start_schedule(void) {
	current = TASKS;
	SP = tcb[TASKS].stack_top + 17;

	__asm__ __volatile__("reti		\n\t");
}

void schedule(void) {
	//  根据中断时保存寄存器的次序入栈，模拟一次中断后，入栈的情况  
	__asm__ __volatile__("PUSH __zero_reg__         \n\t");  //R1
	__asm__ __volatile__("PUSH __tmp_reg__          \n\t");  //R0 
	__asm__ __volatile__("IN   __tmp_reg__,__SREG__ \n \t"); //保存状态寄存器SREG
	__asm__ __volatile__("PUSH __tmp_reg__          \n\t");
	__asm__ __volatile__("CLR  __zero_reg__         \n\t");  //R0重新清零
	__asm__ __volatile__("PUSH R18                  \n\t");
	__asm__ __volatile__("PUSH R19                  \n\t");
	__asm__ __volatile__("PUSH R20                  \n\t");
	__asm__ __volatile__("PUSH R21                  \n\t");
	__asm__ __volatile__("PUSH R22                  \n\t");
	__asm__ __volatile__("PUSH R23                  \n\t");
	__asm__ __volatile__("PUSH R24                  \n\t");
	__asm__ __volatile__("PUSH R25                  \n\t");
	__asm__ __volatile__("PUSH R26                  \n\t");
	__asm__ __volatile__("PUSH R27                  \n\t");
	__asm__ __volatile__("PUSH R30                  \n\t");    
	__asm__ __volatile__("PUSH R31                  \n\t");
	__asm__ __volatile__("PUSH R28                  \n\t");  //R28与 R29用于建立在堆栈上的指针
	__asm__ __volatile__("PUSH R29                  \n \t"); //入栈完成

	tcb[current].stack_top=SP;           // 将正在运行的任务的堆栈底保存

	//选择下一个可以调度的任务
	//轮转调度
	unsigned char OSNextTaskID = (current + 1) % TASKS;
	while (OSNextTaskID != current) {
		if (ready_table & (0x01 << OSNextTaskID)) {
			break;
		}
	}
	if (OSNextTaskID == current) {
		current = TASKS; //当没有可运行的任务时，选择空闲时调度任务
	} else {
		current = OSNextTaskID;
	}

	cli();  // 保护堆栈转换
	SP=tcb[current].stack_top;
	sei();

	//根据中断时的出栈次序    
	__asm__ __volatile__("POP  R29                  \n\t");    
	__asm__ __volatile__("POP  R28                  \n\t");        
	__asm__ __volatile__("POP  R31                  \n\t");    
	__asm__ __volatile__("POP  R30                  \n\t");    
	__asm__ __volatile__("POP  R27                  \n\t");    
	__asm__ __volatile__("POP  R26                  \n\t");    
	__asm__ __volatile__("POP  R25                  \n\t");    
	__asm__ __volatile__("POP  R24                  \n\t");    
	__asm__ __volatile__("POP  R23                  \n\t");    
	__asm__ __volatile__("POP  R22                  \n\t");    
	__asm__ __volatile__("POP  R21                  \n\t");    
	__asm__ __volatile__("POP  R20                  \n\t");    
	__asm__ __volatile__("POP  R19                  \n\t");
	__asm__ __volatile__("POP  R18                  \n\t");
	__asm__ __volatile__("POP  __tmp_reg__          \n \t");      	//SERG 出栈并恢复
	__asm__ __volatile__("OUT  __SREG__,__tmp_reg__ \n\t");		//
	__asm__ __volatile__("POP  __tmp_reg__          \n \t");      	//R0 出栈
	__asm__ __volatile__("POP  __zero_reg__         \n \t");      	//R1 出栈
	//中断时出栈完成
}

void task_delay(unsigned int ticks) {
	if (ticks > 0) {
		ready_table &= ~(0x01 << current);
		tcb[current].wait_ticks = ticks;
		//重新调度
		schedule();
	}
}

//更新等待进程的wait_ticks
void update_ticks(void) {
	unsigned int i;
	for (i = 0; i < TASKS; i++) {
		if (tcb[i].wait_ticks) {
			tcb[i].wait_ticks--;
			if (tcb[i].wait_ticks == 0) {
				ready_table |= (0x01 << i);
			}
		}
	}
}

//空闲时调度任务
void scheduler(void) {
	while (1) {
		schedule();
	}
}

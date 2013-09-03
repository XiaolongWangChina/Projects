#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "../include.h"

// 任务运行就绪表
register unsigned char ready_table	asm("r2");
//正在运行的任务
register unsigned char current 		asm("r3");
register unsigned char temp_r4		asm("r4");
register unsigned char temp_r5  	asm("r5");
register unsigned char temp_r6  	asm("r6");
register unsigned char temp_r7  	asm("r7");
register unsigned char temp_r8  	asm("r8");
register unsigned char temp_r9  	asm("r9");
register unsigned char temp_r10 	asm("r10");
register unsigned char temp_r11 	asm("r11");
register unsigned char temp_r12 	asm("r12");
register unsigned char temp_r13 	asm("r13");
register unsigned char temp_r14 	asm("r14");
register unsigned char temp_r15 	asm("r15");
register unsigned char temp_r16 	asm("r16");
register unsigned char temp_r17 	asm("r17");

#define TASKS 5

//任务控制块
struct task_control_block {
	unsigned int stack_top;
	unsigned int wait_ticks;
} tcb[TASKS + 1];

void task_create(void (*task)(void), unsigned char *stack, unsigned char task_id);

void start_schedule(void);

void schedule(void);

void task_delay(unsigned int ticks);

void update_ticks(void);

void scheduler(void);

#endif

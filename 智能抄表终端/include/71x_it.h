/*************************************************************************************************
*File Name   : 71x_it.h
*Description : 该文件定义了所有的中断处理函数
*Node(s)     : 
*              创建文件
**************************************************************************************************/

#ifndef _71x_IT_H
#define _71x_IT_H

#include "71x_lib.h"

void Undefined_Handler   (void);
void FIQ_Handler         (void);
void SWI_Handler         (void);
void Prefetch_Handler    (void);
void Abort_Handler       (void);
void WDG_IRQHandler      (void);
void UART0_IRQHandler    (void);
void UART1_IRQHandler    (void);
void UART3_IRQHandler    (void);
void T2TIMI_IRQHandler   (void);

#endif /* _71x_IT_H */

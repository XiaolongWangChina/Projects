/*************************************************************************************************
*File Name   : 71x_it.h
*Description : ���ļ����������е��жϴ�����
*Node(s)     : 
*              �����ļ�
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
